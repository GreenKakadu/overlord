/***************************************************************************
                          PhysicalEntity.cpp 
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
#include "PhysicalEntity.h"
#include "TeachingOffer.h"
#include "UnitEntity.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
#include "SimplePattern.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "TertiaryPattern.h"
#include "IntegerData.h"
#include "TerrainRule.h"
#include "SkillUseElement.h"
#include "TravelElement.h"
#include "OrderPrototype.h"
#include "StanceVariety.h"
#include "BasicOrderSynchronizationRequest.h"
#include "ObservationCondition.h"
extern Reporter * newLevelReporter;
extern Reporter * maxLevelReporter;
extern Reporter *	cannotOathYourselfReporter;
extern Reporter *	oathRejectedReporter;
extern Reporter *	oathReporter;
extern Reporter * enterPrivateReporter;
extern Reporter * enterPublicReporter;
extern Reporter * leavePrivateReporter;
extern Reporter * leavePublicReporter;




PhysicalEntity::PhysicalEntity(const PhysicalEntity * prototype): Entity(prototype)
{
    	faction_ = 0;
      toOath_ = 0;
			traced_ = false;
      moving_ = 0;
      guarding_ = false;
      passenger_ = false;
      advertising_ = false;
      announcing_ = false;
      withdrawingSupport_ = false;
}



bool      PhysicalEntity::defaultAction()
{
  if(isUnaccessible())
    return false;

  if(isFullDayOrderFlagSet())
    return false;
  else
    return false;
}



STATUS PhysicalEntity::initialize        ( Parser *parser )
{
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
	  target_ = parser->getWord();
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

 if (parser->matchKeyword("ORDER"))
 	{
	  //          cout << "Reading order..." << endl;
	  orders_.push_back(new Order(parser->getText(),this));
 	}

 if (parser->matchKeyword("SILENT"))
	{
    silent_ = true;
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
 if (parser->matchKeyword("SUPPORT"))
	{
    withdrawingSupport_ = true;
		return OK;
	}
		return OK;
}



void PhysicalEntity::save(ostream &out)
{
  out << endl;
  out << keyword_ << " " <<tag_ << endl;
  if(!name_.empty()) out << "NAME " <<name_ << endl;
  if(!description_.empty()) out << "DESCRIPTION " <<description_  << endl;
	if(faction_) out << "FACTION " << faction_->getTag()<<endl;
  if(silent_) out << "SILENT"<<endl;
  if(announcing_) out << "ANNOUNCE"<<endl;
  if(advertising_) out << "ADVERTISE"<<endl;
  if(withdrawingSupport_) out << "SUPPORT"<<endl;
  
  for (vector<Order *>::iterator  iter = orders_.begin(); iter != orders_.end(); iter++)
    {
           (*iter)->save(out);
    }

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
  if(!target_.empty()) out << "TARGET " <<target_<< endl;
}



void PhysicalEntity::dailyUpdate()
{
   moveAdvance();
//   cout << "... Updating PhysicalEntity " << printName()<<endl;
   TerrainRule * terrain = getLocation()->getTerrain();
   if(terrain == terrains.findByTag("lake",false) || terrain == terrains.findByTag("ocea",false))
   {
     if (getCapacity(3) < getWeight())   // provide function  getCapacity(MovementMode *)
//     if (getStackCapacity(3) < getStackWeight())
      {
       cout << "SOS! " << printName()<< " is drowning\n";
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
Entity * PhysicalEntity::getReportDestination()
{
	return location_;
}



void PhysicalEntity::privateReport(ReportPrinter &out)
{
}



void  PhysicalEntity::printOrderTemplate(ReportPrinter &out)
{
 out.setPrefix("#");
 privateReport(out);
 out.setPrefix(0);
 out << getKeyword()<< " "<< getTag()<<endl;
  for (vector<Order *>::iterator iter = orders_.begin(); iter != orders_.end(); iter++)
    {
           (*iter)->print(out);
    }
 out << endl;
}
         


void PhysicalEntity::reportSkills(FactionEntity * faction, ReportPrinter &out)
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
      (*iter).print(out);
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
 * PhysicalEntity may not be able to execute orders when it is moving 
 * (not as passenger)
 */
bool PhysicalEntity::isBusy()  const
{
  if(moving_ && ! passenger_)
      return true;
  return false;
}



/*
 * PhysicalEntity may be unaccessible for orders when it is
 * in process of destruction 
 */
bool PhysicalEntity::isUnaccessible() const
{
  if(location_ == 0)
      return true;
  else
    return false;
}



/** Location where Entity is actually available (if any)*/
LocationEntity * PhysicalEntity::getGlobalLocation() const
{
    if(moving_)
    return 0;
  else
    return location_;
}




// Inventory methods ==============================================
InventoryElement * PhysicalEntity::findInInventory(ItemRule * item)
{
  InventoryElementIterator iter;
  for( iter = inventory_.begin(); iter != inventory_.end(); ++iter)
  {
    if( (*iter)->getItemType() == item)
       return *iter;
  }
  return 0;
}



vector < InventoryElement *> & PhysicalEntity::getAllInventory()
{
  return inventory_;
}



void PhysicalEntity::giveAllInventory(PhysicalEntity * unit)
{
  InventoryElementIterator iter;
  for (iter = inventory_.begin(); iter != inventory_.end(); ++iter)
  {
    unit->addToInventory((*iter)->getItemType(), (*iter)->getItemNumber() );
    delete (*iter);
  }
  inventory_.clear();
}



/*
 * Takes up to num items from inventory. Returns number of items taken
 */
int PhysicalEntity::takeFromInventory(ItemRule * item, int num)
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
							//		        item->applyEquipementEffects(this,numItems);
                    recalculateStats();
                  }
							return numToGive;
		}
  else
		{
							int taken = numItems;
              int wasEquiped = itemFound->getEquipedNumber();
              deleteFromInventory(itemFound);
              if(wasEquiped >0)
              {
							//		        applyEquipementEffects()
              recalculateStats();
              }
							return taken;
		}
}


/*
 * Takes exactly num items from inventory.
 * Returns true if inventory had num items and false othervise
 */

bool PhysicalEntity::takeFromInventoryExactly(ItemRule * item, int num)
{
  InventoryElement * itemFound = findInInventory(item);
  if(!itemFound)
				return false;

  int numItems = itemFound->getItemNumber();
  int numToGive = num;

  if(numItems > numToGive)
		{
							numItems -= numToGive;
							itemFound->setItemNumber(numItems);
              if(itemFound->getEquipedNumber() > numItems )
                  {
									  itemFound->setEquipedNumber(numItems);
							//		        applyEquipementEffects()
                    recalculateStats();
                  }
							return true;
		}
  else if (numItems == numToGive)
    {
              int wasEquiped = itemFound->getEquipedNumber();
              deleteFromInventory(itemFound);
              if(wasEquiped >0)
                {
							//		        applyEquipementEffects()
                  recalculateStats();
                }
							return true;
    }
      else
				{
				  return false;
				}


}



void PhysicalEntity::deleteFromInventory(InventoryElement * element)
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
int PhysicalEntity::equipItem(ItemRule * item, int num)
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
							//		        applyEquipementEffects()
	return (num - currentlyEquipedItems);
}



/*
 * Add I tem to inventory if it was not there
 * Increase it's amount otherwise
 */
void PhysicalEntity::addToInventory(ItemRule * item, Rational& num)  // should be done better
{
//cout << printName()<< " adding " <<item->printName() <<endl;
  InventoryElement * itemFound = findInInventory(item);
  if(itemFound)
      {
        Rational old(itemFound->getRationalItemNumber());
        itemFound->setRationalItemNumber(old + num);
          return;
      }
  inventory_.push_back(new InventoryElement(item,num));
}



/** No descriptions */
int PhysicalEntity::hasItem(ItemRule * item)
{
  InventoryElement * itemFound = findInInventory(item);
  if(itemFound)
		return itemFound->getItemNumber();
	else
     return 0;
}



int PhysicalEntity::hasEquiped(ItemRule * item)
{
  InventoryElement * itemFound = findInInventory(item);
  if(itemFound)
		return itemFound ->getEquipedNumber();
	else
     return 0;
}

bool PhysicalEntity::isEquiped(InventoryElement * item)
{
  InventoryElement * itemFound = findInInventory(item->getItemType());
  if(itemFound)
		return (itemFound->getEquipedNumber() >= item->getEquipedNumber());
	else
     return false;
}



Rational     PhysicalEntity::getItemAmount(ItemRule * item)
{
  InventoryElement * itemFound = findInInventory(item);
  if(itemFound)
		return itemFound->getRationalItemNumber();
	else
     return 0;
}



void PhysicalEntity::addToInventory(ItemRule * item, int num)  // should be done better
{
  Rational number(num);
  addToInventory(item, number);
}



// Stacking/Containment ========================================================
bool PhysicalEntity::isAccepting(UnitEntity * unit)
{
  return (find(accepting_.begin(),accepting_.end(),unit) != accepting_.end());
}



void PhysicalEntity::clearAccept(UnitEntity * unit)
{
 vector<UnitEntity * >::iterator iter = find(accepting_.begin(),accepting_.end(),unit);
  if(iter != accepting_.end())
      accepting_.erase(iter);
}



void PhysicalEntity::accept(UnitEntity * unit)
{
 vector<UnitEntity * >::iterator iter = find(accepting_.begin(),accepting_.end(),unit);
  if(iter == accepting_.end())
  accepting_.push_back(unit);
}



// Skills ========================================================
vector < SkillElement>& PhysicalEntity::getAllSkills()
{
  return skills_;
}




/*
 * returns true if current unit may further study this skill
 * // may be it should return level, that unit may study or 0 ?
 */
bool PhysicalEntity::mayStudySkill(SkillRule * skill)
{
  if( skill->mayBeStudied(this) == LEARNING_OK)
        return true;
  else
      return false;
}



int PhysicalEntity::addSkill(SkillElement  skill)
{
  return addSkill(skill.getSkill(), skill.getExpPoints());
}



int PhysicalEntity::getSkillLevel(SkillRule  * const skill)
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
int PhysicalEntity::addSkill(SkillRule  * skill, int expPoints)
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

//  PhysicalEntity::newLevelReporter_ = new Reporter(""," reached level "," in ","");



void PhysicalEntity::gainNewLevel(SkillRule * skill, int newLevel)
{
	    TertiaryPattern * Message = new TertiaryPattern(newLevelReporter, this, new IntegerData(newLevel), skill);
	    ReportRecord * currentReport = new   ReportRecord(Message, 0);
		  addReport( currentReport);

   // Add knowledge to faction
     getFaction()->addSkillKnowledge(skill, newLevel);
   // Recalculate stats
       recalculateStats();
   // Is it maximum level?
       if  ( newLevel >= skill->getMaxLevel() )
        {
	    BinaryPattern * Message = new BinaryPattern(maxLevelReporter, this, skill);
	    ReportRecord * currentReport = new   ReportRecord(Message, 0);
		  addReport( currentReport);
         }
}



int PhysicalEntity::getSkillPoints(SkillRule  * const skill)
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



SkillElement *  PhysicalEntity::getSkillElement(SkillRule  * const skill)
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


void PhysicalEntity::cancelTeachingOffer()
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
  cout << "ERROR. on cancelling "<< printName() <<" Can't find his own teachingOffers\n";

}



/*
 * Checks that Entity has specified experience in given skill 
 */
bool PhysicalEntity::hasSkill(SkillRule  * skill, int experience)
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


int PhysicalEntity::getLearningCapacity()
{
  return 1;
}



bool PhysicalEntity::teacherRequired(SkillRule * skill)
{
  return true;
}

// Skill Use ========================================================
// returns number of production cycle completed
// and truncates SkillUseElement
int PhysicalEntity::addSkillUse(SkillUseElement * skillUse)
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

  Rational effectiveDays;
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
     return 0;
  else  // skill use completed (and may be several times)
    {
      numCycles = (effectiveDays / (*iter)->getDuration()).roundDown();
      skillUse->setDaysUsed(effectiveDays - numCycles * (*iter)->getDuration());
      delete (*iter);
      skillUse_.erase (iter); // Unused days will not be automatically used on next working cycle
      return numCycles;
    }
}

bool PhysicalEntity::isCurrentlyUsingSkill(SkillRule * skill)
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
LEARNING_RESULT PhysicalEntity::mayLearn(SkillRule * skill)
{
  return CANNOT_STUDY_FAILURE;
}

// Movement ========================================================



void PhysicalEntity::setEntityMoving(TravelElement * moving)
{
  moving_ = moving;
}



bool PhysicalEntity::moveAdvance()
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


void PhysicalEntity::moveToLocation()
{
  getLocation()->addReport(new UnaryPattern(leavePublicReporter, this), 0 ,
                            ObservationCondition::createObservationCondition(getStealth() ));
  addReport(new UnaryPattern(leavePrivateReporter, getLocation()));
  LocationEntity * newLocation = moving_->getDestination();
  getFaction()->addVisitedLocation(newLocation);
  
	if (isTraced())
    cout <<"== TRACING " <<printName()<< " ==> Enters "<< newLocation->printName()<<"\n";
  newLocation->addReport(new UnaryPattern(enterPublicReporter, this), 0 ,
                            ObservationCondition::createObservationCondition(getStealth() ));
  addReport(new UnaryPattern(enterPrivateReporter, newLocation));
}


void PhysicalEntity::moveGroupToLocation()
{
}



void PhysicalEntity::movingGroupArrived()
{
}



bool PhysicalEntity::retreat()
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
bool PhysicalEntity::isDisbanded()
{
  return (location_ == 0);
}



void PhysicalEntity::recalculateStats()
{
}



bool PhysicalEntity::mayMove()
{
  return false;
}



bool PhysicalEntity::leaveStaying()
{
  return false;
}



int  PhysicalEntity::calculateTotalWeight (int & weight)
{
  return  weight;
}



void PhysicalEntity::calculateTotalCapacity(int & capacity, int modeIndex)
{
}



/** Climate and overloading effects */
int PhysicalEntity::calculateTravelTime(int time, int weight, int capacity)
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



bool PhysicalEntity::mayInterractPhysicalEntity(PhysicalEntity * tokenEntity)
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
  


bool PhysicalEntity::mayInterractFaction(FactionEntity * faction)
{
   if( faction == 0)
       return false;

   for(UnitIterator iter = getLocation()->unitsPresent().begin();
                    iter != getLocation()->unitsPresent().end(); ++iter)
       {
         if( (*iter)->getFaction() == faction)
          {
            if (mayInterractPhysicalEntity(*iter))
//            if (Entity::mayInterract(*iter))// <- check this
                  return true;
          }
       }

   return false;

}



ORDER_STATUS PhysicalEntity::oath(FactionEntity * faction)
{
  if(faction == 0)
  {
   return INVALID;
  }
   
  if(faction == getFaction())
  {   
    addReport(new SimplePattern(cannotOathYourselfReporter)); 		
   return INVALID;
  }

 if(!faction->isNPCFaction()) // you may allways oath to NPC
 {
   if (!mayInterractFaction(faction)) // Not In the same place or can't see
	     return FAILURE;
 
 	  if(*(faction->getStance(getFaction())) < *friendlyStance)
      {
        // not accepting. Reports to both sides
      addReport(new   ReportRecord(new UnaryPattern(oathRejectedReporter, faction)) );
      faction->addReport(new   ReportRecord(new UnaryPattern(oathRejectedReporter, faction)) );
		  return INVALID;
      }
  }
  getFaction()->addReport(new   ReportRecord(new BinaryPattern(oathReporter, this,faction)) );
  markToOath(faction);
  addReport(new   ReportRecord(new BinaryPattern(oathReporter, this,faction)) );
  faction->addReport(new   ReportRecord(new BinaryPattern(oathReporter, this,faction)) );
	return SUCCESS;

}



BasicOrderSynchronizationRequest * PhysicalEntity::hasOrderSyncRequest(BasicOrderSynchronizationRequest * request)
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



void PhysicalEntity::removeOrderSyncRequest(BasicOrderSynchronizationRequest * request)
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



void PhysicalEntity::addOrderSyncRequest(BasicOrderSynchronizationRequest * request)
{
  orderSyncRequests_.push_back(request);
}



bool PhysicalEntity::doneOrderSyncRequest(BasicOrderSynchronizationRequest * request)
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



void PhysicalEntity::markDoneOrderSyncRequest(BasicOrderSynchronizationRequest * request)
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


