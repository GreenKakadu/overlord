/***************************************************************************
                          LocationEntity.cpp
                             -------------------
    begin                : Sun Dec 8 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include <algorithm>
#include "GameConfig.h"
#include "GameFacade.h"
#include "IntegerData.h"
#include "LocationEntity.h"
#include "UnitEntity.h"
#include "FactionEntity.h"
#include "DirectionVariety.h"
#include "TerrainRule.h"
#include "TitleRule.h"
#include "RaceRule.h"
#include "RulesCollection.h"
#include "EntitiesCollection.h"
#include "VarietiesCollection.h"
#include "MovementVariety.h"
#include "BasicExit.h"
#include "ExplicitExit.h"
#include "ReportElement.h"
#include "SkillLevelElement.h"
#include "ItemElement.h"
#include "ResourceElement.h"
#include "TitleElement.h"
#include "EvenConflict.h"
#include "MarketConflict.h"
#include "MarketStrategy.h"
#include "PrototypeManager.h"
#include "ConstructionEntity.h"
#include "ConstructionRule.h"
#include "BonusElement.h"
#include "CombatManager.h"
#include "SimpleMessage.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "ObservationCondition.h"
#include "WeatherRule.h"
#include "SeasonRule.h"
#include "EffectEntity.h"
#include "PlagueEffect.h"
#include "EffectRule.h"
#include "TurnReport.h"
#include "EventRule.h"

extern TerrainRule * findTerrainByTag(const string &tag);
extern ReportPattern * taxCollectedReporter;
extern int Roll_1Dx(int n);

LocationEntity sampleLocation("LOCATION",&sampleTokenEntity);
//EntitiesCollection <LocationEntity>   locations(new DataStorageHandler(gameConfig.getLocationsFile() ),&sampleLocation);

ReportPattern  *	pillagerMovesReporter = new
            ReportPattern("pillagerMovesReporter");
ReportPattern  *	pillagerInsideReporter = new
           ReportPattern("pillagerInsideReporter");
ReportPattern  *	pillagerWeaponsReporter = new
             ReportPattern("pillagerWeaponsReporter");
ReportPattern  *	pillagingUnavailableReporter = new
             ReportPattern("pillagingUnavailableReporter");
ReportPattern  *	pillageOwnerReporter = new
             ReportPattern("pillageOwnerReporter");

const int LocationEntity::migrationFactor = 4;
const int LocationEntity::pillagingFactor =20;
const int LocationEntity::recoveryFactor =4;
const int LocationEntity::growthFactor =2;
const int LocationEntity::battleFactor = 20;

LocationEntity::LocationEntity ( const LocationEntity * prototype ) : Entity(prototype)
{
  terrain_ = 0;
  owner_ = 0;
  race_ = 0;
  guard_ = 0;
	titleCenter_ = 0;
	isPillaged_ = false;
//  location_ = this;
  dailyConflict_ = new EvenConflict(this); // default
  monthlyConflict_ = new EvenConflict(this); // default
  market_ = 0;//dynamic_cast<MarketStrategy *>(sampleMarket.createInstanceOfSelf ());
  if(market_) market_->setLocation(this);
  combatManager_ = new CombatManager(this);
  totalMarketValue_ = 0;
  studentCounter_ = 0;
  teacherCounter_ = 0;
  landPrice_ = 0;
  landTotal_= 0;// 100;
  landFree_ = landTotal_;
  migration_ =0;
  population_ =0;
  optima_ =100;
  economy_ = 0;//100; // Default value
	nextWeather_ = 0;
	wages_ = 0;
	season_ = determineSeason();
        if(season_)
        {
	weather_ = season_->predictWeather();
        }
        else
        {
          weather_ = 0;
        }
	skillBonuses_= SkillBonusAttribute("SKILL");
	isInitialized_ = true;
	outerLocation_ = 0;
}

GameData * LocationEntity::createInstanceOfSelf()
{
   return CREATE_INSTANCE<LocationEntity> (this);
}



// Fill very basic info that is known even for locations that were not visited
// but only observed as adjacient locations 
LocationEntity * LocationEntity::createBasicImage(FactionEntity * referent)
{
    LocationEntity * location = new LocationEntity(sampleLocation);
    location->setTimeStamp(gameFacade->getGameTurn(), gameFacade->getCurrentDay());

//    if(gameFacade->getCurrentDay() == 2)
//    {
//    	//cerr<< print()<<" Location image created on day 2"<<endl;
//    }

    location->setTag(this->getTag());
    location->setName(this->getName());
    location->setTerrain(this->getTerrain());

    location->setX(this->getX() - referent->getX0()); //  relative!
    location->setY(this->getY() - referent->getY0()); // relative!
    location->isVisited_ = false;
    location->imageObservation_ =0;
    location->isImage_ = true;

    return location;
}

// fill location object for visited location with data 

LocationEntity * LocationEntity::makeVisitedImage(LocationEntity * image, LocationEntity * source)
{
    image->isVisited_ = true;

    image->setDescription(source->getDescription());
    image->setNextWeather(source->getNextWeather());
    image->owner_ = source->owner_;
    image->setTitleLocation(source->getTitleLocation());
    image->setOptima(source->getOptima());
    image->setPopulation(source->getPopulation());
    image->setWages(source->getWages());
    image->setLandPrice(source->getLandPrice());
    image->setTotalLand(source->getTotalLand());
    image->setEconomy(source->getEconomy());

    image->market_ = source->market_;
    image->titles_ = source->titles_;
    image->ownershipPolicy_ = source->ownershipPolicy_;
    image->skillBonuses_ = source->skillBonuses_;
    image->localItems_ = source->localItems_;
    image->effects_ = source->effects_;
    // Some exits may have conditional visibility but this is not implemented yet
    image->exits_ = source->exits_;
    //location->orders_ = this->orders_;//unseen now
	return image;
}


// Create complete location image for visited location.

LocationEntity * LocationEntity::createVisitedLocationImage(FactionEntity * referent, int observation)
{
    LocationEntity * locationImage = createBasicImage(referent);
    this->imageObservation_ = observation;;
     makeVisitedImage(locationImage,this);
    for (ConstructionIterator iter = constructions_.begin(); iter != constructions_.end(); iter++)
    {
        if (((*iter)->getStealth() <= observation)||((*iter)->getAdvertising()) ||
        		((*iter)->getFaction()->stanceAtLeast(referent,alliedStance)))
        {
            // create image
//            ConstructionEntity * imageToAdd = (*iter)->createConstructionImage(referent, observation);
              ConstructionEntity * imageToAdd =  referent->getTurnReport()->addConstructionImage((*iter), observation);
              if(imageToAdd)
              {
           locationImage->constructions_.push_back(imageToAdd);
              }
        }
    }
    for (UnitIterator iter = units_.begin(); iter != units_.end(); iter++)
    {
        if (((*iter)->getStealth() <= observation)||((*iter)->getAdvertising()) ||
         		((*iter)->getFaction()->stanceAtLeast(referent,alliedStance)))
        {
//           UnitEntity * imageToAdd = (*iter)->createUnitImage(referent, observation);
           UnitEntity * imageToAdd = referent->getTurnReport()->addUnitImage((*iter), observation);
           if(imageToAdd)
           {
           locationImage->units_.push_back(imageToAdd);
           }
           else
           {
        	   //cerr<<"image of "<<(*iter)->print()<<" is unseen"<<endl;
           }
        }
    }
    // Effects

    // Some resources may be hidden!
    for (ResourceElementIterator iter = this->resources_.begin();
            iter != this->resources_.end(); iter++)
    {
        bool canProspect = false;
        if ((*iter)->getResource()->getProspectSkill())
        {// Resource requires prospecting skill:
            //cout <<"Prospecting for "<< (*iter)->getResource()->print()<<" in " <<print()<<endl;
            for (TokenIterator tokenIter = this->visitors_.begin();
                    tokenIter != this->visitors_.end(); ++tokenIter)
            {// Checking all visitors:
                if ((*tokenIter)->getFaction() != referent)
                {// Not from this faction
                    continue;
                }
                if ((*tokenIter)->canProspect(*iter))
                {// Somebody who can prospect found
                    canProspect = true;
                    // cout <<(*tokenIter)->print()<< " can prospect "<<endl;
                    break;
                }
            }

            if (!canProspect) // Nobody who can prospect was found
            {
                continue;
            }

        } else // No need in prospect or proper prospector found
        {

            locationImage->resources_.push_back((*iter));
        }

    }
     
     // Events
     
     for(vector<Event *>::iterator iter = collectedEvents_.begin(); iter != collectedEvents_.end(); ++iter)
     {
         BasicCondition * condition = (*iter)->getEventRule()->getCondition();
         if(condition)
         {
             bool canObserve = false;
            for (TokenIterator tokenIter = this->visitors_.begin();
                    tokenIter != this->visitors_.end(); ++tokenIter)
            {// Checking all visitors:
                if ((*tokenIter)->getFaction() != referent)
                {// Not from this faction
                    continue;
                }
               if((*iter)->isObservableBy(*tokenIter)) 
                {// Somebody who can observe found
                    canObserve = true;
                    break;
                }
            }

            if (!canObserve) // Nobody who can observe this event was found. Go to next event.
            {
                continue;
            }
            
         }
         Event * eventImage = (*iter)->createEventImage(referent);
         if(eventImage)
         {
           locationImage->collectedEvents_.push_back(eventImage);
           referent->getTurnReport()->addEventImage(eventImage);
         }
         
     }
     
     
   return locationImage;
}


// Create location object for visited location. 
// It does not contain any information which may change during the day 
// or is dependent on observation or other abilities of visiting units
// I.e. Tokens (units, constructions, effects) are not included and 
// resources are not included
// Currently not used
LocationEntity * LocationEntity::createIncompleteVisitedImage(FactionEntity * referent)
{
    LocationEntity * location = createBasicImage(referent);
     makeVisitedImage(location,this);   
    return location;
}


// Add Tokens and resources info to location image. Not used
void LocationEntity::populateLocationImage(FactionEntity * referent, int observation)
{
	LocationEntity * location = gameFacade->locations[getTag()]; // find origin of image
   
    this->imageObservation_ =observation;
    // Some units and Constructions may be hidden


   for (ConstructionIterator iter = location->constructions_.begin(); iter != location->constructions_.end(); iter++)
    {
        if (((*iter)->getStealth() <= observation)||((*iter)->getAdvertising()) ||
        		((*iter)->getFaction()->stanceAtLeast(referent,alliedStance)))
        {
           ConstructionEntity * imageToAdd = referent->getTurnReport()->addConstructionImage((*iter),observation);
            if(imageToAdd)
           {
            this->constructions_.push_back(imageToAdd);
           }
        }
    }
    for (UnitIterator iter = location->units_.begin(); iter != location->units_.end(); iter++)
    {
        if (((*iter)->getStealth() <= observation)||((*iter)->getAdvertising()) ||
        		((*iter)->getFaction()->stanceAtLeast(referent,alliedStance)))
        {
           UnitEntity * imageToAdd = (*iter)->createUnitImage(referent, observation);
           if(imageToAdd)
           {
           referent->getTurnReport()->addUnitImage(imageToAdd,observation);
           //referent->getTurnReport()->addUnitImage(imageToAdd);
           this->units_.push_back(imageToAdd);
           }
        }
    }
    // Effects

    // Some resources may be hidden!

    for (ResourceElementIterator iter = location->resources_.begin();
            iter != location->resources_.end(); iter++)
    {
        bool canProspect = false;
        if ((*iter)->getResource()->getProspectSkill())
        {// Resource requires prospecting skill:
            //cout <<"Prospecting for "<< (*iter)->getResource()->print()<<" in " <<print()<<endl;
            for (TokenIterator tokenIter = location->visitors_.begin();
                    tokenIter != location->visitors_.end(); ++tokenIter)
            {// Checking all visitors:
                if ((*tokenIter)->getFaction() != referent)
                {// Not from this faction
                    continue;
                }
                if ((*tokenIter)->canProspect(*iter))
                {// Somebody who can prospect found
                    canProspect = true;
                    // cout <<(*tokenIter)->print()<< " can prospect "<<endl;
                    break;
                }
            }

            if (canProspect) // Nobody who can prospect was found
            {
                continue;
            }

        } else // No need in prospect or proper prospector found
        {

            this->resources_.push_back(*iter);
        }

    }
}


// Update location image with the data from another image


void LocationEntity::updateImage(LocationEntity *location)
{
  Entity::updateImage(location);

    if(location->isVisited_==false)
    {
        return;
    }


    if(this->isVisited_ ==false)
    {
        makeVisitedImage(this,location);
        this->setTimeStamp(gameFacade->getGameTurn(), gameFacade->getCurrentDay());
		return;
    }
	// Two cases of update:
	// 1. new image is newer: replace units and constructions
    // replace owner
	// 2. new image observation is higher: add missing units and constructions (actualy may replace)
	// In all cases add additional resources
     if(location->getTimeStamp() > this->getTimeStamp())// newer
    {
       this->constructions_ = location->constructions_;
       this->units_ = location->units_;
       this->owner_ = location->owner_;
        
    }
   else if(this->imageObservation_ <  location->imageObservation_ ) //???
    {
        this->constructions_ = location->constructions_;
        this->units_ = location->units_;
    }
    this->setTimeStamp(gameFacade->getGameTurn(), gameFacade->getCurrentDay());
    bool isFound;

    for (ResourceElementIterator iter0 = location->resources_.begin();
            iter0 != location->resources_.end(); ++iter0)
    {
        isFound = false;
        for (ResourceElementIterator iter = this->resources_.begin();
                iter != this->resources_.end(); iter++)
        {
            if ((*iter)->getResource() == (*iter0)->getResource())
            {
                isFound = true;
                continue;
            }
        }
        if(!isFound)
        {
          this->resources_.push_back(*iter0);
        }
    }
    // Events
    // Add new events that are not in the list yet.
     for(vector<Event *>::iterator iter = location->collectedEvents_.begin(); iter != location->collectedEvents_.end(); ++iter)
     {
         bool isEventFound = false;
         for(vector<Event *>::iterator iter1 = this->collectedEvents_.begin(); iter1 != this->collectedEvents_.end(); ++iter1)
         {
             if((*iter1)->getTag()==(*iter1)->getTag())
             {
                isEventFound = true; 
                break;
             }
           }
         if(!isEventFound)
         {
            this->collectedEvents_.push_back(*iter);
         }
     }
 
}


STATUS
LocationEntity::initialize        ( Parser *parser )
{
  string temp;
  
  if (parser->matchKeyword("TERRAIN"))
  {
    //	  setTerrain(terrains[parser->getWord()]);
    setTerrain(findTerrainByTag(parser->getWord()));
    return OK;
  }
  if (parser->matchKeyword("INIT"))
  {
    isInitialized_ = false;
    return OK;
  }
  if (parser->matchKeyword("WEATHER"))
  {
    weather_ =findWeatherByTag(parser->getWord());
    return OK;
  }
  if (parser->matchKeyword("LAND"))
  {
    landTotal_ = parser->getInteger();
    return OK;
  }
  if (parser->matchKeyword("OWNER"))
  {
      FactionEntity * faction = gameFacade->factions[parser->getWord()];
      LocationEntity * location = gameFacade->locations[parser->getWord()];
      setLegalOwner(faction,location);
    return OK;
  }
  //      if (parser->matchKeyword("GUARD"))
    //	{
      //    TokenEntity * guard =0;
      //    guard = units[parser->getWord()];
      //    if(guard == 0)
      //      guard = buildingsAndShips[parser->getWord()];
      //    setGuard(guard);
      //	  return OK;
      //	}
      if (parser->matchKeyword("UNIT"))
      {
	addUnitImmediatelly(gameFacade->units[ parser->getWord()]);
	return OK;
      }
      if (parser->matchKeyword("EFFECT"))
      {
	addEffect(gameFacade->effects[ parser->getWord()]);
	return OK;
      }
      if (parser->matchKeyword("CONSTRUCTION"))
      {
	addConstructionImmediatelly(gameFacade->buildingsAndShips[ parser->getWord()]);
	return OK;
      }
      if (parser->matchKeyword("POPULATION"))
      {
	population_= (parser->getInteger());
	temp = parser->getWord();
	if(!temp.empty())
	{
	  race_ = gameFacade->races[temp];
	  if( race_ != 0)
	    return OK;
	}
	race_ = gameFacade->races["man"];  // default
	return OK;
      }
      if (parser->matchKeyword("WAGES"))
      {
	wages_ = (parser->getInteger());
	return OK;
      }
      if (parser->matchKeyword("OPTIMA"))
      {
	optima_ = (parser->getInteger());
	return OK;
      }
      if (parser->matchKeyword("LANDPRICE"))
      {
	landPrice_ = (parser->getInteger());
	return OK;
      }
      if (parser->matchKeyword("RESOURCES"))
      {
	ResourceElement * newResource = ResourceElement::readStoredElement(parser);
	if(newResource)
	  resources_.push_back(newResource);
	return OK;
      }
      if (parser->matchKeyword("ITEM"))
      {
	ItemElement newItem = ItemElement::readItemElement(parser);
	if(newItem.isValidElement())
	  localItems_.push_back(newItem);
	return OK;
      }
      
      skillBonuses_.initialize(parser);
      
      /*     if (parser->matchKeyword("UNIT"))
      {
	UnitEntity * newUnit = units[parser->getWord()];
	if(newUnit)
	units_.push_back(newUnit);
	return OK;
      }*/
      /*      if (parser->matchKeyword("CLIMATE"))
      {
	climate_ = (parser->getInteger());
	return OK;
      }*/
      if (parser->matchKeyword("ECONOMY"))
      {
	economy_ = (parser->getInteger());
	return OK;
      }
      if (parser->matchKeyword("XY"))
      {
	x_ = (parser->getInteger());
	y_ = (parser->getInteger());
	return OK;
      }
      //      if (parser->matchKeyword("XYZ"))
      //	{
	//	  x_ = (parser->getInteger());
	//	  y_ = (parser->getInteger());
	//	  z_ = (parser->getInteger());
	//	  return OK;
	//	}
	
	if (parser->matchKeyword("EXIT"))
	{
	  
	  DirectionVariety * dir = gameFacade->directions[parser->getWord()];
	  if (dir == 0)
	    return OK;
	  LocationEntity * dest = gameFacade->locations[parser->getWord()];
	  if(dest == 0)
	    return OK;
	  BasicExit * newExit = new BasicExit(this,dir,dest);
	  //    newExit->read(parser);
	  exits_.push_back (newExit);
	}
	
	
	if (parser->matchKeyword("EXIT_EXPLICIT"))
	{
	  
	  DirectionVariety * dir = gameFacade->directions[parser->getWord()];
	  if (dir == 0)
	    return OK;
	  LocationEntity * dest = gameFacade->locations[parser->getWord()];
	  if(dest == 0)
	    return OK;
	  
	  int days;
	  MovementVariety * mode;
	  MovementMode <int> travelTimes;
	  for(int i=0;i<gameFacade->movementModes.size();i++)
	  {
	    if(parser->matchInteger())  // old Overlord compartibility
	    {
	      days = parser->getInteger();
	      mode = gameFacade->movementModes[parser->getInteger()];
	    }
	    else
	    {
	      if(!parser->matchWord().empty())
	      {
		mode =  gameFacade->movementModes[parser->getWord()];
		days = parser->getInteger();
	      }
	      else
		break;
	    }
	    
	    if (mode == 0)
	    {
	      cout << "Failed to read EXPLICIT EXIT data for " << this<<endl;
	      break;
	    }
	    
	    travelTimes[mode] = days;
	    
	  }
	  
	  
	  exits_.push_back ( new ExplicitExit(this,dir,dest,travelTimes));
	}
	if (parser->matchKeyword("EXIT_SKILL"))
	{
	  
	  DirectionVariety * dir = gameFacade->directions[parser->getWord()];
	  if (dir == 0)
	    return OK;
	  LocationEntity * dest = gameFacade->locations[parser->getWord()];
	  if(dest == 0)
	    return OK;
	  int days;
	  int i;
	  string tempName;
	  MovementVariety * mode;
	  MovementMode <int> travelTimes;
	  SkillRule * skill;
	  SkillLevelElement * skillRequirement;
	  for(i=0;i<gameFacade->movementModes.size();i++)
	  {
	    if(parser->matchInteger())
	      days = parser->getInteger();
	    else
	      break;
	    
	    if(parser->matchInteger())
	      mode = gameFacade->movementModes[parser->getInteger()];
	    else
	    {
	      tempName = parser->getWord();
	      if (tempName.empty())
		break;
	      mode =  gameFacade->movementModes[tempName];
	      if (mode == 0)
	      {
		skill = gameFacade->skills[tempName];
		if (skill)
		{
		  skillRequirement = new SkillLevelElement(skill, parser->getInteger());
		  //SkillLevelElement::readElement(parser);
		}
		break;
	      }
	      
	    }
	    
	    travelTimes[mode] = parser->getInteger();
	  }
	  
	  
	  //		exits_.push_back ( new SkillDirection(dir, dest, travelTimes, skillRequirement));
	}
	
	if (parser->matchKeyword("MARKET_TYPE"))
	{
	  string keyword = parser->getWord();
	  GameData * temp =  /*GameData::*/prototypeManager->findInRegistry(keyword);
	  if(temp == 0)
	  {
	    cerr << "Unknown market type " << keyword  << " for location " << print()<< endl;
	  }
	  else
	  {
	    market_ = dynamic_cast<MarketStrategy *>(temp ->createInstanceOfSelf ());
	    market_->setLocation(this);
	  }
	  
	  return OK;
	}
	
	
	if(market_) market_->initialize(parser);
	ownershipPolicy_.initialize(parser);
	
	// location-specific movement modifiers
	movementBonuses_.initialize(parser);
	titles_.initialize(parser);
	
	return Entity::initialize(parser);
	
	
}



void
LocationEntity::save(ostream &out)
{
  out <<endl<<getCollectionKeyword()<< " " <<tag_ << " "<<getKeyword() <<endl;
  getTimeStamp().save(out);
  if(!name_.empty()) out << "NAME " <<name_ << endl;
  if(!description_.empty()) out << "DESCRIPTION " <<description_  << endl;
  if(getTerrain())out<< "TERRAIN " << getTerrain()->getTag()<< endl;
  if(nextWeather_) out << "WEATHER "<< nextWeather_->getTag()<<endl;
  if(getLegalOwner()) out << "OWNER " << owner_->getTag()<<" "<<getTitleLocation()->getTag()<<endl;
//  if(getGuard()) out << "GUARD " << guard_->getTag()<<endl;
  if(optima_) out << "OPTIMA " << optima_<<endl;
  if(population_) out << "POPULATION " << population_<<endl;
  if(wages_) out << "WAGES " << wages_<<endl;
  if(landPrice_)  out << "LANDPRICE " << landPrice_ <<endl;
  if(landTotal_)  out << "LAND " <<   landTotal_ <<endl;

   if(market_) market_->save(out);

	titles_.save(out);

  ownershipPolicy_.save(out);

  ResourceElementIterator resourceIter;
  for ( resourceIter = resources_.begin(); resourceIter != resources_.end(); resourceIter++)
    {
          out << "RESOURCES ";
           (*resourceIter)->save(out);
    }
  skillBonuses_.save(out);
/*  for (BonusIterator bonusIter = skillBonuses_.begin(); bonusIter != skillBonuses_.end(); bonusIter++)
    {
        out << "SKILL " ;
        (*bonusIter)->save(out);
    }*/
  for (ConstructionIterator iter  = constructions_.begin(); iter != constructions_.end(); iter++)
    {
        out << "CONSTRUCTION " << (*iter)->getTag() << endl;
    }
  for (UnitIterator unitIter  = units_.begin(); unitIter != units_.end(); unitIter++)
    {
        out << "UNIT " << (*unitIter)->getTag() << endl;
    }
  for (ItemElementIterator iter  = localItems_.begin(); iter != localItems_.end(); ++iter)
    {
        out << "ITEM "; (*iter).save(out);
    }
  for (EffectIterator effIter  = effects_.begin(); effIter != effects_.end(); effIter++)
    {
        out << "EFFECT " << (*effIter)->getTag() << endl;
    }
  saveEvents(out);
//  if(climate_) out << "CLIMATE " << climate_<<endl;
  if(economy_) out << "ECONOMY " << economy_<<endl;
       out << "XY " << x_ << " "<< y_<<" "<<endl;

  vector<OrderLine *>::iterator orderIter;
  for ( orderIter = orders_.begin(); orderIter != orders_.end(); orderIter++)
    {
           (*orderIter)->save(out);
    }
	DirectionIterator iter1;
  for ( iter1 = exits_.begin(); iter1 != exits_.end(); iter1++)
    {
           (*iter1)->save(out);
    }
  out<<endl;
}



/*
 * Checks data consistency
 */
void    LocationEntity::preprocessData()
{
  if(terrain_ == 0)
  {
    cout << this<<" has no  terrain defined. This may cause program to crash\n";
    return;
  }
  // activate titles
	titles_.activateAll();
   // Migration is based on last turn data so that player can estimate it
   // Calculate equilibrium Population for last turn data
    int populationEquilibrium =  (terrain_->getOptima() * economy_ )/ 100;
    populationExcess_ = population_ - populationEquilibrium;
    // Find inner and outer locations
    checkInnerLocations();
}




void LocationEntity::addBattle()
{
    economy_ = (economy_ * (100 - LocationEntity::battleFactor)) /100;
}

/*
 * Monthly post-processing
 */
void LocationEntity::postProcessData()
{
	if(isUnknownEntity())
	{
		return;
	}
	// Economy processing
    int population = getPopulation();
    if (population)
    {
        // Migration
    int pop0, pop1, delta;
    LocationEntity * dest;
    int migration = LocationEntity::migrationFactor;
    int time =0;
      for (DirectionIterator iter = exits_.begin(); iter != exits_.end(); iter++)
    {
          dest = (*iter)->getDestination();
          time = (*iter)->getTravelTime(walkingMode);
          if(time ==0)
           {
              continue;
          }
          if(time < LocationEntity::migrationFactor)
          {
              migration =LocationEntity::migrationFactor;
          }
          else
          {
              migration = time;
          }
         if(dest->getTerrain()->getOptima()==0)
          {
              continue;
          }
           pop1 = dest->getPopulationExcess();
           pop0 = getPopulationExcess();
           delta = (pop0 - pop1)/migration; // may be negative
           setPopulation(population - delta);
           //dest->setPopulation(dest->getPopulation() + delta);

    }



        int marketBonus = 0;
        int constructionBonus = 0;
        int economyMax = 100;
        // Calculate marketBonus
        marketBonus = totalMarketValue_ / population;
        // Calculate constructionBonus
        for (ConstructionIterator iter = constructions_.begin(); iter != constructions_.end(); ++iter)
        {
            constructionBonus += (*iter)->getEconomyBonus();
        }

        economyMax = 100 + marketBonus + constructionBonus;
        if (isPillaged())
        {
            economy_ = (economy_ * (100 - LocationEntity::pillagingFactor)) / 100;
        } else
        {
            economy_ = (economy_ * (100 + LocationEntity::recoveryFactor)) / 100;
            if (economy_ > economyMax)
            {
                economy_ = economyMax;
            }
        }

        population_ = (population_ * (100 + LocationEntity::growthFactor)) / 100;

    }

  if ( gameFacade->getGameConfig()->runMode != STARTING_TURN )
  {

		// For all tokens call upkeep method
	for(UnitIterator iter = units_.begin(); iter != units_.end(); ++iter)
	{
			(*iter)->payUpkeep();
	}
	//  for(ConstructionIterator iter = constructions_.begin(); iter != constructions_.end(); ++iter)
	//  {
	//    (*iter)->payUpkeep();
	//  }
  }

// Calculate next weather
	if(season_)
		{
		nextWeather_ = season_->predictWeather();
		}

  for(UnitIterator iter = units_.begin(); iter != units_.end(); ++iter)
  {
		(*iter)->recalculateTravelTime();
  }
  for(ConstructionIterator iter = constructions_.begin(); iter != constructions_.end(); ++iter)
  {
    (*iter)->recalculateTravelTime();
  }
	// Dead bodies decay
   ResourceElementIterator iter;
   ResourceElementIterator boneIter = resources_.end();
	ItemRule * deads = gameFacade->items["dead"];
	ItemRule * bones = gameFacade->items["bone"];
	int num;
	if((deads == 0) || (bones == 0))
	return;

  for(iter = resources_.begin(); iter != resources_.end();iter++)
  {
    if ( (*iter)->getResource() == bones)
      {
        num = (*iter)->getAvailableResource().roundDown();
	(*iter)->setAvailableResource((num * 90) /100);
	boneIter = iter;
	break;
      }
  }
  for(iter = resources_.begin(); iter != resources_.end();iter++)
  {
    if ( (*iter)->getResource() == deads)
      {
        num = (*iter)->getAvailableResource().roundDown();
	(*iter)->setAvailableResource((num * 80) /100);
	// Temp!
//         EffectRule * plagueRule = dynamic_cast<PlagueEffectRule *>(effectRules["plag"]);
//	if(num > 13)
//	if(num > plagueRule->getTrashold())
//	{
//	  cout << "================= Starting plague at "<<print() << "======================= "<<num<<endl;
//	  EffectEntity * plague = plagueRule->createEffect(this);
//	}

        if(boneIter != resources_.end())
	{
		(*iter)->setAvailableResource(((num * 20) /100) + (*boneIter)->getAvailableResource());
		break;
	}
	else
	{
          	resources_.push_back(new ResourceElement(bones,(num * 20) /100));
		break;
	}
      }
  }
//  for(iter = resources_.begin(); iter != resources_.end();iter++)
//  {
//      if(!(*iter)->getResource()->isRegeneratingResource() )
//      {
//         (*iter)->setResourceAmount((*iter)->getAvailableResource());
//      }
//  }
        for(int n =0; n < gameFacade->effectRules.size(); ++n)
 // for (EffectRulesIterator effIter  = effectRules.begin(); effIter != effectRules.end(); effIter++)
    {
         if(gameFacade->effectRules[n]->isToBeCreated(this))
         {
	  cout << "================= Starting "<<gameFacade->effectRules[n]->print()<<"  at "<<print() << "======================= "<<endl;
	  gameFacade->effectRules[n]->createEffect(this);
         }
    }
}



void LocationEntity::postPostProcessData()
{
    // Population migration
}



/*
 * Sets location owned
 */
void LocationEntity::setLegalOwner(FactionEntity * owner, LocationEntity * titleLocation)
{

 owner_ = owner;
  if(owner)
	{
     // cout<<"Setting owner of "<< tag_<<" "<<owner->getTag()<<endl;
  	owner->addVisitedLocation(this);
		setTitleLocation(titleLocation);
	}
	else
		setTitleLocation(0);
}



/*
 * When NPC Location changes owner NPC guards will slowly turn to new faction
 */
void LocationEntity::turnNpcGuards()
{
  for (vector <UnitEntity *>::iterator iter = unitsPresent().begin();
  iter != unitsPresent().end(); ++iter)
  {
    if((*iter)->isNpcGuard())
    {
      (*iter)->setLoyality(0);
    }
  }
}
/*
* Adds Unit to location
*/
void LocationEntity::addUnit(UnitEntity * unit)
{
  // may demand additional actions (update of location and unit's data)
  if(unit == 0)
    return;
  if(unit->isDisbanded())
    return;	   
  unitsToAdd_.push_back(unit);
  unit->setLocation(this);
  //		cout << "Unit " << unit << " added at " << *this << endl;
}



void  LocationEntity::addEffect(EffectEntity * effect)
{
  if(effect == 0)
    return;
  
  effects_.push_back(effect);
  effect->setLocation(this);
  if(effect->isTraced())
  {
  cout << "Effect " << effect->print() << " added at " << *this << endl;
  }
}
void LocationEntity::removeEffect(EffectEntity * effect)
{
    for (EffectIterator iter = effects_.begin(); iter != effects_.end();)
    {
        if((*iter)==effect )
        {
           // addReport(new ReportMessage(new UnaryMessage(plagueStopReporter,effect));
           effects_.erase (iter);
        }
        else
        {
            iter++;
        }
    }
}
void LocationEntity::addUnitImmediatelly(UnitEntity * unit)
{
	// may demand additional actions (update of location and unit's data)
		if(unit == 0)
			return;
/*		if(unit->isDisbanded())
			return;	 */  
	        units_.push_back(unit);
		visitors_.push_back(unit);
		unit->setLocation(this);
//		cout << "Unit " << unit << " added at " << *this << endl;
}



/*
 * Removes Unit from location
 */
void LocationEntity::removeUnit(UnitEntity * unit)
{
		if(unit == 0)
			return;
/*		if(unit->isDisbanded())
			return;	*/   
     unitsToRemove_.push_back(unit);
     unit->setLocation(0);
}



void LocationEntity::eraseRemovedUnit(UnitEntity * unit)
{
		if(unit == 0)
			return;
/* 		if(unit->isDisbanded())
			return;	*/   
   UnitIterator iter = find(units_.begin(),units_.end(),unit);
    if( iter == units_.end())
			return;

     units_.erase( iter);
}

void LocationEntity::addConstruction(ConstructionEntity * construction)
{
  if(construction == 0)
			return;
//      cout << "Construction "<< construction->print() <<" was added to " <<print()<<endl;
	   constructionsToAdd_.push_back(construction);
		construction->setLocation(this);
}


void LocationEntity::addConstructionImmediatelly(ConstructionEntity * construction)
{
  if(construction == 0)
			return;
//      cout << "Construction "<< construction->print() <<" was added to " <<print()<<endl;
	        constructions_.push_back(construction);
		visitors_.push_back(construction);
		construction->setLocation(this);
}



void LocationEntity::removeConstruction(ConstructionEntity * construction)
{
		if(construction == 0)
			return;
     constructionsToRemove_.push_back(construction);
     construction->setLocation(0);
}



void LocationEntity::eraseRemovedConstruction(ConstructionEntity * construction)
{
		if(construction == 0)
			return;
    ConstructionIterator iter = find(constructions_.begin(),constructions_.end(),construction);
    if( iter == constructionsToRemove_.end())
			return;

     constructions_.erase( iter);
}



void LocationEntity::eraseAllRemovedConstructions()
{
	for(ConstructionIterator iter = constructionsToRemove_.begin(); iter != constructionsToRemove_.end(); ++iter)
	{
		eraseRemovedConstruction(*iter);
	}
		constructionsToRemove_.clear();
}



void LocationEntity::eraseAllRemovedUnits()
{
	for(UnitIterator iter = unitsToRemove_.begin(); iter != unitsToRemove_.end(); ++iter)
	{
/*		if((*iter)->isDisbanded())
			continue;*/	   
		eraseRemovedUnit(*iter);
	}
	unitsToRemove_.clear();
}



void LocationEntity::addAllAddedConstructions()
{
	for(ConstructionIterator iter =constructionsToAdd_.begin(); iter != constructionsToAdd_.end(); ++iter)
	{
		constructions_.push_back(*iter);
		visitors_.push_back(*iter);
	}
	constructionsToAdd_.clear();
}


void LocationEntity::addAllAddedUnits()
{
	for(UnitIterator iter =unitsToAdd_.begin(); iter != unitsToAdd_.end(); ++iter)
	{
//cout << (*iter)->print() << " is added to "<< print()<<endl;
		if((*iter)->isDisbanded())
			continue;	   
		units_.push_back(*iter);
		visitors_.push_back(*iter);
	}
	unitsToAdd_.clear();
}
/** prints  report about location based on abilities of units present there  */
void LocationEntity::produceFactionReport(FactionEntity * faction, ReportPrinter & out)
{
    if(this->isTraced())
    {
        cout<<"produceFactionReport for "<<print()<<endl;
    }
  bool isFirst = true;
  out << print() << " " <<terrain_->getName();

  if(getLegalOwner())
  {
    out << " lands of ";
//  if(title)
//  out <<getTitleHolder()->printFullName()<< " of ";
    out <<getLegalOwner()->print();
    ownershipPolicy_.report(out);
  }
  if(getLegalOwner() != getRealOwner())
  {
    out << " This land is occupied by ";
    out <<getRealOwner()->print()<<".";
  }
  out << endl;
      faction->addKnowledge(terrain_);
  // Climate
      if(season_)
      {
	out << "It is "<< season_->getName()<<". ";
        faction->addKnowledge(season_);
      }
	if(weather_)
        {
            out << "This turn weather was "<< weather_->getName();
            faction->addKnowledge(weather_);
       }
	if(nextWeather_)
        {
            out << " next turn it should be "<< nextWeather_->getName()<<". ";
            faction->addKnowledge(nextWeather_);
        }

	if(!description_.empty()) out<<description_;


  // Location Titles: (report titles)
		titles_.reportAll(faction, out);


  if(population_)
    {
      out<< "Population: " << population_;
      if(race_)
      {
        out<<" "<<race_->getPluralName();
      faction->addKnowledge(race_);
      }
      out << ". Wages: $" << wages_
//			<< ", entertainment: $" << entertainment_
			<< ", tax income: $" <<taxes_<<".\n";

      if(market_) market_->produceFactionReport(faction,out);
    }

  if(!resources_.empty())
  {
    out<< "Resources: ";
    for(ResourceElementIterator iter = resources_.begin(); iter != resources_.end(); ++ iter)
    {
      bool canProspect = false;
      if((*iter)->getResource()->getProspectSkill()) // Resource requires prospecting skill
      {
	//cout <<"Prospecting for "<< (*iter)->getResource()->print()<<" in " <<print()<<endl;
	for(TokenIterator tokenIter = visitors_.begin(); tokenIter != visitors_.end(); ++tokenIter)
	{// Checking all visitors
	  if((*tokenIter)->getFaction() != faction)// Not from this faction
	  {
	    continue;
	  }
	  if((*tokenIter)->canProspect(*iter))// Somebody who can prospect found
	  {
	    canProspect = true;
	   // cout <<(*tokenIter)->print()<< " can prospect "<<endl;
	    break;
	  }
	}
	if(!canProspect) // Nobody who can prospect was found
	{
	  continue;
	}
      }
      if( isFirst)
      {
        isFirst = false;
      }
      else
        {
          out << ", ";
        }
      out<< *(*iter);
      faction->addKnowledge((*iter)->getResource());
    }
    out << ".\n";
  }


  if(!skillTeaching_.empty())
  {
    out<< "Skills available: ";
    isFirst = true;

    for(TeachingIterator iter2 = skillTeaching_.begin(); iter2 != skillTeaching_.end(); ++ iter2)
    {
      if( isFirst)
        {
          isFirst = false;
        }
        else
          {
            out << ", ";
          }
      out<< (*iter2);
      faction->addSkillKnowledge((*iter2)->getSkill(), (*iter2)->getLevel());
    }
    out << ".\n";
 }

		skillBonuses_.report(out);
		skillBonuses_.extractKnowledge(faction,1);

  out << "Directions: \n";

  for(ExitIterator iter= exits_.begin(); iter != exits_.end(); iter++)
  {
    (*iter)->produceReport(out);
  }
    out << "\n";

  int localObservation  = getFactionalObservation(faction);

  out << "Buildings: \n";

  for(ConstructionIterator iter= constructions_.begin(); iter != constructions_.end(); iter++)
  {
   faction->addKnowledge((*iter)->getConstructionType());
    if((*iter)->getFaction() == faction)
      (*iter)->privateReport(out);
    else
      (*iter)->publicReport(localObservation, out);
  }
    out << "\n";
  if(!localItems_.empty())
    {
      out << "Here are items laying on the ground: \n";
      for(ItemElementIterator iter= localItems_.begin(); iter != localItems_.end(); iter++)
        {
            if(iter != localItems_.begin())
              out << ", ";
            out <<(*iter).print();
        }
      out << ".\n";
    }
  out << "Units: \n";
//    if(faction == factions["f06"] && this == locations["L1"])
//    {
//       cout <<"ots"<<endl;
//    }
  for(UnitIterator iter= units_.begin(); iter != units_.end(); iter++)
  {
//     if(faction == factions["f06"] && this == locations["L1"])
//     {
//       cout <<(*iter)->print()<<endl;
//     }
   if((*iter)->getFaction() == faction)
	{
          if( (*iter)->isLeading(0) && (*iter)->getLeader()==0)// is stack leader -
		{	// print all stack
                  (*iter)->printStackMembersList(faction, out);
/*			vector< UnitEntity *> followers;
			followers = (*iter)->createStackMembersList(followers);
			for(UnitIterator iter1= followers.begin(); iter1 != followers.end(); iter1++)
			{
				(*iter1)->privateReport(out);
			}*/
		}
	else if ( (*iter)->getLeader())// not a stack leader but may be the leader of local substack of own units
		{
                  if((*iter)->getStackHead()->getFaction() != faction)// Stacked under foreign leader
                  {
                    if((*iter)->getLeader()->getFaction() != faction)
                    {
                    (*iter)->printStackMembersList(faction, out,true);
                    }
                    else
                    {
                      //(*iter)->privateReport(out);
		      continue; // is printed when substack printed
                    }
                  }
                  else
                  {
			continue; // is printed when stack printed
                  }
		}
	else // standalone unit
		{
			(*iter)->privateReport(out);
		}
	}
    else // foreign unit
	{
          if((*iter)->getStackHead()->getFaction() == faction)// Stacked under our leader
          {
            continue;// is printed when stack printed
          }
          else
          {
            (*iter)->publicReport(localObservation, out);
          }
	}
  }
     for (EffectIterator iter = effects_.begin(); iter != effects_.end();++iter)
    {
             out << (*iter)->publicReport();
    }
}
// Produce images for faction
        //create location image
        // populate it with all Tokens
        // create all Token Images
// Currently not used
LocationEntity * LocationEntity::produceDailyImages(FactionEntity * faction)
{
	int observation = this->getFactionalObservation(faction);
        //LocationEntity * image = this->createVisitedLocationImage(faction,observation);
        LocationEntity * image = this->createIncompleteVisitedImage(faction);
        image->populateLocationImage(faction,observation);
        return image;
}


//
// Best observation of faction at this location (allies uncluded)
//

int LocationEntity::getFactionalObservation(FactionEntity * faction)
{
 int maxLocalObservation = 0;
 int currentObservation;

  for(UnitIterator iter= units_.begin(); iter != units_.end(); iter++)
  {
 	  if(!(*iter)->isAlive() )
          {
              continue;
          }
 	  if((*iter)->getFaction() ==0 )
          {             
	      cerr <<"Day "<<gameFacade->getCurrentDay() <<" For "<<(*iter)->print()<<" no faction"<<endl;
              continue;
          }
	  if ((*iter)->getFaction()->stanceAtLeast(faction, alliedStance))
    {
        currentObservation = (*iter)->getObservation();
        if(currentObservation > maxLocalObservation)
        {
          maxLocalObservation = currentObservation;
        }
    }
  }
 return maxLocalObservation;
}



//
// Best observation of faction at this location (allies uncluded)
//
UnitEntity * LocationEntity::getFactionalObserver(FactionEntity * faction)
{
 int maxLocalObservation = -1 * VERY_BIG_NUMBER;
 int currentObservation;
 UnitEntity * currentObserver= 0;

  for(UnitIterator iter= units_.begin(); iter != units_.end(); iter++)
  {
	  if(!(*iter)->isAlive() )
          {
              continue;
          }
    if ((*iter)->getFaction()->stanceAtLeast(faction, alliedStance))
    {		
        currentObservation = (*iter)->getObservation();
        if(currentObservation > maxLocalObservation)
        {
          maxLocalObservation = currentObservation;
          currentObserver = *iter;
        }
    }
  }
 return currentObserver;
}



/** No descriptions */
int LocationEntity::getBonus(SkillRule * skill)
{
  return skillBonuses_.getSkillBonus(skill);
}


//
BasicExit *  LocationEntity::findExit(LocationEntity * dest)
{
  for(ExitIterator iter= exits_.begin(); iter != exits_.end(); ++iter)
  {
    if( (*iter)->getDestination() == dest)
      return (*iter);
    }
    return 0;
}


//
BasicExit *  LocationEntity::findExit(DirectionVariety * dir)
{
  for(ExitIterator iter= exits_.begin(); iter != exits_.end(); ++iter)
  {
    if( (*iter)->getDirection() == dir)
      return (*iter);
    }
    return 0;

}


//
void LocationEntity::setResource(ItemElement)
{

}


//
void LocationEntity::setResource(ItemRule * item, int num)
{
  ResourceElementIterator iter;
  for(iter = resources_.begin(); iter != resources_.end();iter++)
  {
    if ( (*iter)->getResource() == item)
    {
        (*iter)->setResourceAmount(num);
        return;
    }
  }
  resources_.push_back(new ResourceElement(item,num,num));
}

void LocationEntity::addResource(ItemRule * item, int num)
{
    int localNum = 0;
    ResourceElementIterator iter;
    for (iter = resources_.begin(); iter != resources_.end(); iter++)
    {
        if ((*iter)->getResource() == item)
        {
            if (item->isRegeneratingResource())
            {
                localNum = (*iter)->getResourceAmount();
                if (localNum != 0)
                {
                    (*iter)->setResourceAmount(localNum + num);
                    return;
                }
            } else // Non-regenerating like dead bodies
            {
                localNum = (*iter)->getAvailableResource().roundUp();
                if (localNum != 0)
                {
                    (*iter)->setAvailableResource(localNum + num);
                    return;
                }
            }

                resources_.push_back(new ResourceElement(item, num));
                return;
            
        }
    }
    resources_.push_back(new ResourceElement(item, num));
}
//
int  LocationEntity::getResource(ItemRule * item)
{
//        cout << "Looking for " <<item->print()<< " at " <<print()<<endl;
  ResourceElementIterator iter;
  for(iter = resources_.begin(); iter != resources_.end();iter++)
  {
    if ( (*iter)->getResource() == item)
      {
//        cout << "   ...have " <<(*iter)->getResource()->print()<<endl;
//        cout << (*iter)->getResourceAmount() << " of " <<item->print()<< " at " <<print()<<endl;
        return (*iter)->getResourceAmount();
      }
  }
//  ResourceElement  newResource (item, 1);
// 	ResourceElementIterator iter = find(resources_.begin(), resources_.end(), &newResource);
//	if(iter != resources_.end())
//		return (*iter)->getResourceAmount();
//	else
     return 0;

}



RationalNumber  LocationEntity::getAvailableResource(ItemRule * item)
{
  ResourceElementIterator iter;
  for(iter = resources_.begin(); iter != resources_.end();iter++)
  {
  if ( (*iter)->getResource() == item)
      {
        return (*iter)->getAvailableResource();
      }
  }
     return 0;
}
void LocationEntity::setAvailableResource(ItemRule * item, int num)
{
  ResourceElementIterator iter;
  for(iter = resources_.begin(); iter != resources_.end();iter++)
  {
    if ( (*iter)->getResource() == item)
    {
        (*iter)->setAvailableResource(num);
        return;
    }
  }
}



RationalNumber  LocationEntity::takeAvailableResource(ItemRule * item, RationalNumber amount)
{
  ResourceElementIterator iter;
  RationalNumber currentAmount;
  for(iter = resources_.begin(); iter != resources_.end();iter++)
  {
  if ( (*iter)->getResource() == item)
      {
         currentAmount  = (*iter)->getAvailableResource();
        if(currentAmount >= amount)
          {
            (*iter)->setAvailableResource(currentAmount-amount) ;
            return amount;
          }
        else
        {
            (*iter)->setAvailableResource(0);
            return currentAmount;
        }
      }
  }
     return 0;
}



void LocationEntity::addMonthlyConflictRequest(BasicCompetitiveRequest * request)
{
  monthlyConflict_->addRequest(request);
}



void LocationEntity::addDailyConflictRequest(BasicCompetitiveRequest * request)
{
//  cout << " Adding daily request \n";
  dailyConflict_->insertRequest(request);
}



void LocationEntity::processMonthlyConflict()
{
  monthlyConflict_->process();
}



void LocationEntity::dailyPreProcess()
{
	if(isUnknownEntity())
	{
		return;
	}
	// activate market: add local requests
 market_->dailyPreProcess();
 	isPillaged_ = false;
  setGuard(0);
}
void LocationEntity::processDailyConflict()
{
//  cout << "Processing daily conflict for "<< print()<< " \n";
  dailyConflict_->process();
}



void LocationEntity::addMarketRequest(MarketRequest * request)
{
  market_->addRequest(request);
}



void LocationEntity::processMarket()
{
	if(market_)
	{
		//  cout << "<><><><><><><><><><>   Processing Market conflict at " << print()<<endl;
		market_->process();
	}
}


void LocationEntity::harvestResource(ItemRule * item, RationalNumber& num)
{
  ResourceElementIterator iter;
  for(iter = resources_.begin(); iter != resources_.end();iter++)
  {
  if ( (*iter)->getResource() == item)
      {
        RationalNumber old((*iter)->getAvailableResource());
        (*iter)->setAvailableResource(old - num);
        return;
      }
  }
//  ResourceElement  newResource (item, 1);
// 	ResourceElementIterator iter = find(resources_.begin(), resources_.end(), &newResource);
//	if(iter != resources_.end())
//      {
//        int old = (*iter)->getResourceAmount();
//        (*iter)->setAvailableResource(old - num);
//      }
//  else
  {
   cerr << "Error: resource "<<item->print() << " was not found at "<< print()<<endl;
   }
}


LocationEntity * LocationEntity::findAdjacientLocation( int x, int y)
{
	if((x == 0)&&(y == 0))
	 return 0; // coordinates (0,0) are illegal - means uninitialized values

	for ( int i=0; i < gameFacade->locations.size();i++)
   {
    if(gameFacade->locations[i] ==0)
          continue;
		if((gameFacade->locations[i]->getX() == x) && (gameFacade->locations[i]->getY() == y))
		  return gameFacade->locations[i];
		}
	return 0;

}

    //Generate resourses
void LocationEntity::generateResourses()
{
    ResourceElementIterator resourceIter;
    for ( resourceIter = (getTerrain()->potentialResources).begin();
    resourceIter != (getTerrain()->potentialResources).end(); resourceIter++)
    {
      ItemRule * resource = (*resourceIter)->getResource();
      if (getResource(resource) != 0) // resource is already defined
	continue;
      int maxNumber = (*resourceIter)->getResourceAmount();
      int probability = ((*resourceIter)->getAvailableResource()).getValue();
      int roll = Roll_1Dx(100);
      if(roll < probability) // This resource will be at this location
      {
	int amount = Roll_1Dx(maxNumber) + 1;
	ResourceElement * newResource = new ResourceElement(resource, amount);
	resources_.push_back(newResource);
      }

    }
}
void LocationEntity::cleanResourses()
{
    ResourceElementIterator resourceIter;
    for ( resourceIter = resources_.begin();
    resourceIter != resources_.end();++resourceIter)
    {
        delete (*resourceIter);
    }
    resources_.clear();
}


STATUS LocationEntity::prepareData()
{
	if(isUnknownEntity())
	{
		return OK;
	}
	preprocessData();
  if(!isInitialized_ || (gameFacade->getGameConfig()->runMode == LOCATION_INITIALIZATION))
  {

        generateResourses();

    
    //Calculate neighbours
    if((x_ != 0)||(y_ != 0))
    {
      LocationEntity * dest = 0;
      DirectionVariety * dir = 0;
      BasicExit * newExit = 0;
      
      dest = findAdjacientLocation(x_,y_ + 2);
      if(dest && !findExit(dest) ) // Destination exists but exit to it not defined yet
      {
	dir = gameFacade->directions["N"];
	newExit = new BasicExit(this,dir,dest);
	exits_.push_back (newExit);
      }
      
      dest = findAdjacientLocation(x_,y_ - 2);
      if(dest && !findExit(dest) )
      {
	dir = gameFacade->directions["S"];
	newExit = new BasicExit(this,dir,dest);
	exits_.push_back (newExit);
      }
      
      dest = findAdjacientLocation(x_+1 ,y_+1);
      if(dest && !findExit(dest) )
      {
	dir = gameFacade->directions["NE"];
	newExit = new BasicExit(this,dir,dest);
	exits_.push_back (newExit);
      }
      
      dest = findAdjacientLocation(x_+1,y_-1);
      if(dest && !findExit(dest) )
      {
	dir = gameFacade->directions["SE"];
	newExit = new BasicExit(this,dir,dest);
	exits_.push_back (newExit);
      }
      
      dest = findAdjacientLocation(x_-1,y_+1);
      if(dest && !findExit(dest) )
      {
	dir = gameFacade->directions["NW"];
	newExit = new BasicExit(this,dir,dest);
	exits_.push_back (newExit);
      }
      
      dest = findAdjacientLocation(x_-1,y_-1);
      if(dest && !findExit(dest) )
      {
	dir = gameFacade->directions["SW"];
	newExit = new BasicExit(this,dir,dest);
	exits_.push_back (newExit);
      }
      
    }
  }
  if (population_ == 0)
  {
    if(terrain_ != 0)
      population_ = terrain_->getOptima();
    else
    {
      cerr << "no terrain for "<<print()<<endl;
    }
  }
  
  taxes_ = population_ * economy_ / 100; // may use something more complicated
  
  for(ConstructionIterator iter= constructions_.begin(); iter != constructions_.end(); iter++)
  {
    landFree_ -= (*iter)->getLandUse();
  }
  if  (landFree_ < 0 )
  {
    cout << "ERROR: buildings use too many land at "<<print() <<"\n";
    return IO_ERROR;
  }
  else
    return OK;
}

void LocationEntity::addExit(BasicExit * exit)
{
    exits_.push_back(exit);
}

void LocationEntity::pillage()
{
    setPillaged(true);
}


void  LocationEntity::updateTotalMarketValue(const int value)
{
  totalMarketValue_ += value;
}



int LocationEntity::getStudentCounter() const
{
  return studentCounter_ ;
}
bool LocationEntity::ordersToBeRepeated() const
{
  assert(studentCounter_ >= 0);
  assert(teacherCounter_ >= 0);

//  cout << "students: " <<studentCounter_ << " teachers: " <<teacherCounter_<<endl;
  return (studentCounter_ || teacherCounter_);
}


int LocationEntity::getTeacherCounter() const
{
  return teacherCounter_ ;
}



void LocationEntity::setStudentCounter(bool flag)
{
  if(flag)
    studentCounter_++;
  else
     studentCounter_--;
}

void LocationEntity::setTeacherCounter(bool flag)
{
  if(flag)
    teacherCounter_++;
  else
     teacherCounter_--;
}

bool LocationEntity::mayInterract(UnitEntity * unit)
{
    if( unit == 0)
       return false;
   if(unit->getGlobalLocation() != this )
        return false;
   else
        return true;

}


bool LocationEntity::useLand(int landSize)
{
  if (landSize < landFree_)
    return false;
  else
    landFree_ -= landSize;
  return true;
}



void LocationEntity::freeLand(int landSize)
{
    landFree_ += landSize;
}


// Titles ========================================================
void LocationEntity::addTitle(TitleElement * title)
{
  titles_.addTitle(title);
  // activate title benefits
}



void LocationEntity::removeTitle(TitleElement * title)
{
   titles_.removeTitle(title);
  // desactivate title benefits
}



void LocationEntity::deleteTitle(TitleRule * titleType)
{
  TitleElement * title = findTitle(titleType);
	if(title)
	{
	title->getTitleHolder()->removeTitle(title);
	removeTitle(title);
	delete title;
	}
}




TitleElement * LocationEntity::findTitle(TitleRule * titleType)
{
		return titles_.findTitle(titleType);
}


void LocationEntity::addNeighbours(FactionEntity * target)
{
  for(ExitIterator iter= exits_.begin(); iter != exits_.end(); iter++)
  {
     target->addDiscoveredLocation((*iter)->getDestination());
  }
}
void LocationEntity::addNeighbours(TurnReport * report)
{
  for(ExitIterator iter= exits_.begin(); iter != exits_.end(); iter++)
  {
	  report->addDiscoveredLocation((*iter)->getDestination());
  }
}
//========================================================================
//            PathFinding
//========================================================================
/*
 * Examine all adjacient locations.
 * Calculate total travel time for each adjacient location.
 * Ignore adjacient location if it is unpassable.
 * Ignore adjacient location if it is marked as "closed"
 *       (i.e. shortest path to it was already found)
 * Ignore adjacient location if we already know better path (totalDistance_)
 * Insert into adjacient location into priority queue
 *        (according to it's totalDistance_)
 */
void LocationEntity::examineNeighboringLocations(int &currentDistance,
                    vector <LocationEntity *> &openList,
                    vector <LocationEntity *> & examinedLocations)
{
  int localDistance;
  int distanceToNeighbourNew;
  int distanceToNeighbourOld;
  LocationEntity * currentNeighbour;

  for(ExitIterator iter= exits_.begin(); iter != exits_.end(); iter++)
  {
      currentNeighbour = (*iter)->getDestination();
//cout<< "     Examining "<< currentNeighbour->printTag();
      if(currentNeighbour->isClosed()) // Shortest way to this location
        {                               // was already found
//cout<< " - it is already closed\n";
          continue;
        }

      localDistance = (*iter)->getTravelTime(walkingMode);
      if( localDistance == 0) // Location can't be entered
        {                      // from this direction
//cout<< " - it is unpassable\n";
          continue;
        }

      distanceToNeighbourNew = currentDistance + localDistance;
      distanceToNeighbourOld =  currentNeighbour->getDistance();
      if(distanceToNeighbourOld == 0)  // Location was never examined
                                       // and not in the open list yet;
        {
//cout<< " - it never was examined. New distance is set to "<< distanceToNeighbourNew<<"\n";
          currentNeighbour->setDistance(distanceToNeighbourNew);
          examinedLocations.push_back(currentNeighbour);
          openList.push_back(currentNeighbour);
          continue;
        }

      if( distanceToNeighbourNew < distanceToNeighbourOld)
        {
//cout<< " - New distance is set to "<< distanceToNeighbourNew<<"\n";
          currentNeighbour->setDistance(distanceToNeighbourNew);
          continue;
        }
//cout<< " - Better path is known \n";
//      if(distanceToNeighbourNew > maxDistance)
//        continue;

    }
}



bool betterDistance( const locPtr & loc1, const locPtr & loc2)
{
  return (loc1->getDistance() < loc2->getDistance());
}



int LocationEntity::findWalkingDistance(LocationEntity * start, LocationEntity * end, int maxDistance = 64000)
{
  LocationEntity * current = start;
  vector <LocationEntity *> examinedLocations;
  int currentDistance  = 0;
  vector <LocationEntity *> openList;
  current->markClosed();
  current->setDistance(0);
  examinedLocations.push_back(current);

  while(current->getDistance()< maxDistance)
  {
//cout<< " Current location is "<<current->printTag() <<"\n";
    current->examineNeighboringLocations(currentDistance, openList,
                                examinedLocations);

//cout << " Before sorting: ";
//    for(vector <LocationEntity *>::iterator iter = openList.begin();iter !=openList.end();++iter)
//      cout<< (*iter)->printTag() <<" ";
//      cout<<endl;
//    std::nth_element(openList.begin(),openList.begin(),openList.end(),betterDistance);
    std::sort(openList.begin(),openList.end(),betterDistance);
//cout << " After sorting: ";
//    for(vector <LocationEntity *>::iterator iter = openList.begin();iter !=openList.end();++iter)
//      cout<< (*iter)->printTag() <<" ";
//      cout<<endl;

   if(openList.empty())
   {
    currentDistance =  0;
//    cout<< " No way!\n";
            break;
   }
   current = *(openList.begin());
//   cout<< " Next is "<<current->printTag() <<"\n";
   openList.erase(openList.begin());

   currentDistance  = current->getDistance();
   if( current == end)
    {
      cout<< " Done! Distance is "<< currentDistance<<"\n";
      break;
    }
   current->markClosed();
  } // end of while loop

 openList.clear();

 for(vector <LocationEntity *>::iterator iter = examinedLocations.begin();
                  iter != examinedLocations.end(); ++iter)
 {
   (*iter)->clearClosed();
   (*iter)->setDistance(0);
 }

 examinedLocations.clear();
 if(currentDistance >  maxDistance)
  {  //  Path not found
//    cout << "Path too long\n";
    return 0;
  }
 else
  return  currentDistance;

}



void LocationEntity::setMarketPrince(UnitEntity * prince)
{
  market_->setMerchantPrince(prince);
}



UnitEntity * LocationEntity::getMarketPrince()
{
  return market_->getMerchantPrince();
}



bool LocationEntity::promoteUnit(UnitEntity * unit1,UnitEntity * unit2)
{
  UnitIterator u1 = find(units_.begin(), units_.end(), unit1);
  UnitIterator u2 = find(units_.begin(), units_.end(), unit2);
  if(u1 >= u2)
    return false;
  if(u1 == units_.end() || u2 == units_.end())
    return false;
  units_.erase(u2);
  units_.insert(u1,unit2);
  return true;
}



void LocationEntity::addLocalItem(ItemRule * item, int number)
{
  for (ItemElementIterator iter  = localItems_.begin(); iter != localItems_.end(); ++iter)
    {
        if((*iter).getItemType() == item)
                {
                 (*iter).setItemNumber((*iter).getItemNumber()+ number);
                      return;
                }
    }
  localItems_.push_back(ItemElement(item,number));
}



void LocationEntity::removeLocalItem(ItemRule * item, int number)
{
  for (ItemElementIterator iter  = localItems_.begin(); iter != localItems_.end(); ++iter)
    {
        if((*iter).getItemType() == item)
        {
          if((*iter).getItemNumber() > number)
                    {
                      (*iter).setItemNumber((*iter).getItemNumber()-number);
                      return;
                    }
          else
          {
            localItems_.erase(iter);
            return;
          }
        }
    }
    cout << "ERROR on attempt to take " << number<<" of local "<<*item<<" from "<<*this<<endl;
}



int LocationEntity::hasLocalItem(ItemRule * item)
{
  for (ItemElementIterator iter  = localItems_.begin(); iter != localItems_.end(); ++iter)
    {
        if((*iter).getItemType() == item)
                return (*iter).getItemNumber();
    }
  return 0;
}



FactionEntity *  LocationEntity::getRealOwner() const
{
  if (guard_) // Occupied
      return guard_->getFaction();

  else
    return owner_;
}



void LocationEntity::setGuard(TokenEntity * guard)
{
  guard_ = guard;
}




TokenEntity * LocationEntity::getGuard() const
{
  return guard_;
}


// It is possible that more than one unit are guarding at the same location
// Only One of them may be real owner of location
// When current guard stops guarding new guard should be selected
// This may cause conflicts.
TokenEntity *  LocationEntity::selectNewGuard()
{
  for (UnitIterator unitIter  = units_.begin(); unitIter != units_.end(); unitIter++)
    {
        if((*unitIter)->isGuarding())
        {
          guard_ = *unitIter;
          return guard_;
        }
    }
  for (ConstructionIterator iter  = constructions_.begin(); iter != constructions_.end(); iter++)
    {
        if((*iter)->isGuarding())
        {
          guard_ = *iter;
          return guard_;
        }
    }
  guard_ = 0;
  return 0;
}



void LocationEntity::checkNewOwnerConflicts(TokenEntity * newOwner)
{

}




TokenEntity * LocationEntity::getBlockingPatrol(TokenEntity * traveler, MovementVariety *    movingMode, StanceVariety * stance)
{
// Units on patrol?
  for (UnitIterator unitIter  = units_.begin(); unitIter != units_.end(); unitIter++)
    {
        if((*unitIter)->isPatrolling())
        {
          if(!(*unitIter)->mayInterractTokenEntity(traveler)) // can't see
            continue;
        // Check Stance
          if(!(*unitIter)->getFaction()->stanceAtLeast(traveler,stance))
            { // Check  MovementMode
              int weight=0;
              int capacity=0;
               (*unitIter)->calculateTotalWeight(weight);
               (*unitIter)->calculateTotalCapacityMode(capacity, movingMode);

               if(capacity>= weight)
                return (*unitIter);
               else
                return  0;
            }

        }
    }
  for (ConstructionIterator iter  = constructions_.begin(); iter != constructions_.end(); iter++)
    {
        if((*iter)->isPatrolling())
        {   // Check Stance
          if(!(*iter)->getFaction()->stanceAtLeast(traveler,stance))
            { // Check  MovementMode
              int weight=0;
              int capacity=0;
               (*iter)->calculateTotalWeight(weight);
               (*iter)->calculateTotalCapacityMode(capacity, movingMode);
               if(capacity>= weight)
                return (*iter);
               else
                return  0;
            }
        }
    }

                return  0;


}



bool LocationEntity::tokenAllowedToEnter(TokenEntity * traveler, MovementVariety *    movingMode, TokenEntity * patrol)
{
  patrol = 0;
//	if (owner_)
//	cout << *traveler << " tries to enter " << *this << " owned by " << *owner_ <<   "Real Owner is " << *(getRealOwner()) <<endl;
  if (owner_ && owner_ == getRealOwner()) // Legal Owner?
  {
//	  int observation = getFactionalObservation(owner_);
    if(!owner_->mayObserveTokenEntity(traveler,this)) // can't see
      return true;
    return owner_->stanceAtLeast(traveler,ownershipPolicy_.getMovePermission());
  }

// Units on patrol?
  patrol =   getBlockingPatrol(traveler,movingMode,neutralStance);
 return (patrol == 0);
}



void LocationEntity::dailyUpdate()
{
	cleanTeachingOffers();
	addAllAddedConstructions();
	if(this->isTraced())
	{
	  cout <<"Adding Units to "<<print()<<endl;
	}
	addAllAddedUnits();
	eraseAllRemovedConstructions();
	eraseAllRemovedUnits();
  if(!isPillaged_)
	{
		if( (getLegalOwner() != 0) && (getLegalOwner() == getRealOwner()))
		{
	 		int taxes = getTaxes()/gameFacade->getGameConfig()->daysInMonth;
	 		if(taxes)
	 		{
//	   		getTitleLocation()->addLocalItem(cash, taxes);
        getLegalOwner()->addToFunds(cash, taxes);
     		getLegalOwner()->addReport(new TertiaryMessage( taxCollectedReporter,
			     new IntegerData(taxes), this, getTitleLocation()),
			     0, ObservationCondition::createObservationCondition(10));
	 		}
		}
	}
	else
	{
		if( getLegalOwner() != 0)
		{
    getLegalOwner()->addReport(new UnaryMessage( pillageOwnerReporter,this),
			     0, ObservationCondition::createObservationCondition(10));
		}
	}
}



bool LocationEntity::unitMayPillage(UnitEntity * unit, bool enableReport = true)
{
 if(getTaxes() == 0)// No taxes. Nothing to pillage.
 {
	if(enableReport) unit->addReport(new UnaryMessage(pillagingUnavailableReporter,
	                                this),0,0);
 	return false;
 }
 if(unit->getTravelStatus())  // Moving unit can't pillage
 {
	if(enableReport) unit->addReport(new SimpleMessage(pillagerMovesReporter),0,0);
  return false;
 }
 if(unit->getContainingConstruction()) // unit inside construction can't pillage
 {
	if(enableReport) unit->addReport(new
	              SimpleMessage(pillagerInsideReporter),0,0);
  return false;
 }
//  if(unit->getFiguresNumber() < 10 )  // Unit should have at least 20 figures in order to pillage
//  {
// 	if(enableReport) unit->addReport(new
// 	            SimpleMessage(pillager20FiguresReporter),0,0);
//   return false;
//  }
 if((unit->getDamage() + unit->getRangedDamage()) == 0) //unarned unit can't pillage
 {
 	if(enableReport) unit->addReport(new SimpleMessage(pillagerWeaponsReporter),0,0);
  return false;
 }

//  if(getPopulation()/ 50 > unit->getFiguresNumber()) // Need 1 soldier for 50 population to pillage
//  {
// 	if(enableReport) unit->addReport(new BinaryMessage( pillagerNotEnoughReporter,
// 	   new IntegerData(getPopulation()), this),0,0);
//   return false;
//  }

 return true;

}
// Weather and seasons ========================================================
WeatherRule * LocationEntity::getWeather() const
{
	return weather_;
}



void LocationEntity::setWeather(WeatherRule * weather)
{
	weather_ =weather;
}



SeasonRule * LocationEntity::getSeason() const
{
	return season_;
}


// May be location-specific
SeasonRule * LocationEntity::determineSeason()
{
 int index = (gameFacade->getGameTurn() + 1)%4;
 if(gameFacade->seasons.size()<index || index < 0)
 {
     cout<< "Season index "<<index<<" "<<gameFacade->seasons.size()<<" seasons"<<endl;
     return 0;
 }
 return gameFacade->seasons[index];
}

int LocationEntity::getMovementBonus(MovementVariety * mode)
{
	int bonus =0;
	bonus += enchantments_.getMovementBonus(mode);
	bonus += movementBonuses_.getMovementBonus(mode);
	return bonus;
}



int LocationEntity::getLocalBuyPrice(ItemRule * item)
{
 return market_->getLocalBuyPrice(item);
}


int LocationEntity::getLocalSellPrice(ItemRule * item)
{
 return market_->getLocalSellPrice(item);
}


int LocationEntity::getLocalRecruitPrice(RaceRule * race)
{
 return market_->getLocalRecruitPrice(race);
}


// Teaching
// void LocationEntity::addLocationTeachingOffer(TeachingOffer *offer)
// {
// 	teachingAcceptorOffers_.push_back(offer);
// }



// TeachingOffer * LocationEntity::findLocationTeachingOffer(SkillRule  * skill, int level)
// {
// }
//









EffectEntity * LocationEntity::findEffect(EffectRule * rule)
{
     for (EffectIterator iter = effects_.begin(); iter != effects_.end();++iter)
    {
        if((*iter)->getEffectType()  ==rule )
        {
            return (*iter);
        }
    }
     return 0;
}

void LocationEntity::extractAndAddKnowledge(FactionEntity * recipient, int parameter)
{
//    if(recipient==factions["f08"])
//    {
//        cout<<"<======== Extracting Location "<<getTag()<<endl;
//    }
    recipient->addKnowledge(getTerrain());

    recipient->addKnowledge(getWeather());

    //visible resources
    for (ResourceElementIterator iter = this->resources_.begin();
            iter != this->resources_.end(); iter++)
    {
        bool canProspect = false;
        if ((*iter)->getResource()->getProspectSkill())
        {// Resource requires prospecting skill:
            //cout <<"Prospecting for "<< (*iter)->getResource()->print()<<" in " <<print()<<endl;
            for (TokenIterator tokenIter = this->visitors_.begin();
                    tokenIter != this->visitors_.end(); ++tokenIter)
            {// Checking all visitors:
                if ((*tokenIter)->getFaction() != recipient)
                {// Not from this faction
                    continue;
                }
                if ((*tokenIter)->canProspect(*iter))
                {// Somebody who can prospect found
                    canProspect = true;
                    // cout <<(*tokenIter)->print()<< " can prospect "<<endl;
                    break;
                }
            }

            if (canProspect) // Nobody who can prospect was found
            {
                continue;
            }

        } else // No need in prospect or proper prospector found
        {

            recipient->addKnowledge((*iter)->getResource());
        }

    }
    // items on market
    if(market_)
    {
    market_->extractAndAddKnowledge(recipient,parameter);
    }

    recipient->addKnowledge(getRace());
    
  for (ItemElementIterator iter  = localItems_.begin(); iter != localItems_.end(); ++iter)
    {
        recipient->addKnowledge((*iter).getItemType());
    }
    // teaching offers
    for(vector <SkillLevelElement *>::iterator iter = skillTeaching_.begin(); iter != skillTeaching_.end();++iter)
    {
        recipient->addSkillKnowledge((*iter)->getSkill(),(*iter)->getLevel());
    }

     vector <SkillLevelElement *>  skillTeaching_;
    // titles
     for( TitleIterator iter = titles_.getAll()->begin(); iter != titles_.getAll()->end(); ++iter)
     {
        recipient->addKnowledge((*iter)->getTitle());

    }

    recipient->addKnowledge(getSeason());
     

}

//void LocationEntity::extractSkillKnowledge(Entity * recipient, int parameter)
//{
//
//}


void LocationEntity::checkInnerLocations()
{
  string dirName, relatedExitName;
  for(auto iter = exits_.begin(); iter != exits_.end(); ++iter)
    {
      dirName = (*iter)->getDirection()->getTag();
      LocationEntity * dest = (*iter)->getDestination();
      BasicExit * relatedExit = dest->findExit(this);
      if(relatedExit)
        {
          relatedExitName = relatedExit->getDirection()->getTag();
        }
      else
        {
          relatedExitName="";
        }
      if((dirName == "OUT")||(relatedExitName =="IN")) //  this is inner loc of dest
        {
          if(outerLocation_ ==0)
            {
            cout<<"<===Loc=== "<<print()<<" is inner location of "<<dest->print()<<endl;
            }
          outerLocation_ = dest;
          dest->innerLocations_[this->getTag()]= this;
          continue;
        }
      if((dirName == "IN")||(relatedExitName =="OUT")) // dest is inner loc of this
        {
          if(dest->outerLocation_ ==0)
            {
              cout<<"<===Loc==||= "<<dest->print()<<" is inner location of "<<print()<<endl;
            }
         dest->outerLocation_ = this;
          innerLocations_[dest->getTag()] = dest;
         continue;

        }
   // Add filter for multiple exits with the same destination
    }


//      ||(dirName == "UP")  ||(dirName == "DOWN")
//      ||(dirName == "PORT")||(dirName == "PORTAL"))

}
