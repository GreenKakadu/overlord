/***************************************************************************
                          LocationEntity.cpp 
                             -------------------
    begin                : Sun Dec 8 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
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
#include "ReportRecord.h"
#include "BasicExit.h"
#include "ExplicitExit.h"
#include "ReportElement.h"
#include "SkillLevelElement.h"
#include "ItemElement.h"
#include "ResourceElement.h"
#include "EvenConflict.h"
#include "MarketConflict.h"
#include "MarketStrategy.h"
#include "PrototypeManager.h"

extern EntitiesCollection <UnitEntity>     units;
extern EntitiesCollection <LocationEntity> locations;
extern RulesCollection    <TerrainRule>    terrains;
extern RulesCollection    <RaceRule>        races;
extern VarietiesCollection    <DirectionVariety>  directions;
extern RulesCollection    <TitleRule>     titles;
extern VarietiesCollection <MovementVariety> movementModes;
extern MarketStrategy     sampleMarket;

LocationEntity::LocationEntity ( const LocationEntity * prototype ) : PhysicalEntity(prototype)
{
  terrain_ = 0;
  location_ = this;
  dailyConflict_ = new EvenConflict(this); // default
  monthlyConflict_ = new EvenConflict(this); // default
  market_ = dynamic_cast<MarketStrategy *>(sampleMarket.createInstanceOfSelf ());
  market_->setLocation(this);
  totalMarketValue_ = 0;
  studentCounter_ = 0;
  teacherCounter_ = 0;
}

GameData * LocationEntity::createInstanceOfSelf()
{
   return CREATE_INSTANCE<LocationEntity> (this);
}


STATUS
LocationEntity::initialize        ( Parser *parser )
{
string temp;
	
      if (parser->matchKeyword ("NAME") )
        {
	  setName(parser->getText());
	  return OK;
	}
      if (parser->matchKeyword("DESCRIPTION"))
	{
	  setDescription(parser->getText());
	  return OK;
	}
      if (parser->matchKeyword("TERRAIN"))
	{
	  setTerrain(parser->getWord());
	  return OK;
	}
      if (parser->matchKeyword("UNIT"))
	{
	  addUnit(units[ parser->getWord()]);
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
      if (parser->matchKeyword("TITLE"))
	{
	  title_= titles[parser->getWord()];
    titleOwner_ = units[parser->getWord()];
	  return OK;
	}
      if (parser->matchKeyword("RESOURCE"))
	{
        ResourceElement * newResource = ResourceElement::readElement(parser);
        if(newResource)
          resources_.push_back(newResource);
	  return OK;
	}
      if (parser->matchKeyword("SKILL"))
	{
        BonusElement * newBonus = BonusElement::read(parser);
        if(newBonus)
          skillBonuses_.push_back(newBonus);
	  return OK;
	}
      if (parser->matchKeyword("UNIT"))
	{
    UnitEntity * newUnit = units[parser->getWord()];
    if(newUnit)
      units_.push_back(newUnit);
    return OK;
	}
      if (parser->matchKeyword("CLIMATE"))
	{
	  climate_ = (parser->getInteger());
	  return OK;
	}
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
       if (parser->matchKeyword("ORDER"))
 	{
	  //          cout << "Reading order..." << endl;
	  orders_.push_back(new Order(parser->getText(), this));
 	}

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
		UINT i;
		MovementVariety * mode;
		MovementMode <int> travelTimes;
		for(i=0;i<movementModes.size();i++)
		 {
			if(parser->matchInteger())
					days = parser->getInteger();
			else
					break;

			if(parser->matchInteger())    // old Overlord compartibility
      		mode = movementModes[parser->getInteger()];
			else
					mode =  movementModes[parser->getWord()];

			if (mode == 0)
					break;
				travelTimes[mode] = parser->getInteger();
			}
				

		exits_.push_back ( new ExplicitExit(dir,dest,travelTimes));
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
		UINT i;
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
       	GameData * temp =  GameData::prototypeManager->findInRegistry(keyword);
			if(temp == 0)
				{
					cout << "Unknown market type " << keyword  << " for location " << printName()<< endl;
				}
			else
				{
  				market_ = dynamic_cast<MarketStrategy *>(temp ->createInstanceOfSelf ());
          market_->setLocation(this);
        }

      return OK;
    }
   if(market_) market_->initialize(parser);
	  return OK;

}



void
LocationEntity::save(ostream &out)
{
  out << endl;
  out << keyword_ << " " <<tag_ << endl;
  if(!name_.empty()) out << "NAME " <<name_ << endl;
  if(!description_.empty()) out << "DESCRIPTION " <<description_  << endl;
 // out << endl;
  if(optima_) out << "OPTIMA " << optima_<<endl;
  if(population_) out << "POPULATION " << population_<<endl;
  if(wages_) out << "WAGES " << wages_<<endl;
  if(title_) out << "TITLE " << title_;
  if(titleOwner_) out << titleOwner_->printTag();


   market_->save(out);
   
  ResourceElementIterator resourceIter;
  for ( resourceIter = resources_.begin(); resourceIter != resources_.end(); resourceIter++)
    {
          out << "RESOURCES ";
           (*resourceIter)->save(out);
    }
  for (BonusIterator bonusIter = skillBonuses_.begin(); bonusIter != skillBonuses_.end(); bonusIter++)
    {
        out << "SKILL " ;
        (*bonusIter)->save(out);
    }
  for (UnitIterator unitIter  = units_.begin(); unitIter != units_.end(); unitIter++)
    {
        out << "UNIT " << (*unitIter)->getTag() << endl;
    }
  if(climate_) out << "CLIMATE " << climate_<<endl;
  if(economy_) out << "ECONOMY " << economy_<<endl;
       out << "XY " << x_ << " "<< y_<<" "<<endl;

  vector<Order *>::iterator orderIter;
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



void LocationEntity::print() // For debugging
{
    cout  << getName();
    cout << " [" << getTag()  << "] "<< endl;

  vector<Order *>::iterator iter;
  for ( iter = orders_.begin(); iter != orders_.end(); iter++)
    {
           (*iter)->save(cout);
    }

}



/** No descriptions */
void LocationEntity::setTerrain(const string &tag)
{
	 terrain_ = terrains[tag];
	if( terrain_ == 0)
	{
		cout << "Wrong terrain tag ["<<tag<<"] in locations" <<endl;
		exit(1);
	}
}



/** Adds Unit to location */
void LocationEntity::addUnit(UnitEntity * unit)
{
	// may demand additional actions (update of location and unit's data)
		if(unit == 0)
			return;
	   units_.push_back(unit);
		unit->setLocation(this);
}



/** Removes Unit from location */
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



/** prints  report for Entity (stats, posessions, private events) */
void LocationEntity::report(FactionEntity * faction, ostream & out)
{
  bool isFirst = true;
  out << printName() << " " <<terrain_->getName();
  // out << " lands of "<<owner;
  out << endl;
      faction->addKnowledge(terrain_);
  // Climate out << "This turn was  next turn should be "
  // Location Description
  // Location Title:
  if(population_)
    {
      out<< "Population: " << population_;
      if(race_)
      {
        out<< race_->printName();
      faction->addKnowledge(race_);
      }
      out << ". Wages: $" << wages_ << ", entertainment: $"
          << entertainment_ << ", tax income: $" <<taxes_<<".\n";

      if(market_) market_->report(faction,out);
    }
  if(!resources_.empty())
  {
    out<< "Resources: ";
    for(ResourceIterator iter = resources_.begin(); iter != resources_.end(); ++ iter)
    {
      if( isFirst)
      {
        isFirst = false;
      }
      else
        {
          out << ", ";
        }
      (*iter)->report(out);
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
      (*iter2)->report(out);
      faction->addSkillKnowledge((*iter2)->getSkill(), (*iter2)->getLevel());
    }
    out << ".\n";
 }

//  out<< "Skills bonuces: ";
//  isFirst = true;
//  for(BonusIterator iter2 = skillBonuses_.begin(); iter2 != skillBonuses_.end(); ++ iter2)
//  {
//    if( isFirst)
//      {
//        isFirst = false;
//      }
//      else
//        {
//          out << ", ";
//        }
//      (*iter2)->report(out);
//      faction->addSkillKnowledge((*iter2)->getSkill());
//    }
//    out << ".\n";



  out << "Directions: \n";

  for(ExitIterator iter= exits_.begin(); iter != exits_.end(); iter++)
  {
    (*iter)->report(out); 
  }


  int localObservation  = getFactionalObservation(faction);
 // if faction has unit - units 
  for(UnitIterator iter= units_.begin(); iter != units_.end(); iter++)
  {
    // special case for own unit
    (*iter)->publicReport(localObservation, out);
  }
// Buildings
}

int LocationEntity::getFactionalObservation(FactionEntity * faction)
{
 int maxLocalObservation = 0;
 int currentObservation;
 
  for(UnitIterator iter= units_.begin(); iter != units_.end(); iter++)
  {
    if ((*iter)->getFaction() == faction)
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
  for(BonusIterator iter = skillBonuses_.begin(); iter != skillBonuses_.end(); iter++)
    {
        if ((*iter)->getSkill() == skill)
          return (*iter)->getBonusPoints();
    }
  return 0;
}



BasicExit *  LocationEntity::findExit(LocationEntity * dest)
{
  for(ExitIterator iter= exits_.begin(); iter != exits_.end(); ++iter)
  {
    if( (*iter)->getDestination() == dest)
      return (*iter);
    }
    return 0;
}



BasicExit *  LocationEntity::findExit(DirectionVariety * dir)
{
  for(ExitIterator iter= exits_.begin(); iter != exits_.end(); ++iter)
  {
    if( (*iter)->getDirection() == dir)
      return (*iter);
    }
    return 0;
  
}



void LocationEntity::setResource(ItemElement)
{
  
}



void LocationEntity::setResource(ItemRule * item, int num)
{

}



int  LocationEntity::getResource(ItemRule * item) 
{
//        cout << "Looking for " <<item->printName()<< " at " <<printName()<<endl;
  ResourceElementIterator iter;
  for(iter = resources_.begin(); iter != resources_.end();iter++)
  {
    if ( (*iter)->getResource() == item)
      {
//        cout << "   ...have " <<(*iter)->getResource()->printName()<<endl;
//        cout << (*iter)->getResourceAmount() << " of " <<item->printName()<< " at " <<printName()<<endl;
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



Rational  LocationEntity::getAvailableResource(ItemRule * item)
{
//        cout << "Looking for available " <<item->printName()<< " at " <<printName()<<endl;
  ResourceElementIterator iter;
  for(iter = resources_.begin(); iter != resources_.end();iter++)
  {
  if ( (*iter)->getResource() == item)
      {
////        cout << "   ...have " <<(*iter)->getResource()->printName()<<endl;
//        ((*iter)->getAvailableResource()).save(cout); cout << " of " <<item->printName()<< " at " <<printName()<<endl;
////        cout << (*iter)->getAvailableResource() << " of " <<item->printName()<< " at " <<printName()<<endl;
        return (*iter)->getAvailableResource();
      }
  }
//  ResourceElement  newResource (item, 1);
// 	ResourceElementIterator iter = find(resources_.begin(), resources_.end(), &newResource);
//	if(iter != resources_.end())
//		return (*iter)->getAvailableResource();
//	else
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
}
void LocationEntity::processDailyConflict()
{
//  cout << "Processing daily conflict for "<< printName()<< " \n";
  dailyConflict_->process();
}



void LocationEntity::addMarketRequest(MarketRequest * request)
{
  market_->addRequest(request);
}



void LocationEntity::processMarket()
{
//  cout << "<><><><><><><><><><>   Processing Market conflict at " << printName()<<endl;
  market_->process();
}


void LocationEntity::harvestResource(ItemRule * item, Rational& num)
{
  ResourceElementIterator iter;
  for(iter = resources_.begin(); iter != resources_.end();iter++)
  {
  if ( (*iter)->getResource() == item)
      {
        Rational old((*iter)->getAvailableResource());
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
   cout << "Error: resource "<<item->printName() << " was not found at "<< printName()<<endl;
   }    
}



STATUS LocationEntity::prepareData()
{
//  /*if(market_) */market_->setLocation(this);
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

