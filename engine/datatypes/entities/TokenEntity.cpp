/***************************************************************************
                          TokenEntity.cpp
                             -------------------
    begin                : Fri Jul 25 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include <algorithm>
#include "TokenEntity.h"
#include "TeachingOffer.h"
#include "UnitEntity.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
#include "SimpleMessage.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "IntegerData.h"
#include "TerrainRule.h"
#include "SkillUseElement.h"
#include "TravelElement.h"
#include "OrderPrototype.h"
#include "StanceVariety.h"
#include "BasicOrderSynchronizationRequest.h"
#include "ObservationCondition.h"
#include "DataManipulator.h"
#include "TargetOrder.h"

extern DataManipulator * dataManipulatorPtr;
extern ReportPattern * newLevelReporter;
extern ReportPattern * maxLevelReporter;
extern ReportPattern *	cannotOathYourselfReporter;
extern ReportPattern *	oathRejectedReporter;
extern ReportPattern *	oathReporter;
extern ReportPattern * enterPrivateReporter;
extern ReportPattern * enterPublicReporter;
extern ReportPattern * leavePrivateReporter;
extern ReportPattern * leavePublicReporter;




TokenEntity::TokenEntity(const TokenEntity * prototype): Entity(prototype)
{
      location_  = 0;
    	faction_ = 0;
      toOath_ = 0;
			traced_ = false;
      moving_ = 0;
      guarding_ = false;
      passenger_ = false;
      advertising_ = false;
      announcing_ = false;
      sharing_ = true;
      withdrawingSupport_ = true;
      target_ = 0;
}



bool      TokenEntity::defaultAction()
{
  if(isUnaccessible())
    return false;

  if(isFullDayOrderFlagSet())
    return false;
  else
    return false;
}



STATUS TokenEntity::initialize        ( Parser *parser )
{

 if (parser->matchKeyword("FACTION"))
	{
	  faction_ = factions[ parser->getWord()];

	  return OK;
	}

 if (parser->matchKeyword("TRACED"))
	{
	  traced_ = true;
		return OK;
	}

 if (parser->matchKeyword("TARGET"))
    {
      string tag = parser->getWord();
      if (tag.size() != 0)
        {
          target_ = TargetOrder::findTarget(tag);
        }
      return OK;
    }

    if (parser->matchKeyword("SKILL"))
	{
    SkillElement * skill = SkillElement::readElement(parser);
    if(skill != 0)
        this->addSkill(*skill);
		return OK;
	}

 if (parser->matchKeyword("ITEM"))
 	{
    InventoryElement * newInventory = InventoryElement::readElement(parser);
		if(newInventory != 0)
		  {
	  	  inventory_.push_back(newInventory);
		  }
 	}

 if (parser->matchKeyword("MOVING"))
	{
    moving_ = TravelElement::readElement(parser);
		return OK;
	}

 if (parser->matchKeyword("SKILL_USE"))
	  {
        SkillUseElement * newSkillUse = SkillUseElement::readElement(parser);
        if(newSkillUse)
          skillUse_.push_back(newSkillUse);
	    return OK;
	  }

 if (parser->matchKeyword("SILENT"))
	{
    silent_ = true;
		return OK;
	}

 if (parser->matchKeyword("NO_SHARING"))
	{
    sharing_ = false;
		return OK;
	}
 if (parser->matchKeyword("ANNOUNCE"))
	{
    announcing_ = true;
		return OK;
	}
 if (parser->matchKeyword("ADVERTISE"))
	{
    advertising_ = true;
		return OK;
	}
 if (parser->matchKeyword("NO_SUPPORT"))
	{
    withdrawingSupport_ = false;
		return OK;
	}

  return Entity::initialize(parser);

}



void TokenEntity::save(ostream &out)
{
  out << endl;
  Entity::save(out);
//  out << keyword_ << " " <<tag_ << endl;
//  if(!name_.empty()) out << "NAME " <<name_ << endl;
//  if(!description_.empty()) out << "DESCRIPTION " <<description_  << endl;
	if(faction_) out << "FACTION " << faction_->getTag()<<endl;
  if(silent_) out << "SILENT"<<endl;
  if(announcing_) out << "ANNOUNCE"<<endl;
  if(advertising_) out << "ADVERTISE"<<endl;
  if(!withdrawingSupport_) out << "NO_SUPPORT"<<endl;
  if(!sharing_) out << "NO_SHARING"<<endl;

//  for (vector<OrderLine *>::iterator  iter = orders_.begin(); iter != orders_.end(); iter++)
//    {
//           (*iter)->save(out);
//    }

  for (InventoryElementIterator iter = inventory_.begin(); iter != inventory_.end(); iter++)
    {
           out << "ITEM ";
										(*iter)->save(out);
    }

  for (SkillIterator iter = skills_.begin(); iter != skills_.end(); iter++)
    {
           out << "SKILL ";
										(*iter).save(out);
    }

  for (SkillUseIterator iter= skillUse_.begin(); iter != skillUse_.end(); iter++)
    {
           out << "SKILL_USE ";
										(*iter)->save(out);
    }

  if (traced_) out << "TRACED" << endl;
  if(target_)
  {
    out << "TARGET "; target_->saveAsParameter(out); out<< endl;
  }
}



void TokenEntity::dailyUpdate()
{
   moveAdvance();
//   cout << "... Updating TokenEntity " << print()<<endl;
   TerrainRule * terrain = getLocation()->getTerrain();
   if(terrain == terrains.findByTag("lake",false) || terrain == terrains.findByTag("ocea",false))
   {
     if (getCapacity(3) < getWeight())   // provide function  getCapacity(MovementMode *)
//     if (getStackCapacity(3) < getStackWeight())
      {
       cout << "SOS! " << print()<< " is drowning\n";
       // unstack all
      }
   }
// if terrain is ocean or lake check swimming capacity
// ifnot enough - drowning
// smart drowning
// - equip items that may rise capacity
// sortout  items that have positive capacity/weight ratio
// sort all items according to weight/price ratio
// throw away items
}



// Reporting ==============================================
/** Retern reference to Entity which keeps reports from this  */
Entity * TokenEntity::getReportDestination()
{
	return location_;
}



void TokenEntity::privateReport(ReportPrinter &out)
{
}



void  TokenEntity::printOrderTemplate(ReportPrinter &out)
{
 out.setPrefix("#");
 privateReport(out);
 out.setPrefix(0);
 out << getKeyword()<< " "<< getTag()<<endl;
  for (vector<OrderLine *>::iterator iter = orders_.begin(); iter != orders_.end(); iter++)
    {
           (*iter)->printOrderLine(out);
    }
 out << endl;
}



/*
 * Report token's specific states
 */
void    TokenEntity::reportFlags(ReportPrinter &out)
{
//  Entity::reportFlags(out);
  if(guarding_) out << " Guarding.";
  if(passenger_) out << " Passenger.";
  if(advertising_) out << " Advertising.";
  if(announcing_) out << " Announcing.";
  if(!withdrawingSupport_) out << " Not withdrawing support.";
  if(!sharing_) out << " Not sharing.";
}



void TokenEntity::reportSkills(FactionEntity * faction, ReportPrinter &out)
{
  bool isFirst = true;
  if(skills_.empty())
         return;
  SkillIterator iter;
  out << "Skills: ";
  isFirst = true;

  for (iter = skills_.begin(); iter != skills_.end(); iter++)
		{
      if( iter != skills_.begin())
        {
          out << ", ";
        }
       out << (*iter);
    }
  out <<". ";

//  out << "May learn: ";

  RulesIterator skillIter;
  isFirst = true;
 	for (skillIter = skills.begin(); skillIter != skills.end(); skillIter++)
		{
       SkillRule * skill = dynamic_cast<SkillRule*>(*skillIter);
       if(skill == 0)
        continue;
       if((skill->getRequirement(0) == 0) && (getSkillLevel(skill) == 0))
       continue;

        if( mayStudySkill(skill))
            {
              faction->addSkillKnowledge(skill, getSkillLevel(skill) + 1); // It may be better placed
                                                                      // in addNewSkill
              if( isFirst)
                {
                  isFirst = false;
                  out << "May learn: ";
                }
               else
                {
                  out << ", ";
                }
                 skill->printLevel(getSkillLevel(skill) + 1, out);
              }

     }

    if( !isFirst)
      out << ". ";
}





// Data access methods ==============================================


/*
 * TokenEntity may not be able to execute orders when it is moving
 * (not as passenger)
 */
bool TokenEntity::isBusy()  const
{
  if(moving_ && ! passenger_)
      return true;
  return false;
}



/*
 * TokenEntity may be unaccessible for orders when it is
 * in process of destruction
 */
bool TokenEntity::isUnaccessible() const
{
  if(location_ == 0)
      return true;
  else
    return false;
}



/** Location where Entity is actually available (if any)*/
LocationEntity * TokenEntity::getGlobalLocation() const
{
    if(moving_)
    return 0;
  else
    return location_;
}




// Inventory methods ==============================================
InventoryElement * TokenEntity::findInInventory(ItemRule * item)
{
  InventoryElementIterator iter;
  for( iter = inventory_.begin(); iter != inventory_.end(); ++iter)
  {
    if( (*iter)->getItemType() == item)
       return *iter;
  }
  return 0;
}



vector < InventoryElement *> & TokenEntity::getAllInventory()
{
  return inventory_;
}



void TokenEntity::giveAllInventory(TokenEntity * unit)
{
  InventoryElementIterator iter;
  for (iter = inventory_.begin(); iter != inventory_.end(); ++iter)
  {
    unit->addToInventory((*iter)->getItemType(), (*iter)->getItemNumber() );
    delete (*iter);
  }
  inventory_.clear();
}



///*
// * Takes up to num items from inventory. Returns number of items taken
// */
//int TokenEntity::takeFromInventory(ItemRule * item, int num)
//{
//  InventoryElement * itemFound = findInInventory(item);
//  if(!itemFound)
//				return 0;
//
//  int numItems = itemFound->getItemNumber();
//  int numToGive = num;
//
//  if(numItems > numToGive)
//		{
//							numItems -= numToGive;
//							itemFound->setItemNumber(numItems);
//              if(itemFound->getEquipedNumber() > numItems )
//                  {
//									  itemFound->setEquipedNumber(numItems);
//							//		        item->applyEquipementEffects(this,numItems);
//                    recalculateStats();
//                  }
//							return numToGive;
//		}
//  else
//		{
//							int taken = numItems;
//              int wasEquiped = itemFound->getEquipedNumber();
//              deleteFromInventory(itemFound);
//              if(wasEquiped >0)
//              {
//							//		        item->applyEquipementEffects(this,0);
//              recalculateStats();
//              }
//							return taken;
//		}
//}


/*
 * Takes exactly num items from inventory if they are available
 * and nothing if number of items in posession is less than required.
 * Returns number of items TokenEntity actually has.
 */

int TokenEntity::takeFromInventory(ItemRule * item, int num)
{
  InventoryElement * itemFound = findInInventory(item);
  if(!itemFound)
				return 0;

  int numItems = itemFound->getItemNumber();
  int numToGive = num;

  if(numItems > numToGive)
		{
							numItems -= numToGive;
							itemFound->setItemNumber(numItems);
              if(itemFound->getEquipedNumber() > numItems )
                  {
									  itemFound->setEquipedNumber(numItems);
//									        item->applyEquipementEffects(this,numItems);
                    recalculateStats();
                  }
							return num;
		}
  else if (numItems == numToGive)
    {
              int wasEquiped = itemFound->getEquipedNumber();
              deleteFromInventory(itemFound);
              if(wasEquiped >0)
                {
//									        item->applyEquipementEffects(this,0);
                  recalculateStats();
                }
							return num;
    }
      else
				{
				  return numItems;
				}


}



void TokenEntity::deleteFromInventory(InventoryElement * element)
{
  InventoryElementIterator iter = find(inventory_.begin(),inventory_.end(),element);
  if(iter != inventory_.end())
    {
      delete element;
		  inventory_.erase(iter);
    }
}

/*
 * After  checks for equipability and skills do equip item
 */
int TokenEntity::equipItem(ItemRule * item, int num)
{
  InventoryElement * itemFound = findInInventory(item);
  if(!itemFound)
				return 0;

  int currentlyEquipedItems = itemFound->getEquipedNumber(); 																				;
	if (num <= currentlyEquipedItems) // unequip
			{
				itemFound->setEquipedNumber(num);
				return ( num - currentlyEquipedItems);
			}
// Item may demand skills for being equiped.
  BasicCondition * equipCondition = item->demandsEquipCondition();
  if(equipCondition )
  {
    if(!equipCondition->isSatisfied(this))
      return 0;
    }
// Can't equip more than have
	if (num > itemFound->getItemNumber())
			num = itemFound->getItemNumber();


	itemFound->setEquipedNumber(num);
							//		        item->applyEquipementEffects(this,num)
	return (num - currentlyEquipedItems);
}



/*
 * Add I tem to inventory if it was not there
 * Increase it's amount otherwise
 */
void TokenEntity::addToInventory(ItemRule * item, RationalNumber& num)  // should be done better
{
//cout << print()<< " adding " <<item->print() <<endl;
  InventoryElement * itemFound = findInInventory(item);
  if(itemFound)
      {
        RationalNumber old(itemFound->getRationalItemNumber());
        itemFound->setRationalItemNumber(old + num);
          return;
      }
  inventory_.push_back(new InventoryElement(item,num));
}



/** No descriptions */
int TokenEntity::hasItem(ItemRule * item)
{
  InventoryElement * itemFound = findInInventory(item);
  if(itemFound)
		return itemFound->getItemNumber();
	else
     return 0;
}



int TokenEntity::hasEquiped(ItemRule * item)
{
  InventoryElement * itemFound = findInInventory(item);
  if(itemFound)
		return itemFound ->getEquipedNumber();
	else
     return 0;
}

bool TokenEntity::isEquiped(InventoryElement * item)
{
  InventoryElement * itemFound = findInInventory(item->getItemType());
  if(itemFound)
		return (itemFound->getEquipedNumber() >= item->getEquipedNumber());
	else
     return false;
}



RationalNumber     TokenEntity::getItemAmount(ItemRule * item)
{
  InventoryElement * itemFound = findInInventory(item);
  if(itemFound)
		return itemFound->getRationalItemNumber();
	else
     return 0;
}



void TokenEntity::addToInventory(ItemRule * item, int num)  // should be done better
{
  RationalNumber number(num);
  addToInventory(item, number);
}



// Stacking/Containment ========================================================
bool TokenEntity::isAccepting(UnitEntity * unit)
{
  return (find(accepting_.begin(),accepting_.end(),unit) != accepting_.end());
}



void TokenEntity::clearAccept(UnitEntity * unit)
{
 vector<UnitEntity * >::iterator iter = find(accepting_.begin(),accepting_.end(),unit);
  if(iter != accepting_.end())
      accepting_.erase(iter);
}



void TokenEntity::accept(UnitEntity * unit)
{
 vector<UnitEntity * >::iterator iter = find(accepting_.begin(),accepting_.end(),unit);
  if(iter == accepting_.end())
  accepting_.push_back(unit);
}



// Skills ========================================================
vector < SkillElement>& TokenEntity::getAllSkills()
{
  return skills_;
}




/*
 * returns true if current unit may further study this skill
 * // may be it should return level, that unit may study or 0 ?
 */
bool TokenEntity::mayStudySkill(SkillRule * skill)
{
  if( skill->mayBeStudied(this) == LEARNING_OK)
        return true;
  else
      return false;
}



int TokenEntity::addSkill(SkillElement  skill)
{
  return addSkill(skill.getSkill(), skill.getExpPoints());
}



int TokenEntity::getSkillLevel(SkillRule  * const skill)
{
	SkillIterator iter;
	for (iter = skills_.begin(); iter != skills_.end(); iter++)
		{
			if (skill == (*iter).getSkill())
				{
          return skill->getLevel((*iter).getExpPoints() );
          }
		}
  return 0;

}



/** Adding skill experience, checking for new level reached */
int TokenEntity::addSkill(SkillRule  * skill, int expPoints)
{
	SkillIterator iter;
   int oldLevel = 0;
   int newLevel = 0;

  for ( iter = skills_.begin(); iter != skills_.end(); iter++)
		{
					if( (*iter).getSkill() == skill)
								{
                   oldLevel = skill->getLevel((*iter).getExpPoints());
									 (*iter).setExpPoints((*iter).getExpPoints() + expPoints);
                   newLevel = skill->getLevel((*iter).getExpPoints());
//    cout <<skill->printTag()<< " Exp " << (*iter).getExpPoints()<<endl;
								  break;
								}
		}
    if (iter == skills_.end())
	      skills_.push_back(SkillElement(skill,expPoints));
//    cout <<skill->printTag()<< " Old level " << oldLevel <<" new " << newLevel<<endl;

   if (oldLevel != newLevel)    // Check for a new level being reached
     gainNewLevel(skill,newLevel);

     return newLevel;
}

//  TokenEntity::newLevelReporter_ = new ReportPattern(""," reached level "," in ","");



void TokenEntity::gainNewLevel(SkillRule * skill, int newLevel)
{
	    TertiaryMessage * Message = new TertiaryMessage(newLevelReporter, this, new IntegerData(newLevel), skill);
		  addReport(Message, 0,0);

   // Add knowledge to faction
     getFaction()->addSkillKnowledge(skill, newLevel);
   // Recalculate stats
       recalculateStats();
   // Is it maximum level?
       if  ( newLevel >= skill->getMaxLevel() )
        {
	    BinaryMessage * Message = new BinaryMessage(maxLevelReporter, this, skill);
		  addReport(Message, 0,0 );
         }
}



int TokenEntity::getSkillPoints(SkillRule  * const skill)
{
	SkillIterator iter;
	for (iter = skills_.begin(); iter != skills_.end(); iter++)
		{
			if (skill == (*iter).getSkill())
				{
          return (*iter).getExpPoints();
          }
		}
  return 0;

}



SkillElement *  TokenEntity::getSkillElement(SkillRule  * const skill)
{
  SkillIterator iter;
	for (iter = skills_.begin(); iter != skills_.end(); iter++)
		{
			if (skill == (*iter).getSkill())
				{
          return &(*iter);
        }
		}
  return 0;
}


void TokenEntity::cancelTeachingOffer()
{
  vector <TeachingOffer  *>::iterator iter;
  for(iter = teachingOffers_.begin(); iter != teachingOffers_.end(); ++iter)
  {
    if((*iter)->getTeacher() == this)
      {
//         cout<< *this<<" deletes " << *(*iter) <<endl;
         delete (*iter);
         teachingOffers_.erase(iter);
         return;
      }
    }
  cout << "ERROR. on cancelling "<< print() <<" Can't find his own teachingOffers\n";

}



/*
 * Checks that Entity has specified experience in given skill
 */
bool TokenEntity::hasSkill(SkillRule  * skill, int experience)
{
  if(( skill == 0) || ( experience == 0) )
    return true;
  SkillIterator iter;
	for (iter = skills_.begin(); iter != skills_.end(); iter++)
		{
			if ((skill == (*iter).getSkill()) && (experience <= (*iter).getExpPoints()))
				return true;
		}

	return false;

}


int TokenEntity::getLearningCapacity()
{
  return 1;
}



bool TokenEntity::teacherRequired(SkillRule * skill)
{
  return true;
}

// Skill Use ========================================================
// finds amount of skill use that remained from the previous use,
// adds current skill use and
// returns number of production cycle completed
// and truncates SkillUseElement
int TokenEntity::addSkillUse(SkillUseElement * skillUse)
{
  int numCycles=0;
//  SkillUseIterator iter = find(skillUse_.begin(), skillUse_.end(), skillUse);
  SkillUseIterator iter;
  for( iter = skillUse_.begin(); iter != skillUse_.end();++iter)
  {
    if((*iter)->getSkillUsed() == skillUse->getSkillUsed())
      break;
  }
  if(iter == skillUse_.end()) // not used before
    {
      if(skillUse->getDaysUsed() < skillUse->getDuration())
      {
        skillUse_.push_back(skillUse);
        return 0;
      }
      numCycles = (skillUse->getDaysUsed() / skillUse->getDuration()).roundDown();
      skillUse->setDaysUsed(skillUse->getDaysUsed() - numCycles * skillUse->getDuration());
      return  numCycles;
    }

  RationalNumber effectiveDays;
  if ( (*iter)->getDuration() == skillUse->getDuration() )
    {
      effectiveDays = (*iter)->getDaysUsed() + skillUse->getDaysUsed();
    }
  else  // skill is used with a different duration now
     {
        effectiveDays = ( (*iter)->getDaysUsed() * skillUse->getDuration()) / (*iter)->getDuration() +  skillUse->getDaysUsed();
        (*iter)->setDuration(skillUse->getDuration());
     }

  if(effectiveDays < (*iter)->getDuration())   // skill use not completed
  {
    (*iter)->setDaysUsed(effectiveDays);
     return 0;
  }
  else  // skill use completed (and may be several times)
    {
      numCycles = (effectiveDays / (*iter)->getDuration()).roundDown();
      skillUse->setDaysUsed(effectiveDays - numCycles * (*iter)->getDuration());
      delete (*iter);
      skillUse_.erase (iter); // Unused days will not be automatically used on next working cycle
      return numCycles;
    }
}



// finds amount of skill use that remained from the previous use,
// adds current skill use and
// returns number of production cycle completed
// no truncation
int TokenEntity::addCumullativeSkillUse(SkillUseElement * skillUse, int accumulationLimit )
{
  int numCycles=0;
  if  (accumulationLimit < 1)
    accumulationLimit = 1;
//  SkillUseIterator iter = find(skillUse_.begin(), skillUse_.end(), skillUse);
  SkillUseIterator iter;
  for( iter = skillUse_.begin(); iter != skillUse_.end();++iter)
  {
    if((*iter)->getSkillUsed() == skillUse->getSkillUsed())
      break;
  }
  if(iter == skillUse_.end()) // not used before
    {
      numCycles = (skillUse->getDaysUsed() / skillUse->getDuration()).roundDown();
      if (numCycles >= accumulationLimit) // all done. nothing to store
        return  accumulationLimit;

      else
      {
        skillUse_.push_back(skillUse);
        return numCycles;
      }
    }

  RationalNumber effectiveDays;
  if ( (*iter)->getDuration() == skillUse->getDuration() )
    {
      effectiveDays = (*iter)->getDaysUsed() + skillUse->getDaysUsed();
    }
  else  // skill is used with a different duration now
     {
        effectiveDays = ( (*iter)->getDaysUsed() * skillUse->getDuration()) / (*iter)->getDuration() +  skillUse->getDaysUsed();
        (*iter)->setDuration(skillUse->getDuration());
     }

  numCycles = (effectiveDays / (*iter)->getDuration()).roundDown();

  if (numCycles < accumulationLimit) // skill use not completed
        return  numCycles;


  else  // skill use completed
    {
//      skillUse->setDaysUsed(effectiveDays - numCycles * (*iter)->getDuration());
      delete (*iter);
      skillUse_.erase (iter); // Unused days will not be  used on next working cycle
      return accumulationLimit;
    }
}




bool TokenEntity::isCurrentlyUsingSkill(SkillRule * skill)
{
  	for (SkillUseIterator iter = skillUse_.begin(); iter != skillUse_.end(); ++iter)
   {
     if( (*iter)->getSkillUsed() == skill)
      return true;
   }
   return false;
}




/*
 *  Intristic ability of Entity to study  skill
 *  Level limitations also taken into considerations
 */
LEARNING_RESULT TokenEntity::mayLearn(SkillRule * skill)
{
  return CANNOT_STUDY_FAILURE;
}

// Movement ========================================================



void TokenEntity::setEntityMoving(TravelElement * moving)
{
  moving_ = moving;
}



bool TokenEntity::moveAdvance()
{
   if(moving_ == 0)
      return false;

   moving_->advance();


   if(moving_->isCrossingBorder())
   {
     // check if stack or vessel can enter location (it may be guarded or other reasons)
     moveGroupToLocation();
   }


   if(moving_->isArrived())
   {
     movingGroupArrived();
     delete moving_;
     moving_ = 0;
     return true;
   }
     return false;

}


void TokenEntity::moveToLocation()
{
  getLocation()->addReport(new UnaryMessage(leavePublicReporter, this), 0 ,
                            ObservationCondition::createObservationCondition(getStealth() ));
  addReport(new UnaryMessage(leavePrivateReporter, getLocation()));
  LocationEntity * newLocation = moving_->getDestination();
  getFaction()->addVisitedLocation(newLocation);

	if (isTraced())
    cout <<"== TRACING " <<print()<< " ==> Enters "<< newLocation->print()<<"\n";
  newLocation->addReport(new UnaryMessage(enterPublicReporter, this), 0 ,
                            ObservationCondition::createObservationCondition(getStealth() ));
  addReport(new UnaryMessage(enterPrivateReporter, newLocation));
}


void TokenEntity::moveGroupToLocation()
{
}



void TokenEntity::movingGroupArrived()
{
}



bool TokenEntity::retreat()
{
  if(moving_)
  {
    moving_->retreat();
    return true;
  }
  else
    return false;
}



// Other ========================================================
/** Unit is not exist anymore (dead or disbanded) */
bool TokenEntity::isDisbanded()
{
  return (location_ == 0);
}



void TokenEntity::recalculateStats()
{
}



bool TokenEntity::mayMove()
{
  return false;
}



bool TokenEntity::leaveStaying()
{
  return false;
}



int  TokenEntity::calculateTotalWeight (int & weight)
{
  return  weight;
}



void TokenEntity::calculateTotalCapacity(int & capacity, int modeIndex)
{
}



/** Climate and overloading effects */
int TokenEntity::calculateTravelTime(int time, int weight, int capacity)
{
    int overloadFactor;
   if(capacity == 0)
          return 0;
   overloadFactor = weight  * 100 / capacity;

   if(overloadFactor >= 200) // overloaded twice
          return 0;

   if(time == 0)
          return 0;

//  apply climate
    if(overloadFactor > 100)
    time = time + time *  3 * (overloadFactor - 100)  / 100;   //  overloading
    return time;
}



bool TokenEntity::mayInterractTokenEntity(TokenEntity * tokenEntity)
{
   if( tokenEntity == 0)
       return false;

   if(tokenEntity->getGlobalLocation() != this->getGlobalLocation() )  // take into account buildings
        return false;
   if(tokenEntity->getFaction() == this->getFaction() ) // the same faction
        return true;
  if(tokenEntity->isExposed())
        return true;
   if (this->getObservation() >= tokenEntity->getStealth() )
        return true;
   if (this->getLocation()->getFactionalObservation(getFaction()) >= tokenEntity->getStealth() )
        return true;
   else
        return false;
}



bool TokenEntity::mayInterractFaction(FactionEntity * faction)
{
   if( faction == 0)
       return false;

   for(UnitIterator iter = getLocation()->unitsPresent().begin();
                    iter != getLocation()->unitsPresent().end(); ++iter)
       {
         if( (*iter)->getFaction() == faction)
          {
            if (mayInterractTokenEntity(*iter))
//            if (Entity::mayInterract(*iter))// <- check this
                  return true;
          }
       }

   return false;

}



ORDER_STATUS TokenEntity::oath(FactionEntity * faction)
{
  if(faction == 0)
  {
   return INVALID;
  }

  if(faction == getFaction())
  {
    addReport(new SimpleMessage(cannotOathYourselfReporter));
   return INVALID;
  }

 if(!faction->isNPCFaction()) // you may allways oath to NPC
 {
   if (!mayInterractFaction(faction)) // Not In the same place or can't see
	     return FAILURE;

 	  if(*(faction->getStance(getFaction())) < *friendlyStance)
      {
        // not accepting. Reports to both sides
      UnaryMessage * oathRejectedMessage = new UnaryMessage(oathRejectedReporter, faction);
      addReport(oathRejectedMessage,0,0);
      faction->addReport(oathRejectedMessage,0,0);
		  return INVALID;
      }
  }
  BinaryMessage * oathReportMessage = new BinaryMessage(oathReporter, this,faction);
  getFaction()->addReport(oathReportMessage,0,0 );
  markToOath(faction);
  faction->addReport(oathReportMessage,0,0);
	return SUCCESS;

}



BasicOrderSynchronizationRequest * TokenEntity::hasOrderSyncRequest(BasicOrderSynchronizationRequest * request)
{
 for(SyncRequestIterator iter = orderSyncRequests_.begin();
      iter != orderSyncRequests_.end(); ++iter)
      {
         if (request->isEqual(*iter))
         {
            return (*iter);
         }
      }
 return 0;
}



void TokenEntity::removeOrderSyncRequest(BasicOrderSynchronizationRequest * request)
{
 for(SyncRequestIterator iter = orderSyncRequests_.begin();
      iter != orderSyncRequests_.end(); ++iter)
      {
         if (request->isEqual(*iter))
         {
            orderSyncRequests_.erase(iter);
            delete (*iter);
            return;
         }
      }
}



void TokenEntity::addOrderSyncRequest(BasicOrderSynchronizationRequest * request)
{
  orderSyncRequests_.push_back(request);
}



bool TokenEntity::doneOrderSyncRequest(BasicOrderSynchronizationRequest * request)
{
 for(SyncRequestIterator iter = orderSyncRequests_.begin();
      iter != orderSyncRequests_.end(); ++iter)
      {
         if (request->isEqual(*iter))
         {
            return (*iter)->syncDone();
         }
      }
  return false;
}



void TokenEntity::markDoneOrderSyncRequest(BasicOrderSynchronizationRequest * request)
{
 for(SyncRequestIterator iter = orderSyncRequests_.begin();
      iter != orderSyncRequests_.end(); ++iter)
      {
         if (request->isEqual(*iter))
         {
            (*iter)->setSyncDone();
         }
      }
}


void TokenEntity::setTarget(AbstractData * target)
{
//  if(target_)
//    target_->clean();
  target_ = target;
}
