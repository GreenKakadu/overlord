/***************************************************************************
                          LocationEntity.cpp
                             -------------------
    begin                : Sun Dec 8 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include <algorithm>
#include "GameConfig.h"
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

extern TerrainRule * findTerrainByTag(const string &tag);
extern ReportPattern * taxCollectedReporter;

LocationEntity sampleLocation("LOCATION",&sampleTokenEntity);
EntitiesCollection <LocationEntity>   locations(new DataStorageHandler(gameConfig.getLocationsFile() ));

ReportPattern  *	pillagerMovesReporter = new
            ReportPattern("pillagerMovesReporter");
ReportPattern  *	pillagerInsideReporter = new
           ReportPattern("pillagerInsideReporter");
ReportPattern  *	pillager20FiguresReporter = new
                   ReportPattern("pillager20FiguresReporter");
ReportPattern  *	pillagerWeaponsReporter = new
             ReportPattern("pillagerWeaponsReporter");
ReportPattern  *	pillagerNotEnoughReporter = new
                    ReportPattern("pillagerNotEnoughReporter");
ReportPattern  *	pillagingUnavailableReporter = new
             ReportPattern("pillagingUnavailableReporter");
ReportPattern  *	pillageOwnerReporter = new
             ReportPattern("pillageOwnerReporter");


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
  market_ = dynamic_cast<MarketStrategy *>(sampleMarket.createInstanceOfSelf ());
  market_->setLocation(this);
  combatManager_ = new CombatManager(this);
  totalMarketValue_ = 0;
  studentCounter_ = 0;
  teacherCounter_ = 0;
  landPrice_ = 0;
  landTotal_= 100;
  landFree_ = landTotal_;
	economy_ = 100;
	nextWeather_ = 0;
	wages_ = 0;
	season_ = determineSeason();
	weather_ = season_->predictWeather();
	skillBonuses_= SkillBonusAttribute("SKILL");
}

GameData * LocationEntity::createInstanceOfSelf()
{
   return CREATE_INSTANCE<LocationEntity> (this);
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

		setLegalOwner(factions[parser->getWord()],locations[parser->getWord()]);
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
	  addUnit(units[ parser->getWord()]);
	  return OK;
	}
      if (parser->matchKeyword("CONSTRUCTION"))
	{
	  addConstruction(buildingsAndShips[ parser->getWord()]);
    return OK;
	}
      if (parser->matchKeyword("POPULATION"))
	{
	  population_= (parser->getInteger());
    temp = parser->getWord();
    if(!temp.empty())
      {
        race_ = races[temp];
        if( race_ != 0)
	          return OK;
       }
    race_ = races["man"];  // default
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
      if (parser->matchKeyword("RESOURCE"))
	{
        ResourceElement * newResource = ResourceElement::readElement(parser);
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

      if (parser->matchKeyword("UNIT"))
	{
    UnitEntity * newUnit = units[parser->getWord()];
    if(newUnit)
      units_.push_back(newUnit);
    return OK;
	}
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

    DirectionVariety * dir = directions[parser->getWord()];
		if (dir == 0)
	  	return OK;
	  LocationEntity * dest = locations[parser->getWord()];
		if(dest == 0)
	  	return OK;
    BasicExit * newExit = new BasicExit(this,dir,dest);
//    newExit->read(parser);
		exits_.push_back (newExit);
 	}


       if (parser->matchKeyword("EXIT_EXPLICIT"))
 	{

    DirectionVariety * dir = directions[parser->getWord()];
		if (dir == 0)
	  	return OK;
	  LocationEntity * dest = locations[parser->getWord()];
		if(dest == 0)
	  	return OK;

		int days;
		MovementVariety * mode;
		MovementMode <int> travelTimes;
		for(int i=0;i<movementModes.size();i++)
		 {
			if(parser->matchInteger())  // old Overlord compartibility
      {
					days = parser->getInteger();
      		mode = movementModes[parser->getInteger()];
      }
			else
      {
        if(!parser->matchWord().empty())
        {
					mode =  movementModes[parser->getWord()];
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

    DirectionVariety * dir = directions[parser->getWord()];
		if (dir == 0)
	  	return OK;
	  LocationEntity * dest = locations[parser->getWord()];
		if(dest == 0)
	  	return OK;
		int days;
		int i;
		string tempName;
		MovementVariety * mode;
		MovementMode <int> travelTimes;
		SkillRule * skill;
		SkillLevelElement * skillRequirement;
		for(i=0;i<movementModes.size();i++)
		 {
			if(parser->matchInteger())
					days = parser->getInteger();
			else
					break;

			if(parser->matchInteger())
      		mode = movementModes[parser->getInteger()];
			else
				{
					tempName = parser->getWord();
					if (tempName.empty())
							break;
					mode =  movementModes[tempName];
					if (mode == 0)
						{
							skill = skills[tempName];
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
					cout << "Unknown market type " << keyword  << " for location " << print()<< endl;
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
  out << endl;
  out << keyword_ << " " <<tag_ << endl;
  if(!name_.empty()) out << "NAME " <<name_ << endl;
  if(!description_.empty()) out << "DESCRIPTION " <<description_  << endl;
  out<< "TERRAIN " << getTerrain()->getTag()<< endl;
	if(nextWeather_) out << "WEATHER "<< nextWeather_->getTag()<<endl;
  if(getLegalOwner()) out << "OWNER " << owner_->getTag()<<" "<<getTitleLocation()->getTag()<<endl;
//  if(getGuard()) out << "GUARD " << guard_->getTag()<<endl;
  if(optima_) out << "OPTIMA " << optima_<<endl;
  if(population_) out << "POPULATION " << population_<<endl;
  if(wages_) out << "WAGES " << wages_<<endl;
  if(landPrice_)  out << "LANDPRICE " << landPrice_ <<endl;
                  out << "LAND " <<   landTotal_ <<endl;

   market_->save(out);

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
  }
}



/*
 * Monthly post-processing
 */
void LocationEntity::postProcessData()
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

// Calculate next weather
		nextWeather_ = season_->predictWeather();

  for(UnitIterator iter = units_.begin(); iter != units_.end(); ++iter)
  {
		(*iter)->recalculateTravelTime();
  }
  for(ConstructionIterator iter = constructions_.begin(); iter != constructions_.end(); ++iter)
  {
    (*iter)->recalculateTravelTime();
  }

}



/*
 * Sets location owned
 */
void LocationEntity::setLegalOwner(FactionEntity * owner, LocationEntity * titleLocation)
{
  owner_ = owner;
  if(owner)
	{
  	owner->addVisitedLocation(this);
		setTitleLocation(titleLocation);
	}
	else
		setTitleLocation(0);
}



/*
 * Adds Unit to location
 */
void LocationEntity::addUnit(UnitEntity * unit)
{
	// may demand additional actions (update of location and unit's data)
		if(unit == 0)
			return;
	   units_.push_back(unit);
		unit->setLocation(this);
}



/*
 * Removes Unit from location
 */
void LocationEntity::removeUnit(UnitEntity * unit)
{
		if(unit == 0)
			return;
    UnitIterator iter = find(units_.begin(),units_.end(),unit);
    if( iter == units_.end())
			return;

     units_.erase( iter);
     unit->setLocation(0);
}



/** prints  report about location based on abilities of units present there  */
void LocationEntity::produceFactionReport(FactionEntity * faction, ReportPrinter & out)
{
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
	out << "It is "<< season_->getName()<<". ";
	if(weather_) out << "This turn weather was "<< weather_->getName();
	if(nextWeather_) out << " next turn it should be "<< nextWeather_->getName()<<". ";

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
      out << ". Wages: $" << wages_ << ", entertainment: $"
          << entertainment_ << ", tax income: $" <<taxes_<<".\n";

      if(market_) market_->produceFactionReport(faction,out);
    }

  if(!resources_.empty())
  {
    out<< "Resources: ";
    for(ResourceElementIterator iter = resources_.begin(); iter != resources_.end(); ++ iter)
    {
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

//    if(!skillBonuses_.empty())
//     {
//       out << " Skill bonuses: ";
//       for ( vector< BonusElement*>::iterator iter = skillBonuses_.begin();
//             iter != skillBonuses_.end(); iter++)
// 		    {
//           if( iter != skillBonuses_.begin())
//             {
//               out << ", ";
//             }
//           out << (*iter);
//           faction->addSkillKnowledge((*iter)->getSkill(), 1);
//         }
//       out <<". ";
//     }
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
  for(UnitIterator iter= units_.begin(); iter != units_.end(); iter++)
  {
    if((*iter)->getFaction() == faction)
      (*iter)->privateReport(out);
    else
      (*iter)->publicReport(localObservation, out);
  }
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
//  cout << "<><><><><><><><><><>   Processing Market conflict at " << print()<<endl;
  market_->process();
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
   cout << "Error: resource "<<item->print() << " was not found at "<< print()<<endl;
   }
}



STATUS LocationEntity::prepareData()
{

  taxes_ = population_ * economy_ / 100; // may use something more complicated

	for(ConstructionIterator iter= constructions_.begin(); iter != constructions_.end(); iter++)
  {
    landFree_ -= (*iter)->getLandUse();
  }
  if  (landFree_ < 0 )
    {
     cout << "ERROR: buildings use too many land \n";
     return IO_ERROR;
    }
  else
    return OK;
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

void LocationEntity::addConstruction(ConstructionEntity * construction)
{
  if(construction == 0)
			return;
//      cout << "Construction "<< construction->print() <<" was added to " <<print()<<endl;
	   constructions_.push_back(construction);
		construction->setLocation(this);
}



void LocationEntity::removeConstruction(ConstructionEntity * construction)
{
		if(construction == 0)
			return;
    ConstructionIterator iter = find(constructions_.begin(),constructions_.end(),construction);
    if( iter == constructions_.end())
			return;

     constructions_.erase( iter);
     construction->setLocation(0);
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
  title->getTitleHolder()->removeTitle(title);
  removeTitle(title);
  delete title;
}




TitleElement * LocationEntity::findTitle(TitleRule * titleType)
{
		return titles_.findTitle(titleType);
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
  if(!isPillaged_)
	{
		if( (getLegalOwner() != 0) && (getLegalOwner() == getRealOwner()))
		{
	 		int taxes = getTaxes()/gameConfig.daysInMonth;
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
 if(unit->getFiguresNumber() < 20 )  // Unit should have at least 20 figures in order to pillage
 {
	if(enableReport) unit->addReport(new
	            SimpleMessage(pillager20FiguresReporter),0,0);
  return false;
 }
 if(unit->getDamage()  == 0) //unarned unit can't pillage
 {
 	if(enableReport) unit->addReport(new SimpleMessage(pillagerWeaponsReporter),0,0);
  return false;
 }

 if(getPopulation()/ 50 > unit->getFiguresNumber()) // Need 1 soldier for 50 population to pillage
 {
	if(enableReport) unit->addReport(new BinaryMessage( pillagerNotEnoughReporter,
	   new IntegerData(getPopulation()), this),0,0);
  return false;
 }

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
 int index = (gameConfig.turn + 1)/4;
 return seasons[index];
}

int LocationEntity::getMovementBonus(MovementVariety * mode)
{
	int bonus =0;
	bonus += enchantments_.getMovementBonus(mode);
	bonus += movementBonuses_.getMovementBonus(mode);
	return bonus;
}
