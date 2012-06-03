/***************************************************************************
                          ConstructionEntity.cpp
                             -------------------
    begin                : Sun Aug 31 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include <algorithm>
#include "GameFacade.h"
#include "RulesCollection.h"
#include "EntitiesCollection.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
#include "UnitEntity.h"
#include "ConstructionEntity.h"
#include "ConstructionRule.h"
#include "ConstructionWorksElement.h"
#include "ConstructionWorksVariety.h"
#include "UnaryMessage.h"
#include "ResourceElement.h"
#include "SkillRule.h"
#include "SkillElement.h"
#include "TravelElement.h"
#include "ObservationCondition.h"
#include "SkillCondition.h"
#include "CombatOrderLine.h"
#include "CombatMoveVariety.h"

extern ReportPattern * buildingDestroyedReporter;

extern ReportPattern * cantMoveReporter;
extern ReportPattern * departLeaderReporter;
extern ReportPattern * departFollowerReporter;
extern ReportPattern * departPublicReporter;
extern ReportPattern * arrivePrivateReporter;
extern ReportPattern * arrivePublicReporter;
ConstructionEntity   sampleConstructionEntity("BUILDING", &sampleTokenEntity);
//EntitiesCollection <ConstructionEntity>   buildingsAndShips
//        (new DataStorageHandler(gameConfig.getBuildingsFile() ), &sampleConstructionEntity );



ConstructionEntity::ConstructionEntity( const  ConstructionEntity* prototype ) : TokenEntity(prototype)
{
  effectiveStaff_ = 0;
  construction_ = 0;
  public_ = false;
  isUnfinished_ =false;
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
        setFaction(gameFacade->factions[parser->getWord()]);
        return OK;
    }

    if (parser->matchKeyword("CONSTRUCTION"))
    {
        construction_ = gameFacade->constructions[parser->getWord()];
        stats.addStats(construction_->getStats());
        return OK;
    }

    if (parser->matchKeyword("RESOURCE"))
    {
        ConstructionWorksElement * newResource = ConstructionWorksElement::readElement(parser);
        if(newResource)
            buildingWorks_.push_back(newResource);
        return OK;
    }
    if (parser->matchKeyword("PUBLIC"))
    {
        public_ = true;
        return OK;
    }
    if (parser->matchKeyword("UNFINISHED"))
    {
        isUnfinished_ = true;
        return OK;
    }

    return TokenEntity::initialize(parser );
}



void    ConstructionEntity::preprocessData()
{
	if(faction_)
	faction_-> addConstruction(this);
   if(construction_ == 0)
    cerr << "ERROR: "<< print() << " has undefined construction type"<<endl;
        this->addAllAddedUnits();
  TokenEntity::preprocessData();
}



/*
 * Monthly post-processing
 */
void ConstructionEntity::postProcessData()
{
	TokenEntity::postProcessData();
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

  if(public_)  out <<"PUBLIC"<<endl;
  if(isUnfinished_)  out <<"UNFINISHED"<<endl;
}


void ConstructionEntity::makeAlliedImage(TokenEntity * source)
{
    TokenEntity::makeAlliedImage(source);
}

void ConstructionEntity::makeObservedImage(TokenEntity * source)
{
    TokenEntity::makeObservedImage(source);

}

// Create token object as it is seen by referent
// Take into account observation. (imageLevel = obse)
// for allied units effective observation may be higher
ConstructionEntity *     ConstructionEntity::createConstructionImage(FactionEntity * referent, int observation)
{

    TokenEntity * token = createTokenImage(referent,observation);
    if(token==0)
    {
        return 0;
    }
    ConstructionEntity  * constructionToAdd = dynamic_cast<ConstructionEntity *> (token) ;

 // all:
   constructionToAdd->construction_ = this->construction_;
    constructionToAdd->public_ = this->public_;
 //allied
   if((referent->stanceAtLeast(this->getFaction(),alliedStance)))
    {
        constructionToAdd->buildingWorks_ = this->buildingWorks_;
    }
    if(!this->buildingWorks_.empty())
    {
      constructionToAdd->isUnfinished_ =true;
    }
    return constructionToAdd;
}


//// Create token object as it is seen by referent
//// Take into account observation. (imageLevel = obse)
//// for allied units effective observation may be higher
//ConstructionEntity *     ConstructionEntity::createConstructionImage(TokenEntity * referent)
//{
//
//    TokenEntity  * token = createTokenImage(referent);
//    if(token==0)
//    {
//        return 0;
//    }
//    ConstructionEntity  * constructionToAdd = dynamic_cast<ConstructionEntity *> (token) ;
//
// // all:
//   constructionToAdd->construction_ = this->construction_;
//    constructionToAdd->public_ = this->public_;
// //allied
//   if((referent->getFaction()->stanceAtLeast(this,alliedStance)))
//    {
//        constructionToAdd->buildingWorks_ = this->buildingWorks_;
//    }
//    if(!this->buildingWorks_.empty())
//    {
//      constructionToAdd->isUnfinished_ =true;
//    }
//    return constructionToAdd;
//}

// Update token image with the data from another image
void ConstructionEntity::updateImage(ConstructionEntity *construction)
{
//    if(image->getTimeStamp() < this->getTimeStamp())
//    {
//        return;
//    }

   this->TokenEntity::updateImage(construction);
// all:
    this->construction_ = construction->construction_;
    this->public_ = construction->public_;
     if(construction->imageLevel_ == ALLIED_IMAGE)
    {
        this->buildingWorks_ = construction->buildingWorks_;
        if(!this->buildingWorks_.empty())
        {
        this->isUnfinished_ =true;
        }
    }


}


int  ConstructionEntity::workToDo(ConstructionWorksVariety * buildingWorksType)
{
    for(vector<ConstructionWorksElement *>::iterator iter = buildingWorks_.begin(); iter != buildingWorks_.end();iter++)
  {
        //if(this->isTraced())
        //cout<< "++++>" <<print()<<":"<<(*iter)->getWorkType()->print()<<" vs. "<<buildingWorksType->print()<<endl;
    if((*iter)->getWorkType() == buildingWorksType)
    {
      return  (*iter)->getWorkAmount().roundUp();
     }
   }
   return 0;
}



bool ConstructionEntity::addBuildingWork(ConstructionWorksElement * buildingWorks)
{
      if(this->isTraced())
      {
          cout<< "Adding "<<buildingWorks->getWorkAmount()<< " of "<< buildingWorks->getWorkType()->print()<<" to "<<print()<<endl;
      }
  for(vector<ConstructionWorksElement *>::iterator iter = buildingWorks_.begin(); iter != buildingWorks_.end();iter++)
  {
//      if(this->isTraced())
//      {
//          cout<< "Adding "<<buildingWorks->getWorkAmount()<< " of "<< buildingWorks->getWorkType()->print()<<" to "<<print()<<endl;
//      }
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
  cerr << "BUILDING ERROR on building "<< construction_->print()<<" - no such material as " <<buildingWorks->getWorkType()->print()<<endl;
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


 reportAppearence(getFaction(), out);
  if(public_)  out <<" It is public.";
 reportInventory(getFaction(), out);
 reportSkills(getFaction(), out);
// recalculateStats();  // Do we really need that?
//   out << "Stats: ";stats.print(out);
 if(getTarget())
  out<< " Targeting "<< getTarget()<<".\n";
 out <<endl;

 out << "Events for "<<print()<<":"<<endl;
 reportEvents(out);
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
    if(!isCompleted())
      out << " (unfinished).";
// out.incr_indent();
// reportAppearence(getFaction(), out);
//  if(public_)  out <<" It is public.";
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
    }
    if(!isCompleted())
      out << " unfinished.";
    if(!description_.empty())
	{
		out<<description_;
	}
   if(public_)  out <<" It is public.";
 if(moving_)
    out << moving_->getMode()->getName()<<" to " << moving_->getDestination()->print()
    << " (" << moving_->getTravelTime() - moving_->getRemainingTime() <<" of "<<moving_->getTravelTime() <<" days)" ;
  // report crew
      out<<endl;

}



void ConstructionEntity::reportInventory(FactionEntity * faction, ReportPrinter &out)
{
  if(!inventory_.empty())
    {
      out << " have: ";
			inventory_.reportInventory(faction, out);
    }
  // capacities
  if(mayMove())
  {
    bool firstMode = true;
    int capacity;
    int weight = 0;
    out << ". Weight "<<calculateTotalWeight(weight);
    for(int i=0; i < gameFacade->movementModes.size(); i++)
    {
      capacity =  getCapacity(i);
      if( capacity )
        {
          if(firstMode)
            out << ". Capacity "<< capacity<< " (" << (gameFacade->movementModes[i])->getName()<<")\n";
          else
          out <<", "<< capacity<< " (" << (gameFacade->movementModes[i])->getName()<<")\n";
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

  for(UnitIterator iter = staff_.begin();
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
// if derived from condition - add weighed value to skills_
// pass throudh all skills_ and normalize

 //clear all skills experience
 SkillCondition * condition = dynamic_cast<SkillCondition *>(construction_->getStaffCondition());
 if(!condition)
  return false;
  assert(construction_->getMaxStaff());

  int currentSkillPoints;
//  SkillElement * currentSkillElement;

 for(SkillIterator iter = skills_.getAll().begin(); iter != skills_.getAll().end(); ++iter)
      {
        (*iter).setExpPoints(0);
       }
 for(UnitIterator staffIter = staff_.begin();
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
 for(SkillIterator skillIter = skills_.getAll().begin(); skillIter != skills_.getAll().end(); ++skillIter)
      {
        currentSkillPoints =   (*skillIter).getExpPoints ();
         if (currentSkillPoints== 0)
         {
           //delete (*skillIter);
           skills_.getAll().erase(skillIter);
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
  SkillCondition * condition = dynamic_cast<SkillCondition *>(construction_->getStaffCondition());
  if(condition == 0)
    return 0;
  if (construction_->getMaxStaff() == 0)
      return 0;
  if(skill != condition->getSkill())
  return 0;

  int totalSkillPoints = 0;
 for(UnitIterator iter = staff_.begin();
                                iter != staff_.end(); ++iter)
      {
        totalSkillPoints += (*iter)->getSkillPoints(skill) *
                            (*iter)->getFiguresNumber();
      }
  return  totalSkillPoints/ construction_->getMaxStaff();
}



int ConstructionEntity::addSkill(SkillElement  skill)
{
  for(UnitIterator iter = staff_.begin();
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



int ConstructionEntity::getCapacity(MovementVariety * mode)
{
  if(!construction_->getStaffCondition())
  return 0;
  if(construction_->getMaxStaff())
    return  getEffectiveStaff() * construction_->getCapacity(mode) / construction_->getMaxStaff();
  else
    return  construction_->getCapacity(mode);

}



bool ConstructionEntity::isOnBoard(UnitEntity * unit)
{
  UnitIterator iter = find(units_.begin(),units_.end(),unit);
  if(iter != units_.end())
  {
   return true;
   }
   return false;
}


void ConstructionEntity::addUnit(UnitEntity * unit)
{
	// may demand additional actions (update of location and unit's data)
		if(unit == 0)
			return;
	   unitsToAdd_.push_back(unit);
//		cout << "Unit " << unit << " added at " << *this << endl;
}



void ConstructionEntity::removeUnit(UnitEntity * unit)
{
 			return;
    unitsToRemove_.push_back(unit);
    removeStaff(unit); // May cause the same Tree problem as with adding/removing units

}

void ConstructionEntity::eraseRemovedUnit(UnitEntity * unit)
{
		if(unit == 0)
			return;
    UnitIterator iter = find(units_.begin(),units_.end(),unit);
    if( iter == units_.end())
			return;

     units_.erase( iter);
}

void ConstructionEntity::eraseAllRemovedUnits()
{
	for(UnitIterator iter = unitsToRemove_.begin(); iter != unitsToRemove_.end(); ++iter)
	{
		eraseRemovedUnit(*iter);
	}
	unitsToRemove_.clear();
}



void ConstructionEntity::addAllAddedUnits()
{
	for(UnitIterator iter =unitsToAdd_.begin(); iter != unitsToAdd_.end(); ++iter)
	{
//cout <<" * " << (*iter)->print() << " is added to "<< print() <<" at " << this->getLocation()->print()<<endl;
		units_.push_back(*iter);
	}
	unitsToAdd_.clear();
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
  UnitIterator iter = find(staff_.begin(),staff_.end(),unit);
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
	LocationEntity * location = location_;
  // remove title
  location->deleteTitle(construction_->getTitle());
  location->removeConstruction(this);
  // move all units otside
  for(UnitIterator iter = units_.begin();
                                iter != units_.end(); ++iter)
      {
      cout<<"    Destroying "<<this->print()<< " : "<< (*iter)->print()<<endl;
        (*iter)->setContainingConstruction(0);
        (*iter)->recalculateStats();
        removeUnit(*iter);
        // may be report ?
       }
   location->freeLand(construction_->getLandUse());
   location->addReport(new UnaryMessage(buildingDestroyedReporter, this));
  // RIP
    gameFacade->buildingsAndShips.addRIPindex(gameFacade->buildingsAndShips.getIndex(tag_));
}



int ConstructionEntity::getLandUse()
{
  return  construction_->getLandUse();
}



int ConstructionEntity::getProductionBonus(ItemRule * product, SkillRule * skill,int level)
{
  int value = construction_->getProductionBonusValue(skill);
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
  int value = construction_->getProductionBonusValue(skill);
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
  for(UnitIterator iter = units.begin(); iter != units.end(); ++iter)
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
            if(effectiveStaff_ >= getConstructionType()->getMaxStaff())
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
 // It may also happen that leader allowed to enter but
// one of followers is not.
// check that without this follower stack may move
// if not - retreat all stack
// if yes - unstack and retreat this follower
 getLocation()->removeConstruction(this);
  moving_->getDestination()->addConstruction(this);
	recalculateTravelTime();
}



void ConstructionEntity::moveGroupToLocation()
{
  moveToLocation();

  for(vector <UnitEntity *>::iterator iter = units_.begin(); iter != units_.end(); ++iter)
  {
    (*iter)->moveToLocation();
  }

}


void ConstructionEntity::movingGroupReport(ReportRecord  report )
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

  getLocation()->addReport(new UnaryMessage(arrivePublicReporter, this), 0 ,
                            ObservationCondition::createObservationCondition(getStealth() ));
  addReport(new UnaryMessage(arrivePrivateReporter, getLocation()));


//      if(moving_->getMode()== flyingMode)
//      {
//        stats.modifyStealth(-1);
//      }

  moving_ = 0;
  isMoving_ = false;
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
	 SkillCondition * condition = dynamic_cast<SkillCondition *>(construction_->getStaffCondition());
	  // may learn only skills derived from condition
	 if(condition == 0)
      return CANNOT_STUDY_FAILURE;

   if(!skill->isDescendFrom(condition->getSkill(),1))
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

/** Climate and other bonuses */
int ConstructionEntity::calculateMovementBonus(MovementVariety * mode)
{
	int bonus = 0;
	bonus = TokenEntity::calculateMovementBonus(mode);

// Construction
	bonus += getConstructionType()->getMovementBonus(mode);
   return bonus;
}


// Currently nothing
void ConstructionEntity::sufferDamage(int value)
{
}



string ConstructionEntity::printComposition()
{
	return construction_->print();
}


Rule * ConstructionEntity::getComposition()
{
	return construction_;
}



EntityStatistics  ConstructionEntity::getBasicStats()
{

  EntityStatistics basicStats;
 // construction type
  basicStats.addStats(construction_->getStats());
 // Effects
     enchantments_.addStats(&basicStats,getFiguresNumber());

return basicStats;
}


// Destroy construction
void ConstructionEntity::disband()
{
  isDisbanded_ = true;
	destroy();
}




void ConstructionEntity::dailyUpdate()
{
    if(isDisbanded())
    {
        return;
    }
    enchantments_.carryOutAllActions(this,0);
   enchantments_.processExpiration(this,getFiguresNumber());
   TokenEntity::dailyUpdate();
    addAllAddedUnits();
// if terrain is ocean or lake check swimming capacity
// ifnot enough - drowning
// smart drowning
// - equip items that may rise capacity
// sortout  items that have positive capacity/weight ratio
// sort all items according to weight/price ratio
// throw away items
   LocationEntity * location = getLocation();
   if(location)
	{
		if(location->getTerrain()->isAquatic())
		{
			if (getCapacity(swimingMode) < getWeight())
				{
				cout << "SOS! " << print()<< " is drowning\n";
				// drop away cargo
				}
		}
	}

}

void ConstructionEntity::setDefaultCombatMovement()
{
	if(mayMove())
	{
		if(combatTactics_.getCombatMove())
			defaultCombatMovement_ =
					new CombatOrderLine( combatTactics_.getCombatMove()->getTag(), this);
		else
			defaultCombatMovement_ =
					new CombatOrderLine( defaultCombatMove->getTag(), this);
	}
	else // Can't move should stay.
	{
		defaultCombatMovement_ =
					new CombatOrderLine( "stand", this);
	}
}



int ConstructionEntity::getObservationRating() const
{
   return 0;
}



int ConstructionEntity::getStealthRating()
{
  return 0;
}
void ConstructionEntity::extractAndAddKnowledge(Entity * recipient, int parameter)
{
TokenEntity::extractAndAddKnowledge(recipient, parameter);
recipient->addKnowledge(construction_);
  for (vector <ResourceElement *>::iterator iter = resourceQuotas_.begin();
       iter != resourceQuotas_.end(); ++iter)
       {
         recipient->addKnowledge((*iter)->getResource());
  }
}

//void ConstructionEntity::extractSkillKnowledge(Entity * recipient, int parameter)
//{
//
//}
