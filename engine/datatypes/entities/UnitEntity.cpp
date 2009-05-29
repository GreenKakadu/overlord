/***************************************************************************
                          UnitEntity.cpp
                             -------------------
    begin                : Tue Nov 13 18:07:56 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include <algorithm>
#include "UnitEntity.h"
#include "RationalNumber.h"
#include "InventoryElement.h"
#include "RaceElement.h"
#include "SkillLevelElement.h"
#include "TravelElement.h"
#include "TitleElement.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
#include "RaceRule.h"
#include "WeatherRule.h"
#include "SkillRule.h"
#include "TitleRule.h"
#include "IntegerData.h"
#include "BasicExit.h"
#include "ItemElement.h"
#include "GameConfig.h"
#include "SkillUseElement.h"
#include "PrototypeManager.h"

#include "EntitiesCollection.h"
#include "RulesCollection.h"

#include "ReportRecord.h"
#include "ReportMessage.h"
#include "ReportPattern.h"
#include "SimpleMessage.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "QuartenaryMessage.h"
#include "QuintenaryMessage.h"
#include "ConstructionEntity.h"
#include "ConstructionRule.h"
#include "WagesCompetitiveRequest.h"
#include "ObservationCondition.h"
#include "BattleUnit.h"
// reporters
UnitEntity     sampleUnit    ("UNIT",    &sampleTokenEntity);
EntitiesCollection <UnitEntity>   units(new DataStorageHandler(gameConfig.getUnitsFile()));
extern ReportPattern * withdrawReporter;
extern ReportPattern * borrowReporter;
extern ReportPattern * lendReporter;
extern ReportPattern * cantMoveReporter; //
extern ReportPattern * departLeaderReporter;
extern ReportPattern * departFollowerReporter;
extern ReportPattern * departPublicReporter;
extern ReportPattern * arrivePrivateReporter;
extern ReportPattern * arrivePublicReporter;
extern ReportPattern * skillLossReporter;
extern ReportPattern * mergeSkillReporter;
extern ReportPattern * followerExitReporter;
extern ReportPattern * leaderExitReporter;
extern ReportPattern * followerEnterReporter;
extern ReportPattern * leaderEnterReporter;
extern ReportPattern * consumeFoodReporter;
extern ReportPattern * upkeepReporter;
extern ReportPattern * cannotPayUpkeepReporter;
extern ReportPattern * disbandReporter;
ReportPattern  *	guardMovesReporter = new ReportPattern("guardMovesReporter");
ReportPattern  *	guardInsideReporter = new ReportPattern("guardInsideReporter");
ReportPattern  *	guard20FiguresReporter = new
                   ReportPattern("guard20FiguresReporter");
ReportPattern  *	guardWeaponsReporter = new ReportPattern("guardWeaponsReporter");
ReportPattern  *	guardNotEnoughReporter = new
                    ReportPattern("guardNotEnoughReporter");

UnitEntity::UnitEntity(const string & keyword, GameData * parent)  : TokenEntity(keyword, parent)
{
    keyword_   = keyword;
    parent_   = parent;
    prototypeManager->addToRegistry(this);
}



UnitEntity::UnitEntity(const UnitEntity * prototype): TokenEntity(prototype)
{
     raceComposition_ =0;
      stackFollowingTo_ = 0;
      staying_  = false;
      consuming_ = true;
      exposeFlag_   = false;
      containingConstruction_=0;
      discontenting_ = false;
      isAssignedToStaff_  = false;
			mana_ = 0;
//  cerr << "UnitEntity constructor";
// combatTactics_.report(cerr);
// cerr <<endl;

}


/*
 * Virtual constructor
 * Creates new unit at specified location, belonging to specified faction
 * and consisting of specified nimber of figured of specified race
 */
UnitEntity * UnitEntity::createUnit (FactionEntity * faction, RaceRule * race,
                  int number, LocationEntity * location)
{
  UnitEntity * newUnit = new UnitEntity(this);
  if(units.addNew(newUnit) != OK)
      {
        cout << "Failed to add new unit \n";
        return 0;
   }

   newUnit->setRace(race,number);
   location->addUnit(newUnit);
   faction->addUnit(newUnit);
   newUnit->recalculateStats();
   return newUnit;
}


/*
 *
 */
void UnitEntity::setRace(RaceRule * race, int number)
{
  raceComposition_ = new RaceElement(race,number);
}



/*
 *
 */
GameData * UnitEntity::createInstanceOfSelf()
{
   return CREATE_INSTANCE<UnitEntity> (this);
}



/*
 *
 */
STATUS UnitEntity::initialize        ( Parser *parser )
{


  if (parser->matchKeyword("RACE"))
	{
    raceComposition_ = RaceElement::readElement(parser);

	  return OK;
	}

	if (parser->matchKeyword("MANA"))
	{
    mana_ =  parser->getInteger();
	  return OK;
	}

 if (parser->matchKeyword("CONTAINER"))
	{
	  containingConstruction_ = buildingsAndShips[parser->getWord()];
    if (parser->matchKeyword("STAFF"))
        {
           containingConstruction_->addStaff(this);
        }

		return OK;
	}

 if (parser->matchKeyword("STACK"))
	{
	  UnitEntity * unit = units[ parser->getWord()];
		if(unit != 0)
				stack (this, unit);
		return OK;
	}

 if (parser->matchKeyword("NO_CONSUME"))
	{
    consuming_ = false;
		return OK;
	}
 if (parser->matchKeyword("DISCONTENT"))
	{
    discontenting_ = true;
		return OK;
	}
	  return TokenEntity::initialize(parser );

}



/*
 *
 */
void UnitEntity::save(ostream &out)
{
  if(isDisbanded())
      return;
  TokenEntity::save(out);
  if(!consuming_) out << "NO_CONSUME"<<endl;
  if(discontenting_) out << "DISCONTENT"<<endl;
  if(mana_) out << "MANA "<< mana_<<endl;

  if((moving_ !=0) && (stackFollowingTo_ == 0))  moving_->save(out);
	out << "RACE "; raceComposition_->save(out);
	if(stackFollowingTo_) out << "STACK " << stackFollowingTo_->getTag() << endl;
  if(containingConstruction_)  out << "CONTAINER " <<containingConstruction_->getTag();
                              if(isAssignedToStaff_)   out << "STAFF "<<endl;
                              else out <<endl;
}





/*
 *
 */
void UnitEntity::dailyUpdate()
{
   enchantments_.processExpiration(getFiguresNumber());
   TokenEntity::dailyUpdate();

// if terrain is ocean or lake check swimming capacity
// ifnot enough - drowning
// smart drowning
// - equip items that may rise capacity
// sortout  items that have positive capacity/weight ratio
// sort all items according to weight/price ratio
// throw away items
   if(getLocation() ==0)
{
	return;
}
   if(getLocation()->getTerrain()->isAquatic())
   {
     if ((getCapacity(swimingMode) < getWeight()) && (getContainingConstruction()==0))
//     if (getStackCapacity(3) < getStackWeight())
      {
       cout << "SOS! " << print()<< " is drowning\n";
       // unstack all
      }
   }

	 // Regenerate mana
	 if(mana_ < stats.getMana())
	 {
	 	setMana(mana_ + 1);
	 }
	 if(mana_ > stats.getMana())
	 {
	 	setMana(mana_ - 1);
	 }

}



void      UnitEntity::payUpkeep()
{
	if(!isPayingUpkeep())// Should not pay upkeep
	{
	  return;
	}
  int upkeep = stats.getUpkeep() * getFiguresNumber();
  //  winter upkeep is higher?
  if(getConsuming())
  {

    int localFood = hasItem(food);
    if(localFood >= getFiguresNumber())
      {
        localFood = getFiguresNumber();
        takeFromInventory(food,localFood);
      }
    else
    {
    takeFromInventory(food,localFood);
    localFood += borrow(food, getFiguresNumber() - localFood );
    }
    if(localFood)
    {
    upkeep -= (upkeep * localFood / getFiguresNumber()) * 80 /100;
    addReport(new UnaryMessage(consumeFoodReporter, new IntegerData(localFood)));
    }
  }
  if(pay(upkeep))
  {
    addReport(new UnaryMessage(upkeepReporter, new IntegerData(upkeep)));
  }
  else
  {
    addReport(new SimpleMessage(cannotPayUpkeepReporter));
    if(getDiscontenting())
    {
      addReport(new UnaryMessage(disbandReporter,this));
      disband();
    }

    else
	{
      		if(!getFaction()->isNPCFaction())
		{	
			setDiscontenting(true);
		}
	}

  }
}



void UnitEntity::postProcessData()
{
  if(location_ && isNpcGuard() && getLoyality() < 100)
  {
    // Loyality raise may also depend on owner's abilities.
    // That is not implemented
    int newLoyality = getLoyality() + 20;
    if(newLoyality > 100)
    {
      newLoyality = 100;
    }
    setLoyality(newLoyality);
    if(newLoyality == 100)
    {
      if(location_->getLegalOwner())
      {
        if(location_->getLegalOwner() != faction_)
        {
        oath(location_->getLegalOwner());
        }
      }
    }
  }
  if(toOath_)
  {
    doOath();
  }
	TokenEntity::postProcessData();
}



void UnitEntity::doOath()
{
    FactionEntity * oldFaction = getFaction();
    oldFaction->removeUnit(this);
  for(TitleIterator iter = titles_.getAll()->begin();
					iter != titles_.getAll()->end(); ++iter)
  {
   oldFaction->removeMaxControlPoints((*iter)->getTitle()->getControl());
   (*iter)->desactivateClaimingEffects(); // will clean ownership info
  }
    setFaction(toOath_);
    toOath_->addUnit(this);
    toOath_->addVisitedLocation(getLocation());
    // Transfer titles and land ownership
  for(TitleIterator iter = titles_.getAll()->begin();
	iter != titles_.getAll()->end(); ++iter)
  {
   toOath_->addMaxControlPoints((*iter)->getTitle()->getControl());
   (*iter)->activateClaimingEffects(); // will renew ownership info
  }
}
//===============================================================================
//
//                          REPORTING
//
//===============================================================================
/*
 *
 */
void UnitEntity::produceFactionReport(FactionEntity * faction, ReportPrinter &out)
{
  if(isHidden())
      return;

  out  << print();


  if(isDisbanded())
    {
      out << " [disbanded] "<<endl;
      reportEvents(out);
      out <<endl;
      return;
    }

    else

   titles_.printAll(out);
   out  << " at "<<getLocation()->print();

 if(moving_)
    out << moving_->getMode()->getName()<<" to " << moving_->getDestination()->print()
    << " (" << moving_->getTravelTime() - moving_->getRemainingTime() <<" of "<<moving_->getTravelTime() <<" days)" ;
 if(stackFollowingTo_)
    out << " following " << getLeader()->print();
 if(getContainingConstruction())
    out << " inside " << getContainingConstruction()->print();
    out  <<". "<< endl;

    out.incr_indent();
    out<<raceComposition_->print();

    reportInventory(faction, out);
    reportSkills(faction, out);


 recalculateStats();  // Do we really need that?
 out << "Stats: " << stats;
  if(mana_) out << "Mana: " <<hasMana()<< "("<< stats.getMana()<<")";
 out << " Upkeep: " << stats.getUpkeep() * getFiguresNumber() << " coins.";
 if(getTarget())
  out<< " Targeting "<< getTarget()->print()<<".\n";
 out <<endl<<endl;

 out << "Events for "<<print()<<":"<<endl;
 reportEvents(out);
    out <<endl;
    out.decr_indent();
}



/*
 * Prints standard report for unit owner
 */
void UnitEntity::privateReport(ReportPrinter &out)
{
  if(isHidden())
      return;

  out  << " * "<< print();

    if(isDisbanded())
    {
      out << " (disbanded) "<<endl;
      return;
    }

 out.incr_indent();
 reportAppearence(getFaction(), out);
 reportInventory(getFaction(), out);
 reportSkills(getFaction(), out);
 recalculateStats();  // Do we really need that?
   out << "Stats: "<< stats;
if(hasMana())
{
   out << " Mana: " <<hasMana()<<"("<<stats.getMana() <<"). ";
}
 reportFlags(out);
 // Report skill use
 if(skillUse_.size())
{
  out << "Using skills: ";
  bool isFirst = true;
  for (SkillUseIterator iter= skillUse_.begin(); iter != skillUse_.end(); iter++)
  {
    if(!isFirst)
    {
      out << ", ";
    }
    isFirst = false;
    out <<   (*iter)->getSkillUsed()->print()<< " "<<(*iter)->getDaysUsed()  <<" of "<<(*iter)->getDuration();
  }
  out <<". ";
}

 out<<endl;
 out.decr_indent();
}



/*
 * Report unit's specific states and settings
 */
void    UnitEntity::reportFlags(ReportPrinter &out)
{
  TokenEntity::reportFlags(out);
  if(!consuming_) out << " Will not use food for upkeep.";
  if(discontenting_) out << " Discontent.";


}



/*
 * Report unit to foreign observer
 */
void    UnitEntity::publicReport(int observation, ReportPrinter &out)
{
  if(isHidden())
      return;

  if(isDisbanded())
          return;
  if(!isGuarding())
  {
    if (getStealth() > observation)
          return;
  }
  out  << " - "<< print();
  if (!isGuarding()||(getStealth() < observation))
     out  << " of "<< getFaction()->print();
    out<<" "<<raceComposition_->print();
    if (isGuarding())
        out << " on guard";
  if(!inventory_.empty())
    {
      if(raceComposition_->getFigures() > 1)
        inventory_.reportPublicInventory(out,true);
      else
	inventory_.reportPublicInventory(out,false);
    }
	else
	{
            out << ". ";
	}
	
 if(moving_)
    out << moving_->getMode()->getName()<<" to " << moving_->getDestination()->print()
    << " (" << moving_->getTravelTime() - moving_->getRemainingTime() 
    <<" of "<<moving_->getTravelTime() <<" days)" ;

 if(getContainingConstruction())
    out << " inside " << getContainingConstruction()->print();
    out<<endl;
}



/*
 * Prints skills that unit has and skills it may learn
 */
void UnitEntity::reportSkills(FactionEntity * faction, ReportPrinter &out)
{
  skills_.reportAllShort(out);

//  out << "May learn: ";

  RulesIterator skillIter;
  bool isFirst = true;
/*  if (isTraced())
    cout <<"== TRACING " << print()<< " May learn " << endl; */
  for (skillIter = skills.begin(); skillIter != skills.end(); skillIter++)
		{
       SkillRule * skill = dynamic_cast<SkillRule*>(*skillIter);
       if(skill == 0)
        continue;
       if (isTraced())
       {
//          cout <<".";
//          if(skills["airs"] == skill)
//          {
//            cout <<"+";
//          }
       }
          
       if((skill->getRequirement(0) == 0) /*&& (getSkillLevel(skill) == 0)*/)
       continue;

        if(mayStudySkill(skill))
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
		if(mayStudyWithTeacher(skill))
		{
			out<<"(needs teacher)";
		}
              }

     }

    if( !isFirst)
      out << ". ";
}




/*
 * Prints unit's items and equipement, weight and capacity
 */
void UnitEntity::reportInventory(FactionEntity * faction, ReportPrinter &out)
{
  if(!inventory_.empty())
    {
      if(raceComposition_->getFigures() > 1)
            out << " have: ";
      else
            out << " has: ";
			inventory_.reportInventory(faction, out);
    }
	else
	{
            out << ". ";
	}

    int StackWeight=0;
    int weight = getWeight();
    calculateTotalWeight(StackWeight);
    out << "Weight "<<weight; if(weight != StackWeight) out << "("<<StackWeight <<")"; out<<". ";
    for (int movementModeIndex = 0; movementModeIndex < movementModes.size();  movementModeIndex++)
    {
      int StackCapacity=0;
      int capacity = getCapacity(movementModeIndex);
      calculateTotalCapacity(StackCapacity,movementModeIndex);
      if(StackCapacity > 0)
      {
       if(!movementModeIndex)
        out << "Capacity ";
        else
        out <<", "<< (movementModes[movementModeIndex])->getName()<< " ";
        out << capacity; if(StackCapacity != capacity) out << "("<<StackCapacity <<")";
      }
    }
out<<". ";
//   out<<endl;
}



/*
 *
 */
void    UnitEntity::reportAppearence(FactionEntity * faction, ReportPrinter &out)
{
   titles_.printAll(out);

 if(moving_)
	{
    	out <<" "<< moving_->getMode()->getName()<<" to " << moving_->getDestination()->print()
    	<< " (" << moving_->getTravelTime() - moving_->getRemainingTime() <<" of "<<moving_->getTravelTime() 
	<<" days)" ;
	}
else
	{
	out << " at "<< location_->print();
	}

 if(faction == getFaction())
 {
  if(stackFollowingTo_)
    out << " following " << getLeader()->print();
 }
 if(getContainingConstruction())
    out << " inside " << getContainingConstruction()->print();
 out  << endl;
   out<<raceComposition_->print();

}

//===============================================================================
//===============================================================================
//===============================================================================
void UnitEntity::dailyPreProcess()
{
	expose(false);
}


/*
 * Checks data consistency, restores data that were saved for another entity.
 */
void UnitEntity::preprocessData()
{
// Detect location

// Report itself to Faction
	if(faction_)
	faction_-> addUnit(this);
	else
	cerr << "Unassigned faction for unit" << print() <<endl;
    if(raceComposition_ == 0)
		{
			cerr << "Race data are missing for unit " << print() <<endl;
			return;
			}
  if(containingConstruction_)
     containingConstruction_->addUnit(this);
// Set movement flags for all stack
  if((moving_ !=0) && (stackFollowingTo_ == 0))  setStackMoving(moving_);

 recalculateStats();
  TokenEntity::preprocessData();
}



/*
 * Checks data consistency
 */
STATUS UnitEntity::dataConsistencyCheck()
{
  if (location_ ==0)
	{
		cerr << "Incomplete data (no location reference) for " << print() << endl;
		return IO_ERROR;
	}
	faction_->addVisitedLocation(location_);
	 return OK;
}


/*
 * Here unit determines what it will do in the case it has no full-day orders
 */
bool      UnitEntity::defaultAction()
{
  if(isUnaccessible())
    return false;

  if(isBusy())
    return false;

  if(isFullDayOrderFlagSet())
    return false;
  else
  return work();
}
//  ==============================================================
//  ==============================================================
//  ===================  Data access methods  ====================
//  ==============================================================
//  ============================================== ================

/*
 * Returns number of figures in unit
 */
int UnitEntity::getFiguresNumber() const
{
  return raceComposition_->getFigures();
}



/*
 * Defines number of figures in unit
 */
void UnitEntity::setFigures(int number)
{
  raceComposition_->setFigures(number);
  if(number)
  {
    recalculateStats(); //?
  }
  else
    disband();
}



/*
 * Returns race of unit
 */
RaceRule * UnitEntity::getRace() const
{
  return raceComposition_->getRace();
}




/*
 * Returns unit's observation
 */
int UnitEntity::getObservation() const{
return stats.getObservation();
}



/*
 * Returns unit's stealth
 */
int UnitEntity::getStealth() const{
return stats.getStealth();
}



/*
 * Returns unit's weight
 */
int UnitEntity::getWeight()
{
	int sum = raceComposition_ ->getWeight();
	sum += inventory_.getWeight();
	return sum;
}



/*
 * Determines capacity of unit for specified movement mode
 */
int UnitEntity::getCapacity(int modeIndex)
{
    int capacity = raceComposition_->getCapacity(modeIndex);


		capacity += inventory_.getCapacity(modeIndex);


    capacity += skills_.getCapacity(modeIndex);

  //  capacity += location_->getCapacity(modeIndex) * figures_;
  // effects
    capacity += enchantments_.getCapacity(modeIndex, getFiguresNumber());
    return capacity;
}



int UnitEntity::getCapacity(MovementVariety * mode)
{
    int capacity = raceComposition_->getCapacity(mode);
		capacity += inventory_.getCapacity(mode);

    capacity += skills_.getCapacity(mode);

	//  capacity += location_->getCapacity(mode) * figures_;
  // effects
    capacity += enchantments_.getCapacity(mode, getFiguresNumber());
    return capacity;
}





//  ==============================================================
//  ==============================================================
//  ===================   Inventory methods   ====================
//  ==============================================================
//  ============================================== ================
/*
 * Updates number of equiped items after decreasing of number of figures in unit
 */
vector < InventoryElement> UnitEntity::updateEquipement()
{
	int equipMax;
  vector < InventoryElement> unequiped;
  RaceRule * race = raceComposition_->getRace();
  int figures = raceComposition_->getFigures();

  for (EquipSlotIterator iter = race->getAllEquipmentSlots().begin();
                    iter != race-> getAllEquipmentSlots().end(); ++iter)
    {
	 		equipMax = figures * race-> getEquipCapacity((*iter)->type);
			inventory_.updateSlotEquipement((*iter)->type, unequiped,  equipMax);
//     cout << "checking equipment for "<< (*iter)->type->print()<< " max = " << equipMax<<endl;
		}
   return  unequiped;
}



/*
 * Returns amount of money in unit's possesions
 */
int UnitEntity::hasMoney()
{
  // may withdraw cash from faction funds
  return hasItem(cash);
}



/*
 * After  checks for equipability and skills do equip item
 */
int UnitEntity::equipItem(ItemRule * item, int num)
{
    InventoryElement * current =  inventory_.findItem(item);
    if ( current == 0)
            return 0;


  int currentlyEquipedItems = current->getEquipedNumber();
  if (num < currentlyEquipedItems) // unequip
    {
      cout << " num = " << num << " currentlyEquipedItems = "<< currentlyEquipedItems<<endl;
      current->setEquipedNumber(num);
      recalculateStats();
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
	if (num > current->getItemNumber())
			num = current->getItemNumber();

// determine Equipement Max:
	 int equipCapacity = raceComposition_->getFigures() *
	 raceComposition_->getRace()-> getEquipCapacity(item->getEquipSlot());
	 equipCapacity -= inventory_.equipedInSlot(item->getEquipSlot());

    if(equipCapacity >= num - currentlyEquipedItems)
    {
      current->setEquipedNumber(num);
      item->applyEquipementEffects(this,num);
      recalculateStats();
      return (num - currentlyEquipedItems);
    }
    else
    {      
      current->setEquipedNumber(equipCapacity);
      item->applyEquipementEffects(this,equipCapacity);
      recalculateStats();
      return (equipCapacity - currentlyEquipedItems);
    }

}



//  ==============================================================
//  ==============================================================
//  ===================   Stacking methods    ====================
//  ==============================================================
//  ============================================== ================


/*
 * Stacks unit under new leader
 */
void stack(UnitEntity * unit, UnitEntity * newLeader)
{
	if (unit->stackFollowingTo_ == newLeader)
		return;
// Check for stacking loops. If newLeader following unit, unstack newLeader
  UnitEntity * tempLeader = newLeader->getPrevStack();
	int counter = 0 ;
 	while (tempLeader)
		{
		if (tempLeader == unit )
			{
				newLeader->unstack();
				break;
			}
			tempLeader = tempLeader->getPrevStack();
			counter ++;
			if (counter > 10)
				{
					cout << "ERROR: Stack too deep  for unit " <<
					 unit->printTag()<<endl;
					break;
				}
		}


  if (unit->isTraced() || newLeader->isTraced() )
         cout <<"== TRACING " <<unit->printTag()<< " ==> stacks under "<< newLeader->print() <<"\n";
	//move_to_location(stacked, 0);
   unit->stackFollowingTo_ = newLeader;
   newLeader->stackFollowers_.push_back(unit);
   // If unit and it's new leader are in diferent constructions, unit will move to it's new leader
  // construction and will leave it's current construsction if any
   if(newLeader->getContainingConstruction() != unit->getContainingConstruction())
	{
		unit->setContainingConstruction(newLeader->getContainingConstruction());
	}
}



/*
 * Unstacks unit
 */
bool UnitEntity::unstack()
{
	if ( stackFollowingTo_ == 0)
		return false;

	if (isTraced() || stackFollowingTo_->isTraced() )
    cout <<"== TRACING " <<printTag()<< " ==>  unstacks from ";
		cout<< stackFollowingTo_->print() <<"\n";

  this->stackFollowingTo_->stackFollowers_.erase (
				find( stackFollowingTo_->stackFollowers_.begin(),
							stackFollowingTo_->stackFollowers_.end(),
							this )
																									);
	this->stackFollowingTo_ = 0;
  return true;
}



bool UnitEntity::promoteUnit(UnitEntity * unit1,UnitEntity * unit2)
{
  UnitIterator u1 = find(stackFollowers_.begin(), stackFollowers_.end(), unit1);
  UnitIterator u2 = find(stackFollowers_.begin(), stackFollowers_.end(), unit2);
  if(u1 >= u2)
    return false;
  if(u1 == stackFollowers_.end() || u2 == stackFollowers_.end())
    return false;
  stackFollowers_.erase(u2);
  stackFollowers_.insert(u1,unit2);
  return true;
}//==========================================================
// ================== set of recursion methods =============
//==========================================================
int recursionCounter = 0;


/*
 * Unstacks all staying units in the stack
 */
bool UnitEntity::leaveStaying()
{
  if(staying_)
    {
      return  unstack();
    }
	recursionCounter++;
	if ( recursionCounter > 100 )
			{
      	cout << "Recursion overflow in stayStack\n";
        return false;
			}
  UINT i;
	for (i = 0; i <  stackFollowers_.size();)
  {
		if ( !( stackFollowers_[i]-> leaveStaying() ) )
         i++;
    }
	recursionCounter--;
	return false;
}




/*
 * returns list of all units following current unit
 */
vector< UnitEntity *> &  UnitEntity::createStackMembersList(vector< UnitEntity *> & followers)
{
	recursionCounter++;
	if ( recursionCounter > 100 )
			{
      	cout << "Recursion overflow in createStackMembersList\n";
				return followers;
			}
  followers.push_back(this);
	StackIterator iter;
	for (iter = stackFollowers_.begin(); iter !=  stackFollowers_.end(); iter++)
	{
		(*iter)-> createStackMembersList(followers);
	}
	recursionCounter--;
	return followers;
//	followers.insert(followers.end(), stackFollowers_.begin(),stackFollowers_.end());
}




void UnitEntity::printStackMembersList(ReportPrinter &out)
{
	recursionCounter++;
	if ( recursionCounter > 100 )
		{
			cout << "Recursion overflow in printStackMembersList\n";
			return;
		}
	privateReport(out);
	out.incr_indent();
	StackIterator iter;
	for (iter = stackFollowers_.begin(); iter !=  stackFollowers_.end(); iter++)
	{
		(*iter)-> printStackMembersList(out);
	}
	out.decr_indent();
	recursionCounter--;
}


/*
 *
 */
void UnitEntity::moveGroupToLocation()
{
	recursionCounter++;
	if ( recursionCounter > 100 )
			{
      	cout << "Recursion overflow in moveGroupToLocation\n";
				return ;
			}
  moveToLocation();
	StackIterator iter;
	for (iter = stackFollowers_.begin(); iter !=  stackFollowers_.end(); iter++)
	{
		(*iter)-> moveGroupToLocation();
	}
	recursionCounter--;
	return;
}



/*
 *
 */
bool UnitEntity::addStackLandwalkExperience(bool newLevel)
{
	recursionCounter++;
	if ( recursionCounter > 100 )
			{
      	cout << "Recursion overflow in addStackLandwalkExperience\n";
				return newLevel;
			}
  if(addLandwalkExperience())
       newLevel = true;
	StackIterator iter;
	for (iter = stackFollowers_.begin(); iter !=  stackFollowers_.end(); iter++)
	{
		newLevel = (*iter)->addStackLandwalkExperience(newLevel);
	}
	recursionCounter--;
	return newLevel;
}


/*
 *
 */
void UnitEntity::movingGroupArrived()
{
	recursionCounter++;
	if ( recursionCounter > 100 )
			{
      	cout << "Recursion overflow in movingGroupArrived\n";
				return;
			}
  movingEntityArrived();
  StackIterator iter;
	for (iter = stackFollowers_.begin(); iter !=  stackFollowers_.end(); iter++)
	{
		(*iter)->movingGroupArrived();
	}
	recursionCounter--;
}



/*
 *
 */
void UnitEntity::setEntityMoving(TravelElement * moving)
{
   TokenEntity::setEntityMoving(moving);
   unstack();
   setStackMoving(moving);
}




/*
 * Applies all movement effects to all units in stack
 */
void UnitEntity::setStackMoving(TravelElement * moving)
{
	recursionCounter++;
	if ( recursionCounter > 100 )
			{
      	cout << "Recursion overflow in setStackMoving\n";
				return ;
			}

      TokenEntity::setEntityMoving(moving);
      stats.modifyStealth( -1 * raceComposition_->getFigures()/10);

      if(moving_->getMode()== flyingMode)
      {
        stats.modifyStealth(1);
      }
  getLocation()->addReport(
            new BinaryMessage(departPublicReporter, this, moving_->getDestination()), 0 ,
                            ObservationCondition::createObservationCondition(getStealth() )
            );
 if(getLeader())
 {
  addReport(
            new QuintenaryMessage(departFollowerReporter, this, getLocation(),
                moving_->getDestination(), getLeader(),
                new IntegerData(moving_->getTravelTime())   )
            );
 }
 else
 {
  addReport(
            new QuartenaryMessage(departLeaderReporter, this, getLocation(),
                moving_->getDestination(),
                new IntegerData(moving_->getTravelTime())   )
            );
 }
	StackIterator iter;
	for (iter = stackFollowers_.begin(); iter !=  stackFollowers_.end(); iter++)
	{
		(*iter)-> setStackMoving(moving);
	}
	recursionCounter--;
	return;
}



/*
 * recursively adds report to all units in the stack
 */
void UnitEntity::movingGroupReport(ReportRecord report )
{
	recursionCounter++;
	if ( recursionCounter > 100 )
			{
      	cout << "Recursion overflow in movingGroupReport\n";
				return ;
			}
	for (StackIterator iter = stackFollowers_.begin(); iter !=  stackFollowers_.end(); iter++)
	{
		(*iter)->addReport(report);
	}
	recursionCounter--;

}
/*
 * recursively calculates weight of all units in stack
 */
int UnitEntity::calculateTotalWeight (int & weight){
	recursionCounter++;
	if ( recursionCounter > 100 )
			{
      	cout << "Recursion overflow in calculateTotalWeight of "<<getTag() <<"\n";
				return weight;
			}
  weight += getWeight();
	StackIterator iter;
	for (iter = stackFollowers_.begin(); iter !=  stackFollowers_.end(); iter++)
	{
		(*iter)-> calculateTotalWeight(weight);
	}
	recursionCounter--;
	return weight;
}


/*
 * recursively calculates total capacity of the stack
 */
void UnitEntity::calculateTotalCapacity(int & capacity, int modeIndex)
{
	recursionCounter++;
	if ( recursionCounter > 100 )
			{
      	cout << "Recursion overflow in calculateTotalCapacity of "<<getTag() <<"\n";
				return;
			}
     capacity += getCapacity(modeIndex);

	StackIterator iter;
	for (iter = stackFollowers_.begin(); iter !=  stackFollowers_.end(); iter++)
	{
		(*iter)-> calculateTotalCapacity(capacity, modeIndex);
	}
	recursionCounter--;
}


//  ==============================================================
//  ==============================================================
//  ===================     Skill methods     ====================
//  ==============================================================
//  ============================================== ================
/*
 *
 */
void UnitEntity::mergeUnits( int number, UnitEntity * unit = 0)
{
  SkillIterator iter;
  SkillIterator iter2;
  int oldFigures = raceComposition_->getFigures();
  int oldLevel;
  raceComposition_->setFigures(oldFigures + number);
  vector < SkillElement>      oldSkills(skills_.getAll());
  // recalculate skills
  // 1-st step of normalization: multiply experience by old numOfFigures
  for (iter = skills_.getAll().begin(); iter != skills_.getAll().end(); iter++)
		{
     (*iter).setExpPoints( ( (*iter).getExpPoints() *  oldFigures ));
    }

  if(unit)
  {
     setDiscontenting( unit->getDiscontenting()); // copy discontent flag

    for (iter = unit->getAllSkills().begin(); iter != unit->getAllSkills().end(); iter++)
		  {
       if ( (*iter).getSkill()->mayBeStudied(this) !=
                          FOLLOWER_CANNOT_STUDY_SECOND_BASIC_SKILL_FAILURE)
            skills_.addSkillExperience((*iter).getSkill(),(*iter).getExpPoints() *  number);
        else
          {
  	        addReport( new BinaryMessage(mergeSkillReporter, unit, (*iter).getSkill()));
          }
      }
   }

  // last step of normalization:  multiply experience by new numOfFigures
  for (iter = skills_.getAll().begin(); iter != skills_.getAll().end(); iter++)
		{
// Find old level
     oldLevel = 0;

//     cout <<"Merging: before "; (*iter).print(cout);cout<< " and after ";
         for (iter2 = oldSkills.begin(); iter2 != oldSkills.end(); iter2++)
		      {
            if( (*iter2).getSkill() == (*iter2).getSkill())
              {
                oldLevel = (*iter2).getLevel();
                break;
              }
            }

     (*iter).setExpPoints( ( (*iter).getExpPoints() /  (oldFigures + number)));
//     (*iter).print(cout);cout <<endl;
      if( (*iter).getLevel() < oldLevel)
        {
  	        addReport( new BinaryMessage(skillLossReporter, (*iter).getSkill(), new IntegerData((*iter).getLevel()) ));
           (*iter).getSkill()->checkConditions(this);
        }
    }
  recalculateStats();

}



/*
 * Add unskilled figures to unit. Recalculate skill levels
 * and stats
 */
void UnitEntity::addNewFigures(int number)
{
  int oldFigures = raceComposition_->getFigures();
  raceComposition_->setFigures(oldFigures + number);
  // recalculate skills
	skills_.proportionallyDiluteAll(oldFigures,oldFigures + number,this);
  recalculateStats();
}





//bool UnitEntity::hasTeacher(SkillRule  * )
//{
//  // Location teaches skill
//  // or
//  // Teacher present
//  return true;
//}



/*
 * How many seats occupies this entity in the class.
 * Number of entities that can be tought by one teacher determined by this value.
 */
int UnitEntity::getLearningCapacity()
{
  return getRace()->getLearningCapacity() * getFiguresNumber();
}



/*
 * Determines either unit intristicly requires teacher to study a skill
 * due to race specifics
 */
bool UnitEntity::teacherRequired(SkillRule * skill)
{
  return getRace()->teacherRequired(skill, this);
}

/*
 *  Intristic ability of unit to study skill
 *  Level limitations also taken into considerations
 */
LEARNING_RESULT UnitEntity::mayLearn(SkillRule * skill)
{
  return getRace()->mayLearn(skill, this);
}

//===========   Bonuses ================================

/*
 * Calculate ability of unit to study skill above normal
 * level limits
 */
int UnitEntity::getLearningLevelBonus(SkillRule * skill)
{
	int bonus = 0;
	int currentBonus = 0;
// construction
  ConstructionEntity * construction=getContainingConstruction();
	if(construction)
	bonus = construction->getConstructionType()->getLearningBonus(skill);
// Title
	currentBonus = titles_.getLearningBonus(skill);
	if(currentBonus >bonus)
		bonus = currentBonus;
// Item
	currentBonus = inventory_.getLearningBonus(skill);
	if(currentBonus >bonus)
		bonus = currentBonus;

// weather
	LocationEntity * currentLocation = getLocation();
	currentBonus = currentLocation->getWeather()->getLearningBonus(skill);
	if(currentBonus >bonus)
		bonus = currentBonus;
// terrain
	currentBonus = currentLocation->getTerrain()->getLearningBonus(skill);
	if(currentBonus >bonus)
		bonus = currentBonus;
// Race
	currentBonus = getRace()->getLearningBonus(skill);
	if(currentBonus >bonus)
		bonus = currentBonus;
// Enchantment
	currentBonus = enchantments_.getLearningBonus(skill);
	if(currentBonus >bonus)
		bonus = currentBonus;
// Skills
	currentBonus = skills_.getLearningBonus(skill);
	if(currentBonus >bonus)
		bonus = currentBonus;

// Season (?)

  return 0;
}













int UnitEntity::getSkillStudyBonus(SkillRule * skill)
{
	return skills_.getStudyBonus(skill);
}





int UnitEntity::getItemStudyBonus(SkillRule * skill)
{
	return inventory_.getStudyBonus(skill);
}


int UnitEntity::getTitleStudyBonus(SkillRule * skill)
{
 return titles_.getStudyBonus(skill);
}



int UnitEntity::getEnchantmentStudyBonus(SkillRule * skill)
{
	return enchantments_.getStudyBonus(skill);
}




int UnitEntity::getProductionBonus(SkillRule * skill)
{
	int bonus = TokenEntity::getProductionBonus(skill);
// Title
	bonus += titles_.getProductionBonus(skill);
// Race
	 bonus += getRace()->getProductionBonusValue(skill);
// construction
  ConstructionEntity * construction=getContainingConstruction();
	if(construction)
	bonus += construction->getConstructionType()->getProductionBonusValue(skill);
	return bonus;
}





// Titles ========================================================




/*
 * Calculate control points unit costs
 */
int UnitEntity::getControlPoints() const
{

  return  stats.getControlPoints();
}



/*
 * Add title to unit's title collection
 */
void UnitEntity::addTitle(TitleElement * title)
{
  titles_.addTitle(title);
  getFaction()->addMaxControlPoints(title->getTitle()->getControl());
}




void UnitEntity::removeTitle(TitleElement * title)
{
  title->setTitleHolder(0);
  title->desactivateClaimingEffects(); // will clean ownership info

  titles_.removeTitle(title);

  getFaction()->removeMaxControlPoints(title->getTitle()->getControl());

}



bool UnitEntity::claimTitle(TitleElement * title)
{
  title->setTitleHolder(this);
  pay(title->getTitle()->getCost());
  addTitle(title);
  title->activateClaimingEffects();
  return true;
}



bool UnitEntity::mayHoldTitles()
{
  return getRace()->mayHoldTitles();
}



bool UnitEntity::mayCancelTitle(TitleElement * title)
{
 return titles_.mayCancelTitle(title);
}
//  ==============================================================
//  ==============================================================
//  ===================     Other methods     ====================
//  ==============================================================
//  ============================================== ================



/*
 *  Generic interface function for determining rules responsible for
 *  TokenEntity ability to study
 */
Rule * UnitEntity::getType()
{
  return getRace();
}


/*
 * Determines racial type of unit
 */
bool UnitEntity::isOfType(Rule * type)
{
  return getRace()->isDescendantFrom(type);
}



/*
 *  borrows no more than amount of items
 */
int UnitEntity::borrow(ItemRule * item, int amount)
{
  int result = 0;
  int current = 0;
  for (vector <UnitEntity *>::iterator iter = location_->unitsPresent().begin();
                    iter != location_->unitsPresent().end(); ++iter)
  {
    if ((*iter) == this)
        continue;
    if ((*iter)->getFaction() != faction_)
        continue;
    if(!(*iter)->getSharing())
        continue;
    current =  (*iter)->hasItem(item);
    if(current == 0)
        continue;

    if(current  > amount - result)
    {
      current =  amount - result;
    }
    result += current;
    (*iter)->takeFromInventory(item,current);
//QQQ
       addReport(new TertiaryMessage(borrowReporter, this,new ItemElement(item,current),(*iter)));
      (*iter)->addReport(new TertiaryMessage(lendReporter, (*iter),new ItemElement(item,current),this));
	    if (isTraced())
      {
        ItemElement * itemsGiven = new ItemElement(item,current);
      cout <<"== TRACING " <<print()<< " ==> borrows "<< * itemsGiven<< " from "<< (*iter)->print() <<"\n";
         delete itemsGiven;
      }
    if(result >= amount)
      return amount;
  }
      return result;
}




/*
 *  determines how many items (but no more than amount) can borrow unit
 */
int UnitEntity::mayBorrow(ItemRule * item, int amount)
{
  int result = 0;
  for (vector <UnitEntity *>::iterator iter = location_->unitsPresent().begin();
                    iter != location_->unitsPresent().end(); ++iter)
  {

    if ((*iter)->getFaction() != faction_)
        continue;
    if ((*iter) == this)
        continue;
    if(!(*iter)->getSharing())
        continue;
    result +=  (*iter)->hasItem(item);
    if(result >= amount)
      return amount;
  }
      return result;
}



bool UnitEntity::mayPay(int price)
{
  // Try to pay from the pocket money
  int cashAmount = hasItem(cash);
 if(cashAmount >= price)
  return true;
  else
    price -= cashAmount;
 // Try to borrow
    cashAmount = mayBorrow(cash,price);
 if(cashAmount >= price)
  return true;
  else
    price -= cashAmount;

  // Try to withdraw
  if(getWithdrawingSupport())
  {
    return faction_->mayWithdraw(cash,price);
  }
  else
    return false;
}



bool UnitEntity::pay(int price)
{
  // Try to pay from the pocket money
  int cashAmount = takeFromInventory (cash, price);
  if (cashAmount >= price)
  {
    return true;
  }

    price -= cashAmount;
 // Try to borrow
    cashAmount = mayBorrow(cash,price);
    if(cashAmount >= price)
      {
	borrow(cash,price);
        return true;
      }
    else
      price -= cashAmount;

  // Try to withdraw
  if(getWithdrawingSupport())
  {
  cashAmount = faction_->withdraw(cash,price);
	if(cashAmount)
  	addReport(new BinaryMessage(withdrawReporter, this, new ItemElement(cash,cashAmount)));
	if (isTraced() && cashAmount )
    cout <<"== TRACING " <<print()<< " ==> withdraws "<<cashAmount<< "\n";
  if (cashAmount >= price)
  {
    return true;
  }
  }
    return false; // Can't afford to pay
}


// Mana DO NOT need speciat treat. Stats contain max mana and actual mana is in mana_
void UnitEntity::recalculateStats()
{
  stats.clearStats();
 // Race
  stats.addStats(raceComposition_->getRace()->getStats());
 // skills
	skills_.addStats(&stats);
 // items
//  int figures = raceComposition_->getFigures();

		inventory_.addStats(&stats,getFiguresNumber());
 // Effects // may be partial stats?
     enchantments_.addStats(&stats,getFiguresNumber());

 // Building
  if(containingConstruction_ != 0)
  {
//      cout << containingConstruction_->print()<<endl;
      stats.addStats(getContainingConstruction()->getConstructionType()->getBonusStats());
   }
	 // Titles
//	 int controlPoints  = titles_.getControlPointsBonus();
//	 stats.modifyControlPoints(-1 * controlPoints);// Title adds CP to faction
}




bool UnitEntity::isLeading(UnitEntity * unit)
{
	if(unit ==0)
	{
		if(stackFollowers_.size() >0)
			return true;
		else
			return false;
	}
 vector<UnitEntity * >::iterator iter = find(stackFollowers_.begin(),stackFollowers_.end(),unit);
  if(iter != stackFollowers_.end())
   return true;
  else
    return false;
}



bool UnitEntity::isFollowingInStackTo(UnitEntity * unit)
{
	UnitEntity * currentLeader = getLeader();
	while (currentLeader )
	{
		if(currentLeader == unit)
			return true;
		currentLeader = currentLeader->getLeader();
	}
	return false;
}



bool UnitEntity::maySee(TokenEntity * tokenEntity)
{
   if( tokenEntity == 0)
       return false;
   if(tokenEntity->getLocation() != this->getLocation() )
        return false;
   if(tokenEntity->getFaction() == this->getFaction() ) // the same faction
        return true;

//   if(unit->getLeader() == this)
//        return true;
//  if(this->getLeader() == unit)
//        return true;

  if(tokenEntity->isExposed())
        return true;
   if (this->getObservation() >= tokenEntity->getStealth() )
        return true;
   if (this->getLocation()->getFactionalObservation(getFaction()) >= tokenEntity->getStealth() )
        return true;
   else
        return false;
}



bool UnitEntity::mayInterract(UnitEntity * unit)
{
   if( unit == 0)
       return false;
   if(unit->getGlobalLocation() != this->getGlobalLocation() )  // take into account buildings
        return false;
   if(unit->getFaction() == this->getFaction() ) // the same faction
        return true;
   if(unit->getLeader() == this)
        return true;
  if(this->getLeader() == unit)
        return true;
  if(unit->isExposed())
        return true;
   if (this->getObservation() >= unit->getStealth() )
        return true;
   if (this->getLocation()->getFactionalObservation(getFaction()) >= unit->getStealth() )
        return true;
   else
        return false;

}

bool UnitEntity::mayInterract(ConstructionEntity * buildingOrShip)
{
   if( buildingOrShip == 0)
       return false;
   if(buildingOrShip->getLocation() != this->getGlobalLocation() )  // take into account buildings
        return false;
//   if(buildingOrShip->getFaction() == this->getFaction() ) // the same faction
//        return true;
//  if(this->getContainingConstruction() == buildingOrShip)
//        return true;
//   if (buildingOrShip->getStealth() > this->getObservation())
//        return false;
   else
        return true;

}







bool UnitEntity::isExposed() const
{
  return exposeFlag_;
}



void UnitEntity::expose(bool value)
{
  exposeFlag_  = value;
}


/** At the beginning of movement checks conditions*/
/** calculates movement mode and time unstacks*/
/** creates TravelElement */
//MOVEMENT_RESULT UnitEntity::moveDepart(BasicExit * destination)
//{
//   if(!raceComposition_->getRace()->mayMove())
//   {
//      addReport(new BinaryMessage(cantMoveReporter,this,raceComposition_->getRace()) );
//     return RACE_CANNOT_MOVE;
//   }
// // 1. Some races may not be able to move (followers)
//	if (isTraced())
//    cout <<"== TRACING " <<print()<< " ==> Attempts to move\n";
//
// // Can't move while shopping - obsolete
// stayStack();
// // Can't enter unfinished buildings - obsolete
//
// int weight=0;
// int time = 0;
// int totalTravelTime = 999;
// MovementVariety * movingMode = 0;
// MovementMode<int> capacity;
// calculateStackWeight(weight);
// long int i;
// int bestCapacity = 0;
//
// for(i = 0; i < movementModes.size(); i++)
//  {
//   calculateStackCapacity(capacity[i], i);
//   time = destination->getTravelTime(movementModes[i]);
//   if(time == 0)
//    continue;
//    if(capacity[i] > bestCapacity)
//        bestCapacity = capacity[i];
//   time = calculateTravelTime(time , weight, capacity[i]);
//   if(time == 0)
//    continue;
// // 5. Conditions (Skill) may be demanded to enter
//
//    if (time < totalTravelTime)
//        {
//         totalTravelTime = time;
//         movingMode = movementModes[i];
//         }
//  }
//
//  if(movingMode == 0)
//    {
//         // report overload, weight, bestCapacity, set flag
//         return OVERLOAD;
//    }
//   moving_ = new TravelElement(movingMode,location_,destination->getDestination(),
//                                totalTravelTime, totalTravelTime);
//    unstack();
//   setStackMoving(moving_);
//   return MOVEMENT_OK;
//}
//

/** */
void UnitEntity::moveToLocation()
{
	ConstructionEntity * container = getContainingConstruction();
	LocationEntity * destination;
	if(container != 0)
	{
		TravelElement * traveling = container->getTravelStatus();
		if(traveling)
			destination = traveling->getDestination();
		else
			{
			exitConstruction();
			destination = moving_->getDestination();
			}
	}
	if(container == 0)
	{
		destination = moving_->getDestination();
	}
// 	else
// 	{
// 		cerr << print() << " can't exit location "<<endl;
// 	}
	if(destination == 0)
	{
		return;
	}
	TokenEntity::moveToLocation(destination);
  getLocation()->removeUnit(this);
  destination->addUnit(this);
	recalculateTravelTime();

}


void UnitEntity::moveToLocation(LocationEntity * destination)
{
	TokenEntity::moveToLocation(destination);
  getLocation()->removeUnit(this);
  destination->addUnit(this);
	recalculateTravelTime();

}
/** */
bool UnitEntity::moveAdvance()
{
   if(moving_ == 0)
      return false;
   if(stackFollowingTo_)  // not a stack leader
      return false;
   if(patroling_)    // Unit is just patroling. no advancement.
      return false;

   moving_->advance();


   if(moving_->isCrossingBorder())
   {
     tryEnterLocation();
// It may also happen that leader allowed to enter but
// one of followers is not.
// check that without this follower stack may move
// if not - retreat all stack
// if yes - unstack and retreat this follower
   }

   if(addStackLandwalkExperience(false))
   {
        int modeIndex = moving_->getModeIndex();
        if ( modeIndex != -1)
          {
            int capacity = 0;
            int weight;
            calculateTotalWeight(weight);
            calculateTotalCapacity(capacity, modeIndex);
            int oldTime = moving_->getRemainingTime();
            int totalTime = calculateOverloading(moving_->getTravelTime(), weight, capacity);
            int time = calculateOverloading(moving_->getRemainingTime(), weight, capacity);
            moving_->resetRemainingTime(time);
            moving_->resetTravelTime(totalTime - (oldTime - time));
          }
   }

   if(moving_->isArrived())
   {
     movingGroupArrived();
     delete moving_;
     moving_ = 0;
     if(isTraced())                                  //For Debugging only
     {                                               //For Debugging only
       cout <<"--xxx--->  Arrived"<< endl;          //For Debugging only
     }                                              //For Debugging only
     return true;
   }
     return false;

}


void UnitEntity::movingEntityArrived()
{

  getLocation()->addReport(new UnaryMessage(arrivePublicReporter, this), 0 ,
                            ObservationCondition::createObservationCondition(getStealth() ));
  addReport(new UnaryMessage(arrivePrivateReporter, getLocation()));

      stats.modifyStealth( raceComposition_->getFigures()/10);

			MovementVariety * mode;
			ConstructionEntity * construction = getContainingConstruction();

			if(construction)
			{
				mode = construction->getTravelStatus()->getMode();
			}
			else
			{
				mode = moving_->getMode();
			}

			if(mode == flyingMode)
      {
        stats.modifyStealth(-1);
      }

   if(stackFollowingTo_)  // not a stack leader
            moving_ = 0;

   if(isTraced())                                  //For Debugging only
   {                                                      //For Debugging only
     cout <<"--xxx--->  Stack Arrived"<< endl;  //For Debugging only
   }                                                     //For Debugging only
}



bool UnitEntity::addLandwalkExperience()
{
   SkillRule * landWalkSkill = getLocation()->getTerrain()->getLandWalk();
   if(landWalkSkill == 0)
        return false;
   int oldLevel = getSkillLevel(landWalkSkill);
   int newLevel = addSkill(landWalkSkill,BasicLearningStrategy::getPointsPerDay()/10);
   return (oldLevel != newLevel);
}



void UnitEntity::setStaying()
{
  staying_ = true;
}



bool UnitEntity::retreat()
{
  if(moving_)
  {
     moving_->retreat();
     return true;
     }
  else
  return false;
}


void UnitEntity::clearStay()
{
  staying_ = false;

}



bool UnitEntity::isStaying()
{
  return staying_;
}

bool UnitEntity::work()
{
    if( ! raceComposition_->getRace()->mayWork() )
      {
        // report race can't work
        return false;
        }
    RationalNumber amount(location_->getWages() *  getFiguresNumber(), gameConfig.daysInMonth);
    if(amount.getNumenator())
    {
      location_->addMonthlyConflictRequest(new WagesCompetitiveRequest(this,0,amount));
//      cout << print() << " is working at "<< location_->print()<<"\n";
      return true;
    }
    else
    {
//      cout << print() << " can't work at "<< location_->print()<<"\n";
      return false;
    }
}


/** Unit's  can't be destroyed immediatelly because         */
/*  unit should produce it's last report at the end of turn */
/*  also unit's data may be used in reports                 */
void UnitEntity::disband()
{

  location_->removeUnit(this);
	alive_ = false;
  // remove from faction?
  //faction_->removeUnit(this);
  // remove from stack
  // release title!
   titles_.releaseAll(this);

    StackIterator iter;
  vector < UnitEntity *>      currentFollowers = stackFollowers_;
  if (unstack())
  {

	  for (iter = currentFollowers.begin(); iter !=  currentFollowers.end(); iter++)
	  {
	    stack((*iter),stackFollowingTo_);
	  }
  }
  else
  {
	  for (iter = currentFollowers.begin(); iter !=  currentFollowers.end(); iter++)
	  {
	    (*iter)->unstack();
    }
    }
  // RIP
    units.addRIPindex(units.getIndex(tag_));
  // If this is the last unit of the faction - resign it
	faction_->checkAnyUnitsLeft();	
}



void UnitEntity::enterConstruction(ConstructionEntity * containingConstruction)
{
   if(stackFollowingTo_)
   {
      unstack();
   }
   if(containingConstruction_)
      exitConstruction();



   enterStack(containingConstruction);

}


/** Move unit and it's stack (if any) from the construction */
void UnitEntity::exitConstruction()
{
// if stacked unstack
//   ConstructionEntity  * containingConstruction  = getContainingConstruction();
   if(stackFollowingTo_)
   {
      unstack();
   }

   exitStack();
//   containingConstruction->recalculateCapacity();
// recalculate building capacity ?
}



/** Move unit and it's stack (if any) into  construction */
void UnitEntity::enterStack(ConstructionEntity * containingConstruction)
{
	recursionCounter++;
	if ( recursionCounter > 100 )
			{
      	cout << "Recursion overflow in enterStack\n";
				return ;
			}
  if(isGuarding())
  {
    guarding_ = false;
    getLocation()->selectNewGuard();
  }

  containingConstruction_ = containingConstruction;
  containingConstruction_->addUnit(this);
 if(getLeader())
 {
  addReport(new BinaryMessage(followerEnterReporter, containingConstruction_,getLeader()));
 }
 else
 {
  addReport(new UnaryMessage(leaderEnterReporter, containingConstruction_));
 }
  recalculateStats();
	StackIterator iter;
	for (iter = stackFollowers_.begin(); iter !=  stackFollowers_.end(); iter++)
	{
		(*iter)-> enterStack(containingConstruction);
	}
	recursionCounter--;
	return;
}



/** Move unit and it's stack (if any) from the construction */
void UnitEntity::exitStack()
{
	recursionCounter++;
	if ( recursionCounter > 100 )
			{
      	cout << "Recursion overflow in exitStack\n";
				return ;
			}
  ConstructionEntity * containingConstruction = containingConstruction_; // will be overriden otherwise.
  containingConstruction_->removeUnit(this);
  recalculateStats();
 if(getLeader())
 {
  addReport(new BinaryMessage(followerExitReporter, containingConstruction,getLeader()));
 }
 else
 {
    addReport(new UnaryMessage(leaderExitReporter, containingConstruction_));
 }
  containingConstruction_ = 0;
	StackIterator iter;
	for (iter = stackFollowers_.begin(); iter !=  stackFollowers_.end(); iter++)
	{
		(*iter)-> exitStack();
	}
	recursionCounter--;
	return;
}



bool UnitEntity::mayBuild (ConstructionRule * construction)
{
 if( getLocation()->getFreeLand() < construction->getLandUse())
      {
          // report no free land
          return false;
      }

  if( getLocation()->getLandPrice())
      {
          // TBD
          cout << " WARNING: this feature (pay for building land) is not implemented yet\n";
        if( mayInterractFaction( getLocation()->getLegalOwner())) // may interract with owner
           {
            if (! mayPay( getLocation()->getLandPrice()
                                      * construction->getLandUse()))
          // report no money to buy a land
              return false;
            }

        else
          {
            // report owner not present
            return false;
          }

      }
   return true;
}



bool UnitEntity::mayMove()
{
  return getRace()->mayMove(this);
}



bool UnitEntity::isHidden()
{
  if(isAssignedToStaff_)
    return true;
  else
    return false;
}




// Removes all experience in given skill and all it's derivatives
void UnitEntity::forgetSkill(SkillRule * skill)
{
		skills_.forgetSkill(skill,this);
		recalculateStats();
}


// Checks that unit may guard
bool UnitEntity::mayGuard(bool enableReport = true)
{
 if(moving_) // Moving unit can't guard
 {
		if(enableReport) addReport(new SimpleMessage(guardMovesReporter),0,0);
  	return false;
 }
 if(getContainingConstruction()) // unit inside construction can't guard
 {
	if(enableReport) addReport(new SimpleMessage(guardInsideReporter),0,0);
  return false;
	}
 if(getFiguresNumber() < 20 )  // Unit should have at least 20 figures in order to guard
 {
	if(enableReport) addReport(new SimpleMessage(guard20FiguresReporter),0,0);
  return false;
	}
 if(stats.getDamage()  == 0 && stats.getRangedDamage()==0) //unarned unit can't guard
 {
	if(enableReport) addReport(new SimpleMessage(guardWeaponsReporter),0,0);
  return false;
	}
int guardTrashold = getLocation()->getPopulation()/ 100;

 if(/*(getFiguresNumber() < 50) && */(guardTrashold > getFiguresNumber())) // Need 1 soldier for 100 population to guard
 {
	if(enableReport) addReport(new BinaryMessage( guardNotEnoughReporter,
	   new IntegerData(guardTrashold), getLocation()),0,0);
  return false;
	}

 return true;
}



// Damage that unit may do against defenceless opponent (x100)
int UnitEntity::getAttackRating() const
{
  if(stats.getDamage() == 0)
    return 0;
  int att = stats.getMelee();
  if( stats.getMissile() > att)
    att =  stats.getMissile();
  return (40 + att * 4) * (stats.getHits()  + 1) * (stats.getDamage() + stats.getRangedDamage())* getFiguresNumber();
}



// How long unit may stand against melee 5 dmg 2 opponent  (x100)
// (i.e. comb 1, blde 1, sword)
int UnitEntity::getDefenceRating() const
{
  int att =5;
  int def = stats.getDefence();
  if(def)
  {
   if(att>def)
   {
    att = att + 1 -def;
    def = 1;
    }
   else
   {
    def = def + 1 -att;
    att = 1;
   }
  }
  return 1000 * stats.getLife() * getFiguresNumber()* (att + def)/(12 * att);
}

//BattleInstance * UnitEntity::createBattleInstantiation()
//{
//	cout << print() << " creates battle instatiation"<<endl;
//	BattleUnit * battleInstance = new BattleUnit(this);
//	return battleInstance;
//}

/** Climate and other bonuses */
int UnitEntity::calculateMovementBonus(MovementVariety * mode)
{
	int bonus = 0;
	bonus = TokenEntity::calculateMovementBonus(mode);

// Race
	bonus += getRace()->getMovementBonus(mode);
   return bonus;
}


void UnitEntity::sufferDamage(int value)
{
 setFigures(getFiguresNumber() - value);
 updateEquipement();
}



string UnitEntity::printComposition()
{
	return raceComposition_->print();
}


Rule * UnitEntity::getComposition()
{
	return getRace();
}

// "permanent" part of stats
EntityStatistics  UnitEntity::getBasicStats()
{

  EntityStatistics basicStats;
 // Race
  basicStats.addStats(raceComposition_->getRace()->getStats());
 // Effects
     enchantments_.addStats(&basicStats,getFiguresNumber());

 // Building
  if(containingConstruction_ != 0)
  {
//      cout << containingConstruction_->print()<<endl;
      basicStats.addStats(getContainingConstruction()->getConstructionType()->getStats());
   }

return basicStats;
}
