/***************************************************************************
                          ConstructionEntity.cpp 
                             -------------------
    begin                : Sun Aug 31 2003
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
#include "RulesCollection.h"
#include "EntitiesCollection.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
#include "UnitEntity.h"
#include "ConstructionEntity.h"
#include "ConstructionRule.h"
#include "ConstructionWorksElement.h"
#include "ConstructionWorksVariety.h"
#include "UnaryPattern.h"
#include "ResourceElement.h"
#include "SkillRule.h"
#include "SkillElement.h"
#include "TravelElement.h"
#include "ObservationCondition.h"
#include "SkillCondition.h"


extern Reporter * buildingDestroyedReporter;

extern Reporter * cantMoveReporter; 
extern Reporter * departLeaderReporter;
extern Reporter * departFollowerReporter;
extern Reporter * departPublicReporter;
extern Reporter * arrivePrivateReporter;
extern Reporter * arrivePublicReporter;


ConstructionEntity::ConstructionEntity( const  ConstructionEntity* prototype ) : TokenEntity(prototype)
{
  effectiveStaff_ = 0;
  construction_ = 0;
}


ConstructionEntity::~ConstructionEntity(){
}



GameData * ConstructionEntity::createInstanceOfSelf()
{
   return CREATE_INSTANCE<ConstructionEntity> (this);
}


STATUS
ConstructionEntity::initialize        ( Parser *parser )
{
 if (parser->matchKeyword("OWNER"))
	{
	  setFaction(factions[parser->getWord()]);
	  return OK;
	}

 if (parser->matchKeyword("CONSTRUCTION"))
	{
	  construction_ = constructions[parser->getWord()];
	  return OK;
	}

  if (parser->matchKeyword("RESOURCE"))
	  {
        ConstructionWorksElement * newResource = ConstructionWorksElement::readElement(parser);
        if(newResource)
          buildingWorks_.push_back(newResource);
	    return OK;
	  }
	return TokenEntity::initialize(parser );
}



void    ConstructionEntity::preprocessData()
{
	if(faction_)
	faction_-> addConstruction(this);
   if(construction_ == 0)
    cout << "ERROR: "<< print() << " has undefined construction type"<<endl;
}



/*
 * Monthly post-processing
 */
void ConstructionEntity::postProcessData()
{
}



//void      payUpkeep()
//{
//
//}



void      ConstructionEntity::save (ostream &out)
{
  if(isDisbanded())
      return;

  TokenEntity::save(out);
  if(moving_ !=0)  moving_->save(out);

  if(construction_) out << "CONSTRUCTION " << construction_->getTag()<<endl;

   for (vector<ConstructionWorksElement *>::iterator iter = buildingWorks_.begin();
        iter !=buildingWorks_.end(); ++iter)
        {
         out << "RESOURCE " << (*iter)->getWorkType()->getTag()<< " "<<(*iter)->getWorkAmount()<<endl; 
        }
//  if(()) out << " " << ()->()<<endl;
//  if(()) out << " " << ()->()<<endl;
//  if(()) out << " " << ()->()<<endl;

  if(getFaction()) out << "OWNER " << getFaction()->getTag()<<endl;

}



int  ConstructionEntity::workToDo(ConstructionWorksVariety * buildingWorksType)
{
    for(vector<ConstructionWorksElement *>::iterator iter = buildingWorks_.begin(); iter != buildingWorks_.end();iter++)
  {
    if((*iter)->getWorkType() == buildingWorksType)
    {
      return  (*iter)->getWorkAmount().roundUp();
     }
   }
   return 0;
}  



bool ConstructionEntity::addBuildingWork(ConstructionWorksElement * buildingWorks)
{
  for(vector<ConstructionWorksElement *>::iterator iter = buildingWorks_.begin(); iter != buildingWorks_.end();iter++)
  {
    if((*iter)->getWorkType() == buildingWorks->getWorkType())
    {
      if((*iter)->getWorkAmount() > buildingWorks->getWorkAmount())
        {
          (*iter)->setWorkAmount((*iter)->getWorkAmount() - buildingWorks->getWorkAmount() );
          return false;
        }
        else
          {
            (*iter)->setWorkAmount(0);
            buildingWorks_.erase(iter);
          }
        return  isCompleted();
    }
  }
  cout << "BUILDING ERROR on building "<< construction_->print()<<" - no such material as" <<buildingWorks->getWorkType()->print()<<endl;
  return false;
}



void      ConstructionEntity::produceFactionReport (FactionEntity * faction, ReportPrinter &out)
{
    out << print() << " " <<construction_->getName();
    if(isDisbanded())
    {
      out << " (destroyed) "<<endl;
      out <<endl;
      return;
    }
     out<< " at "<< location_->print()<<".\n";
     out.incr_indent();
  for(vector<ConstructionWorksElement *>::iterator iter = buildingWorks_.begin(); iter != buildingWorks_.end();iter++)
  {
    if(iter == buildingWorks_.begin())
        out << " Needs ";
    else
        out << ", ";

    out<<(*iter)->getWorkAmount()<< " "<<(*iter)->getWorkType()->print();
  }
   
 reportAppearence(getFaction(), out);
 reportInventory(getFaction(), out);
 reportSkills(getFaction(), out);
// recalculateStats();  // Do we really need that?
//   out << "Stats: ";stats.print(out);
 if(getTarget())
  out<< " Targeting "<< getTarget()<<".\n";
 out <<endl;
 out.decr_indent();
    out <<endl;


}


void      ConstructionEntity::privateReport (ReportPrinter &out)
{
    
    out << " * " << print()  <<" "<<construction_->getName(); out.incr_indent();
    if(isDisbanded())
    {
      out << " (destroyed) "<<endl;
      out <<endl;
      return;
    }
// out.incr_indent();
// reportAppearence(getFaction(), out);
 reportInventory(getFaction(), out);
 reportSkills(getFaction(), out);
// recalculateStats();  // Do we really need that?
//   out << "Stats: ";stats.print(out);
 if(getTarget())
  out<< "Targeting "<< getTarget()<<".\n";
 out <<endl;
 out.decr_indent();
// out.decr_indent();
}



void      ConstructionEntity::publicReport (int observation, ReportPrinter &out)
{
    if(isDisbanded())
          return;
    out << " - "<< print()  <<construction_->getName();
    if(getFaction())
    {
      out << " (belongs to ";
      out <<getFaction()->print()<<")";
      if(!isCompleted())
        out << " unfinished.";
    }

 if(moving_)
    out << moving_->getMode()->getName()<<" to " << moving_->getDestination()->print()
    << " (" << moving_->getTravelTime() - moving_->getRemainingTime() <<" of "<<moving_->getTravelTime() <<" days)" ;
  // report crew
      out<<endl;

}



void ConstructionEntity::reportInventory(FactionEntity * faction, ReportPrinter &out)
{
    bool isFirst = true;
  if(!inventory_.empty())
    {
      out << " have: ";
      isFirst = true;
	    for (InventoryElementIterator iter = inventory_.begin();
                                iter != inventory_.end(); ++iter)
        {
          faction->addKnowledge((*iter)->getItemType());
          if( isFirst)
            {
              isFirst = false;
            }
          else
            {
              out << ", ";
            }
          (*iter)->reportEquipment(out);
        }
    out <<". ";
    }
  // capacities
  if(mayMove())
  {
    bool firstMode = true;
    int capacity;
    int weight = 0;
    out << ". Weight "<<calculateTotalWeight(weight);
    for(int i=0; i < movementModes.size(); i++)
    {
      capacity =  getCapacity(i);
      if( capacity )
        {
          if(firstMode)
            out << ". Capacity "<< capacity<< " (" << (movementModes[i])->getName()<<")\n";
          else
          out <<", "<< capacity<< " (" << (movementModes[i])->getName()<<")\n";
          firstMode = false;
        }

      }
//      out<<endl;
  }
}



void    ConstructionEntity::reportAppearence(FactionEntity * faction, ReportPrinter &out)
{
 if(moving_)
    out << moving_->getMode()->getName()<<" to " << moving_->getDestination()->print()
    << " (" << moving_->getTravelTime() - moving_->getRemainingTime() <<" of "<<moving_->getTravelTime() <<" days)" ;

//  if((getFaction() == faction) || isObservableBy(faction) )
//  {
    for(vector<ConstructionWorksElement *>::iterator iter = buildingWorks_.begin(); iter != buildingWorks_.end();iter++)
      {
        if(iter == buildingWorks_.begin())
          out << " needs ";
        else
          out << ", ";
        out<<(*iter)->getWorkAmount()<< " "<<(*iter)->getWorkType()->getName();
      }
//  }
//  else
//    if(!isCompleted())
//        out << " unfinished.";

  for(vector <UnitEntity *>::iterator iter = staff_.begin();
                                iter != staff_.end(); ++iter)
      {
        if(iter == staff_.begin())
        {
          out<< " Operated by ";
        }
        else
        {
          out << ", " ;
        }
          out << (*iter)->print();
      }
  if(construction_->getMaxStaff())
    {
        out << " total staff "<< effectiveStaff_
            << " (of "<<construction_->getMaxStaff() <<").\n";
    }
}



bool ConstructionEntity::isCompleted() const
{
  for(vector<ConstructionWorksElement *>::const_iterator iter = buildingWorks_.begin(); iter != buildingWorks_.end();iter++)
  {
    if((*iter)->getWorkAmount()!=0) 
      return false;
  }
      return true;
  
}



void ConstructionEntity::buildingCompleted()
{
  construction_->finishConstruction(location_);
}


/*
 * Construction's skill defined as average skill of the staff.
 * After each change in the staff Construction's skill should be
 * recalculated. 
 */
bool ConstructionEntity::recalculateSkills()
{
// count all skills of staff if it is derived from condition.
// clear skills
// pass through all staff
// pass through all staff's skills
// if derived from condition - add weifghed value to skills_
// pass throudh all skills_ and normalize

 //clear all skills experience
 SkillCondition * condition = construction_->getStaffCondition();
 if(!condition)
  return false;
  assert(construction_->getMaxStaff());
  
  int currentSkillPoints;
//  SkillElement * currentSkillElement;
 for(SkillIterator iter = skills_.begin(); iter != skills_.end(); ++iter)
      {
        (*iter).setExpPoints(0);
       }
 for(vector <UnitEntity *>::iterator staffIter = staff_.begin();
                                staffIter != staff_.end(); ++staffIter)
      {
        for(SkillIterator skillIter = ((*staffIter)->getAllSkills()).begin();
                     skillIter!= (*staffIter)->getAllSkills().end(); ++skillIter)
        {
         if((*skillIter).getSkill()->isDescendFrom(condition->getSkill(), condition->getLevel()))
          {
            TokenEntity::addSkill( (*skillIter).getSkill(),
                              (*skillIter).getExpPoints() * (*staffIter)->getFiguresNumber());
          }
        }
      }

// normalize
 for(SkillIterator skillIter = skills_.begin(); skillIter != skills_.end(); ++skillIter)
      {
        currentSkillPoints =   (*skillIter).getExpPoints ();
         if (currentSkillPoints== 0)
         {
           //delete (*skillIter);
           skills_.erase(skillIter);
         }
         else
         {
          ++skillIter;
          (*skillIter).setExpPoints (currentSkillPoints/construction_->getMaxStaff());
        }
      }
return false;
}



int ConstructionEntity::calculateSkill(SkillRule *  skill)
{
  if(!construction_->getStaffCondition())
    return 0;
  if (construction_->getMaxStaff() == 0)
      return 0;
  if(skill != construction_->getStaffCondition()->getSkill())
  return 0;
  
  int totalSkillPoints = 0;
 for(vector <UnitEntity *>::iterator iter = staff_.begin();
                                iter != staff_.end(); ++iter)
      {
        totalSkillPoints += (*iter)->getSkillPoints(skill) *
                            (*iter)->getFiguresNumber();
      }
  return  totalSkillPoints/ construction_->getMaxStaff();
}



int ConstructionEntity::addSkill(SkillElement  skill)
{
  for(vector <UnitEntity *>::iterator iter = staff_.begin();
                                iter != staff_.end(); ++iter)
      {
        (*iter)->addSkill(skill.getSkill(), skill.getExpPoints()); 
      }
  return recalculateSkills();
}



int ConstructionEntity::addSkill(SkillRule  * skill, int expPoints)
{
 return TokenEntity::addSkill(skill,expPoints);
}



int ConstructionEntity::getStealth() const
{
 return  construction_->getStats()->getStealth();
}



int ConstructionEntity::getCapacity(int modeIndex)
//void recalculateCapacity()
{
  if(!construction_->getStaffCondition())
  return 0;
  if(construction_->getMaxStaff())
    return  getEffectiveStaff() * construction_->getCapacity(modeIndex) / construction_->getMaxStaff();
  else
    return  construction_->getCapacity(modeIndex);
}



bool ConstructionEntity::isOnBoard(UnitEntity * unit)
{
  vector <UnitEntity *>::iterator iter = find(units_.begin(),units_.end(),unit);
  if(iter != units_.end())
  {
   return true;
   }
   return false;
}


void ConstructionEntity::addUnit(UnitEntity * unit)
{
  units_.push_back(unit) ;
}



void ConstructionEntity::removeUnit(UnitEntity * unit)
{
  vector <UnitEntity *>::iterator iter = find(units_.begin(),units_.end(),unit);
  if(iter != units_.end())
  {
    units_.erase(iter);
    removeStaff(unit);
    }
}



void ConstructionEntity::addStaff(UnitEntity * unit)
{
  staff_.push_back(unit) ;
  effectiveStaff_ = min(effectiveStaff_ + unit->getFiguresNumber() , construction_->getMaxStaff());  
//  unit->setHidden(true);
    unit->setPassenger(true);
    recalculateSkills();
}



void ConstructionEntity::removeStaff(UnitEntity * unit)
{
  vector <UnitEntity *>::iterator iter = find(staff_.begin(),staff_.end(),unit);
  if(iter != staff_.end())
  {
    staff_.erase(iter);
    effectiveStaff_ = effectiveStaff_ - unit->getFiguresNumber();
    assert (effectiveStaff_ >= 0);
//    unit->setHidden(false);
    unit->setPassenger(false);
    recalculateSkills();
  }
  
}



void ConstructionEntity::destroy()
{
  location_->removeConstruction(this);
  // remove title
  location_->deleteTitle(construction_->getTitle());
  // move all units otside
  for(vector <UnitEntity *>::iterator iter = units_.begin();
                                iter != units_.end(); ++iter)
      {
        (*iter)->setContainingConstruction(0);
        (*iter)->recalculateStats();
        removeUnit(*iter);
        // may be report ?
       }
   location_->freeLand(construction_->getLandUse());
   location_->addReport(new UnaryPattern(buildingDestroyedReporter, this));
  // RIP
    buildingsAndShips.addRIPindex(buildingsAndShips.getIndex(tag_));
}



int ConstructionEntity::getLandUse()
{
  return  construction_->getLandUse();
}



int ConstructionEntity::getProductionBonus(ItemRule * product, SkillRule * skill,int level)
{
  int value = construction_->getProductionBonusValue(skill, level);
  if(value == 0)
    return 0;
// If land use implemented
//  value = (value * getLocation()->getResource(product) * getLandUse() ) /
//               (getLocation()->getTotalLand() * 100);

  value = (value * getLocation()->getResource(product)) / 100;

  for (vector <ResourceElement *>::iterator iter = resourceQuotas_.begin();
       iter != resourceQuotas_.end(); ++iter)
       {
         if ((*iter)->getResource() == product )
         {
           return  (*iter)->getAvailableResource().getValue();
         }
       }

    resourceQuotas_.push_back(new ResourceElement(product,value));
    return value;
}



void ConstructionEntity::setProductionBonus(ItemRule * product, SkillRule * skill,int level)
{
  int value = construction_->getProductionBonusValue(skill, level);
  if(value == 0)
    return ;
// If land use implemented
//  value = (value * getLocation()->getResource(product) * getLandUse() ) /
//               (getLocation()->getTotalLand() * 100);

  value = (value * getLocation()->getResource(product)) / 100;

  for (vector <ResourceElement *>::iterator iter = resourceQuotas_.begin();
       iter != resourceQuotas_.end(); ++iter)
       {
         if ((*iter)->getResource() == product )
         {
           return;
         }
       }

    resourceQuotas_.push_back(new ResourceElement(product,value));
}



RationalNumber ConstructionEntity::useProductionBonus(ItemRule * product, RationalNumber number)
{
  for (vector <ResourceElement *>::iterator iter = resourceQuotas_.begin();
       iter != resourceQuotas_.end(); ++iter)
       {
         if ((*iter)->getResource() == product )
         {
           int available = (*iter)->getAvailableResource().getValue();
           if(available > number)
            {
              (*iter)->setAvailableResource(available - number);
              return number;
            }
           else
            {
              (*iter)->setAvailableResource(0);
              return available;
            }   
         }
       }
  return 0;     
}



bool ConstructionEntity::mayMove()
{
  return getConstructionType()->mayMove();
}



bool ConstructionEntity::leaveStaying()
{
  vector <UnitEntity *> units = units_;
  for(vector <UnitEntity *>::iterator iter = units.begin(); iter != units.end(); ++iter)
  {
    if( (*iter)->isStaying() )
       (*iter)->exitConstruction();
  }
  return false;
}



int  ConstructionEntity::calculateTotalWeight (int & weight)
{
  weight += getConstructionType()->getWeight();
  for(vector <UnitEntity *>::iterator iter = units_.begin(); iter != units_.end(); ++iter)
  {
    weight += (*iter)->getWeight();
  }
  return weight;
}



/** Check crew skills */
bool ConstructionEntity::mayUseAsStaff(UnitEntity * unit)
{
  return getConstructionType()->getStaffCondition()->isSatisfied(unit);
}



void ConstructionEntity::calculateTotalCapacity(int & capacity, int modeIndex)
{
  if(effectiveStaff_ < getConstructionType()->getMaxStaff())
  {
   // try to assign crew
  for (vector <UnitEntity *>::iterator iter = units_.begin();
        iter != units_.end(); ++iter)
        {
          if(( *iter) ->isStaff()  )
              continue;
          if(mayUseAsStaff(*iter))
          {
            addStaff(*iter);    
            if(effectiveStaff_ < getConstructionType()->getMaxStaff())
              break;
           }   
        }

     
  }
  capacity += getCapacity(modeIndex);
  for(vector <UnitEntity *>::iterator iter = units_.begin(); iter != units_.end(); ++iter)
  {
    capacity += (*iter)->getCapacity(modeIndex);
  }  
}



void ConstructionEntity::moveToLocation()
{
  TokenEntity::moveToLocation();
  getLocation()->removeConstruction(this);
  moving_->getDestination()->addConstruction(this);
}



void ConstructionEntity::moveGroupToLocation()
{
  moveToLocation();
  
  for(vector <UnitEntity *>::iterator iter = units_.begin(); iter != units_.end(); ++iter)
  {
    (*iter)->moveToLocation();
  }
  
}


void ConstructionEntity::movingGroupReport(ReportRecord * report )
{
  for(vector <UnitEntity *>::iterator iter = units_.begin(); iter != units_.end(); ++iter)
  {
    (*iter)->addReport(report);
  }
}



void ConstructionEntity::movingGroupArrived()
{
  for(vector <UnitEntity *>::iterator iter = units_.begin(); iter != units_.end(); ++iter)
  {
    (*iter)->movingEntityArrived();
  }
}

void ConstructionEntity::movingEntityArrived()
{

  getLocation()->addReport(new UnaryPattern(arrivePublicReporter, this), 0 ,
                            ObservationCondition::createObservationCondition(getStealth() ));
  addReport(new UnaryPattern(arrivePrivateReporter, getLocation()));


//      if(moving_->getMode()== flyingMode)
//      {
//        stats.modifyStealth(-1);
//      }

  moving_ = 0;

}



bool ConstructionEntity::mayInterract(UnitEntity * unit)
{
  for(vector <UnitEntity *>::iterator iter = units_.begin(); iter != units_.end(); ++iter)
  {
    if((*iter) == unit)
    {
//      if (this->getObservation() >= unit->getStealth() )
//        return true;
//      if (this->getLocation()->getFactionalObservation(getFaction()) >= unit->getStealth() )
//        return true;
      return true;
    }
  }
  return false;
}



/*
 * ConstructionEntity may be unaccessible for orders for some reasons:
 * - if it is not finished yet,
 *  - if it is on the way
 *  - if it is destroyed
 */
bool ConstructionEntity::isUnaccessible() const
{
  if(!isCompleted())
      return true;
  if(location_ == 0)
      return true;
  if(moving_)
      return true;
  else
    return false;
}



bool ConstructionEntity::isOfType(Rule * type)
{
 return getConstructionType()->isDescendantFrom(type);
}



bool ConstructionEntity::isObservableBy(FactionEntity * faction)
{

  for(vector <UnitEntity *>::iterator iter = units_.begin(); iter != units_.end(); ++iter)
  {
    if((*iter)->getFaction() == faction)
    return true;
  }
  return false;
}

/*
 *  Intristic ability of construction to study  skill
 *  Level limitations also taken into considerations
 */
LEARNING_RESULT ConstructionEntity::mayLearn(SkillRule * skill)
{
  // may learn only skills derived from condition
   if(!skill->isDescendFrom(construction_->getStaffCondition()->getSkill(),1))
      return CANNOT_STUDY_FAILURE;


   // If at least one of the staff may study
  for(vector <UnitEntity *>::iterator iter = staff_.begin();
                iter != staff_.end(); ++iter)
  {
    if(skill->mayBeStudied(*iter) == LEARNING_OK)
    return LEARNING_OK;
  }
    return CANNOT_STUDY_FAILURE;
}

