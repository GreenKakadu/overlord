/***************************************************************************
                          UnitEntity.cpp 
                             -------------------
    begin                : Tue Nov 13 18:07:56 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include <algorithm>
#include "UnitEntity.h"
#include "Rational.h"
#include "InventoryElement.h"
#include "RaceElement.h"
#include "SkillLevelElement.h"
#include "TravelElement.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
#include "RaceRule.h"
#include "SkillRule.h"
#include "IntegerData.h"
#include "ItemElementData.h"
#include "BasicExit.h"
#include "ItemElement.h"
#include "GameInfo.h"

#include "PrototypeManager.h"

#include "EntitiesCollection.h"
#include "RulesCollection.h"

#include "ReportRecord.h"
#include "ReportPattern.h"
#include "Reporter.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "TertiaryPattern.h"
#include "QuartenaryPattern.h"
#include "QuintenaryPattern.h"

#include "WagesCompetitiveRequest.h"
#include "ObservationCondition.h"
extern GameInfo game;
extern EntitiesCollection <UnitEntity>      units;
extern EntitiesCollection <FactionEntity>   factions;
extern RulesCollection    <ItemRule>    items;
//extern EntitiesCollection <LocationEntity>  locations;
extern RulesCollection <RaceRule>      races;
// reporters
extern Reporter * withdrawReporter;
extern Reporter * borrowReporter;
extern Reporter * lendReporter;
extern Reporter * newLevelReporter;
extern Reporter * maxLevelReporter;
extern Reporter * cantMoveReporter; //
extern Reporter * departLeaderReporter;
extern Reporter * departFollowerReporter;
extern Reporter * departPublicReporter;
extern Reporter * enterPrivateReporter;
extern Reporter * enterPublicReporter;
extern Reporter * leavePrivateReporter;
extern Reporter * leavePublicReporter;
extern Reporter * arrivePrivateReporter;
extern Reporter * arrivePublicReporter;
extern Reporter * skillLossReporter;
extern Reporter * mergeSkillReporter;

UnitEntity::UnitEntity(const string & keyword, GameData * parent)  : PhysicalEntity(keyword, parent)
{
    keyword_   = keyword;               
    parent_   = parent;               
    prototypeManager->addToRegistry(this);
}

UnitEntity::UnitEntity(const UnitEntity * prototype): PhysicalEntity(prototype)
{
    	faction_ = 0;
      stackFollowingTo_ = 0;
			traced_ = false;
      guarding_ = false;
      staying_  = false;
      moving_ = 0;
      exposeFlag_   = false;
}
UnitEntity * UnitEntity::createUnit (FactionEntity * faction, RaceRule * race,
                  int number, LocationEntity * location)
{
  UnitEntity * newUnit = new UnitEntity(this);
   newUnit->setFaction(faction);
   newUnit->setLocation(location);
   newUnit->setRace(race,number);     
   return newUnit;
}
void UnitEntity::setRace(RaceRule * race, int number)
{
  raceComposition_ = new RaceElement(race,number);
}

GameData * UnitEntity::createInstanceOfSelf()
{
   return CREATE_INSTANCE<UnitEntity> (this);
}

FactionEntity * UnitEntity::getFaction() const
{
return faction_;
}
STATUS 
UnitEntity::initialize        ( Parser *parser )
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
      if (parser->matchKeyword("RACE"))
	{
    raceComposition_ = RaceElement::readElement(parser);
			
	  return OK;
	}
      if (parser->matchKeyword("TRACED"))
	{
	  traced_ = true;
		return OK;
	}
      if (parser->matchKeyword("STACK"))
	{
	  UnitEntity * unit = units[ parser->getWord()];
		if(unit != 0)
				stack (this, unit);
		return OK;
	}
      if (parser->matchKeyword("SKILL"))
	{
    SkillElement * skill = SkillElement::readElement(parser);
    if(skill != 0)
        this->addSkill(*skill);
		return OK;
	}
      if (parser->matchKeyword("MOVING"))
	{
    moving_ = TravelElement::readElement(parser);
		return OK;
	}
       if (parser->matchKeyword("ORDER"))
 	{
	  //          cout << "Reading order..." << endl;  
	  orders_.push_back(new Order(parser->getText(),this));
 	}
       if (parser->matchKeyword("ITEM"))
 	{
    ItemRule * newItem = items[parser->getWord()];
			if(newItem != 0)
					{
						int num = parser->getInteger();
						int equip = parser->getInteger();
            InventoryElement newElement(newItem, num, equip);
	  				inventory_.push_back(newElement);
//	  				inventory_.push_back(InventoryElement(newItem, num, equip));
					}
 	}

	  return OK;

}
void
UnitEntity::save(ostream &out)
{
  if(isDisbanded())
      return;
  
  out << endl;
  out << keyword_ << " " <<tag_ << endl;
  if(!name_.empty()) out << "NAME " <<name_ << endl;
  if(!description_.empty()) out << "DESCRIPTION " <<description_  << endl;
	out << "FACTION " << faction_->getTag()<<endl;
	out << "RACE "; raceComposition_->save(out);
	if(stackFollowingTo_) out << "STACK " << stackFollowingTo_->getTag() << endl;
  if((moving_ !=0) && (stackFollowingTo_ == 0))  moving_->save(out);
  
  vector<Order *>::iterator iter;
  for ( iter = orders_.begin(); iter != orders_.end(); iter++)
    {
           (*iter)->save(out);
    }
  vector<InventoryElement>::iterator iter1;
  for ( iter1 = inventory_.begin(); iter1 != inventory_.end(); iter1++)
    {
           out << "ITEM ";
										(*iter1).save(out);
    }
  vector<SkillElement>::iterator iter2;
  for ( iter2 = skills_.begin(); iter2 != skills_.end(); iter2++)
    {
           out << "SKILL ";
										(*iter2).save(out);
    }
	if (traced_) out << "TRACED" << endl;
}


void UnitEntity::print() // For debugging
{
    cout  << printName()<< " of "<< getFaction()->printTag()<<endl;

  vector<Order *>::iterator iter;
  for ( iter = orders_.begin(); iter != orders_.end(); iter++)
    {
           (*iter)->save(cout);
    }
      
}



void UnitEntity::dailyUpdate()
{
   moveAdvance();
}


void UnitEntity::reportInventory(FactionEntity * faction, ostream &out, int indent = 0)
{
    bool isFirst = true;
    string indentString(indent,' ');
    int wrapPos = out.tellp();  // Wrapping. Temporary here.
  if(!inventory_.empty())
    {
      if(raceComposition_->getFigures() > 1)
            out << " have: ";
      else
            out << " has: ";
	    InventoryElementIterator iter2;
      isFirst = true;
	    for (iter2 = inventory_.begin(); iter2 != inventory_.end(); iter2++)
        {
          faction->addKnowledge((*iter2).getItemType());
          if( isFirst)
            {
              isFirst = false;
            }
          else
            {
              out << ", ";
            }
            if(out.tellp()  > wrapPos + 60) {out<<endl; wrapPos = out.tellp(); out << indentString;} // Wrapping. Temporary here.
          (*iter2).print(out);
        }
    out <<".";
    }
    int StackWeight=0;
    int StackCapacity=0;
    int weight = getWeight();
    int capacity = getCapacity(0);
    calculateStackWeight(StackWeight);
   out << " Weight "<<weight; if(weight != StackWeight) out << "("<<StackWeight <<")"; out<<".";
   calculateStackCapacity(StackCapacity,0);
   out << " Capacity "<< capacity; if(StackCapacity != capacity) out << "("<<StackCapacity <<")"; out<<".";
   out<<endl;
}

void    UnitEntity::reportAppearence(FactionEntity * faction, ostream &out, int indent)
{
 if(moving_)
    out << moving_->getMode()->getName()<<" to " << moving_->getDestination()->printName()
    << " (" << moving_->getTravelTime() - moving_->getRemainingTime() <<" of "<<moving_->getTravelTime() <<" days)" ;
 if(faction == getFaction())
 {
  if(stackFollowingTo_)
    out << " following " << getLeader()->printName();
 }   
 out  << endl;
   out<<raceComposition_->printName();

}

void UnitEntity::report(FactionEntity * faction, ostream &out)
{
    bool isFirst = true;
    int wrapPos;
  out  << printName(); 
    if(isDisbanded())
    {
      out << " (disbanded) "<<endl;
      reportEvents(out);
      out <<endl;
      return;
    }

    else
      out  << " at"<<getLocation()->printName();
  
 if(moving_)
    out << moving_->getMode()->getName()<<" to " << moving_->getDestination()->printName()
    << " (" << moving_->getTravelTime() - moving_->getRemainingTime() <<" of "<<moving_->getTravelTime() <<" days)" ;
 if(stackFollowingTo_)
    out << " following " << getLeader()->printName();
 out  << endl;
   out<<raceComposition_->printName();

   reportInventory(faction, out, 0);

// Skills:

  SkillIterator iter;
  if(!skills_.empty())
  {
    wrapPos = out.tellp();  // Wrapping. Temporary here.
    out << "Skills: ";
    isFirst = true;
	   for (iter = skills_.begin(); iter != skills_.end(); iter++)
		{
        if( isFirst)
                {
                  isFirst = false;
                }
               else
                {
                  out << ", ";
                }
      if(out.tellp()  > wrapPos + 60) {out<<endl; wrapPos = out.tellp();out<<"   ";} // Wrapping. Temporary here.
      (*iter).print(out);
    }
    out <<"."<<endl;
    wrapPos = out.tellp();  // Wrapping. Temporary here.
    
    out << "May learn: ";

    RulesIterator skillIter;
//    RulesCollection <SkillRule>::iterator skillIter;
    isFirst = true;
 	   for (skillIter = skills.begin(); skillIter != skills.end(); skillIter++)
		{
      // Rewrite. Make mayStudy method of UnitEntity.
       SkillRule * skill = dynamic_cast<SkillRule*>(*skillIter);
       if(skill == 0)
        continue;
       if((skill->getRequirement(0) == 0) && (getSkillLevel(skill) == 0))
       continue;

        if( skill->mayStudy(this) == LEARNING_OK)
            {
              faction->addSkillKnowledge(skill, getSkillLevel(skill) + 1); // It may be better placed
                                                                      // in addNewSkill
              if( isFirst)
                {
                  isFirst = false;
                }
               else
                {
                  out << ", ";
                }
                if(out.tellp()  > wrapPos + 60) {out<<endl; wrapPos = out.tellp();out<<"   ";} // Wrapping. Temporary here.
                 skill->printLevel(getSkillLevel(skill) + 1, out);
              }

     }
//    vector <SkillLevelElement *> mayLearn;
// 	   for (iter = skills_.begin(); iter != skills_.end(); iter++)
//		{
//        if( (*iter)
//        (*iter).getAllDerivatives(mayLearn);
//    }
//    vector <SkillLevelElement *>::iterator iter3;
//	   for (iter3 = mayLearn.begin(); iter3 != mayLearn.end(); iter3++)
//		{
//      if( hasSkill(*iter3))
//          continue;
//      if( isFirst)
//        {
//           isFirst = false;
//        }
//      else
//                out << ", ";
//      (*iter3)->print(out);
//
//    }
    out <<"."<<endl;
  }

 recalculateStats();  // Do we really need that?
 stats.print(out);
 out <<endl;
// Posessions
  out << "Events for "<<printName()<<":"<<endl;
 reportEvents(out);
    out <<endl;
}



/** Report unit to foreign observer */
void    UnitEntity::publicReport(int observation, ostream &out)
{
    if(isDisbanded())
          return;
  if (getStealth() > observation)
        return;

  bool isFirst = true;
  out  << " - "<< printName();
  if (getStealth() < observation)
     out  << " of "<< getFaction()->printName();
        
 if(moving_)
    out << moving_->getMode()->getName()<<" to " << moving_->getDestination()->printName()
    << " (" << moving_->getTravelTime() - moving_->getRemainingTime() <<" of "<<moving_->getTravelTime() <<" days)" ;

    out<<endl;  
}



/** No descriptions */
void UnitEntity::preprocessData()
{
// Detect location

// Report itself to Faction
	if(faction_)
	faction_-> addUnit(this);
	else
	cout << "Unassigned faction for unit" << printName() <<endl;
    if(raceComposition_ == 0)
		{
			cout << "Race data are missing for unit" << printName() <<endl;
			}
// Set movement flags for all stack
  if((moving_ !=0) && (stackFollowingTo_ == 0))  setStackMoving(moving_);

 recalculateStats();
}



/** Checks data consistency */
STATUS UnitEntity::dataConsistencyCheck()
{
  if (location_ ==0)
	{
		cout << "Incomplete data (no location reference) for " << printName() << endl;
		return IO_ERROR;
	}
	faction_->addVisitedLocation(location_);
	 return OK;
}


bool      UnitEntity::defaultAction()
{
  if(isUnaccessible())
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

int UnitEntity::getFiguresNumber() const
{
  return raceComposition_->getFigures();
}



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



RaceRule * UnitEntity::getRace() const
{
  return raceComposition_->getRace();
}
  



/** No descriptions */
int UnitEntity::getObservation() const{
return stats.getObservation();
}



/** No descriptions */
int UnitEntity::getStealth() const{
return stats.getStealth();
}



/** Retern reference to Entity which keeps reports from this  */
Entity * UnitEntity::getReportDestination()
{
	return location_;
}



/** No descriptions */
int UnitEntity::getWeight()
{
	int sum = raceComposition_ ->getWeight();
	InventoryElementIterator iter;
	for (iter = inventory_.begin(); iter != inventory_.end(); iter++)
		{
			sum += (*iter).getWeight();
		}
	return sum;
}



int UnitEntity::getCapacity(int modeIndex)
{
    int capacity = raceComposition_->getCapacity(modeIndex);

    InventoryElementIterator invIter;
    for(invIter = inventory_.begin(); invIter != inventory_.end();invIter++)
    {
        capacity += (*invIter).getCapacity(modeIndex);
      }

  SkillIterator skillIter;
    for(skillIter = skills_.begin(); skillIter != skills_.end();skillIter++)
    {
        capacity += (*skillIter).getCapacity(modeIndex);
      }
  //  capacity += location_->getCapacity(modeIndex) * figures_;
  // effects
    return capacity;
}



/** Unit may be unaccessible for orders for some reasons */
bool UnitEntity::isUnaccessible() const
{
  if(moving_)
      return true;
  if(location_ == 0)    
      return true;
  else
    return false;    
}

//  ==============================================================
//  ==============================================================
//  ===================   Inventory methods   ====================
//  ==============================================================
//  ============================================== ================
vector < InventoryElement > & UnitEntity::getAllInventory()
{
  return inventory_;
}

void UnitEntity::giveAllInventory(UnitEntity * unit)
{
  InventoryElementIterator iter;
  for (iter = inventory_.begin(); iter != inventory_.end(); ++iter)
  {
    unit->addToInventory((*iter).getItemType(), (*iter).getItemNumber() );
    inventory_.clear();
    }
}



/** After  checks for equipability and skills do equip item */
int UnitEntity::equipItem(ItemRule * item, int num)
{
	InventoryElement  newItem (item, num);
  InventoryElementIterator iterEquip;
 	InventoryElementIterator iter = find(inventory_.begin(), inventory_.end(), newItem);
	if ( iter == inventory_.end())
				return 0;
int currentlyEquipedItems = (*iter).getEquipedNumber(); 																				;
	if (num <= currentlyEquipedItems) // unequip
			{
				(*iter).setEquipedNumber(num);
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
	if (num > (*iter).getItemNumber())
			num = (*iter).getItemNumber();

// determine Equipement Max:
	int equipCapacity = raceComposition_->getFigures() * raceComposition_->getRace()-> getEquipCapacity(item->getEquipSlot());
  for ( iterEquip = inventory_.begin(); iterEquip != inventory_.end(); iterEquip++)
    {
	     if( (*iterEquip).getItemType()-> getEquipSlot() == item->getEquipSlot())
				{
//          cout<< "======================" << (*iterEquip).getItemType()->printName()<< " "<< (*iterEquip).getEquipedNumber()<< " "<< (*iterEquip).getItemType()-> getNumEquipSlotsRequired()<<endl;
          equipCapacity -= (*iterEquip).getEquipedNumber() * (*iterEquip).getItemType()-> getNumEquipSlotsRequired();
				}
		}
			if(equipCapacity >= num)
					{
						(*iter).setEquipedNumber(num);
							//		        applyEquipementEffects()
						return (num - currentlyEquipedItems);
						}
			else
						{
						(*iter).setEquipedNumber(equipCapacity);
						// 		        applyEquipementEffects()
						return (equipCapacity - currentlyEquipedItems);
						}

}



/** Updates number of equiped items after decreasing of number of figures in unit */
vector < InventoryElement > UnitEntity::updateEquipement()
{
	EquipSlotIterator iter;
  InventoryElementIterator iterEquip;
	int equipMax;
	int currentEquip;
  vector < InventoryElement > unequiped;
  RaceRule * race = raceComposition_->getRace();
  int figures = raceComposition_->getFigures();
  int unequippedNum; 
  for ( iter = race->getAllEquipmentSlots().begin(); iter != race-> getAllEquipmentSlots().end(); ++iter)
    {
	 		equipMax = figures * race-> getEquipCapacity((*iter)->type);
//     cout << "checking equipment for "<< (*iter)->type->printName()<< " max = " << equipMax<<endl;
  		for ( iterEquip = inventory_.begin(); iterEquip != inventory_.end(); ++iterEquip)
    		{
				  currentEquip = (*iterEquip).getEquipedNumber() * (*iterEquip).getItemType()-> getNumEquipSlotsRequired();
	     		if( (*iterEquip).getItemType()-> getEquipSlot() == (*iter)->type)
						{
//     cout << "------- "<< (*iterEquip).getItemType()->printName()<< " slots left "<< equipMax<<endl;
							if(equipMax == 0)  // no more slots
              {
                unequippedNum = currentEquip / ((*iterEquip).getItemType()-> getNumEquipSlotsRequired());
                 (*iterEquip).setEquipedNumber(0);
                 if(unequippedNum)
                 {
//     cout << "------- unequips "<< unequippedNum << "  ."<<endl;
                  unequiped.push_back(InventoryElement( (*iterEquip).getItemType() , unequippedNum ) ) ;
                  }
              }
//     cout << "------- may use "<< currentEquip<< "  slots."<<endl;
							if(currentEquip <= equipMax) // enough space
								{
									equipMax -= currentEquip;
								}
							else // not enough space for all. only part will be equiped
								{
									(*iterEquip).setEquipedNumber (equipMax/ (*iterEquip).getItemType()-> getNumEquipSlotsRequired());
                  unequippedNum = (currentEquip - equipMax) / ((*iterEquip).getItemType()-> getNumEquipSlotsRequired());
									equipMax = 0 ;
//     cout << "------- unequips "<< unequippedNum << "  ."<<endl;
                  unequiped.push_back(InventoryElement( (*iterEquip).getItemType() , unequippedNum ) ) ;
								}
						}
				}
		}
   return  unequiped;
}



/** Takes up to num items from inventory. Returns number of items taken */
int UnitEntity::takeFromInventory(ItemRule * item, int num)
{
	InventoryElement  newItem (item, num);
 	InventoryElementIterator iter = find(inventory_.begin(), inventory_.end(), newItem);
	if(iter != inventory_.end())
			{
        int numItems = (*iter).getItemNumber();
        int numToGive = num;
				if(numItems > numToGive)
						{
							numItems -= numToGive;
							(*iter).setItemNumber(numItems);
              if((*iter).getEquipedNumber() > numItems )
                  {
									  (*iter).setEquipedNumber(numItems);
                    recalculateStats();
                  }
							return numToGive;
						}
				else
						{
							int taken = numItems;
              int wasEquiped = (*iter).getEquipedNumber();
							inventory_.erase(iter);
              if(wasEquiped >0)
              recalculateStats();
							return taken;
						}
			}
	else
		return 0;
}


/** Takes exactly num items from inventory. Returns true if inventory had num items and false othervise  */

bool UnitEntity::takeFromInventoryExactly(ItemRule * item, int num)
{
	InventoryElement  newItem (item, num);
 	InventoryElementIterator iter = find(inventory_.begin(), inventory_.end(), newItem);
	if(iter != inventory_.end())
			{
        int numItems = (*iter).getItemNumber();
        int numToGive = num;
				if(numItems > numToGive)
						{
							numItems -= numToGive;
							(*iter).setItemNumber(numItems);
              if((*iter).getEquipedNumber() > numItems )
                  {
									  (*iter).setEquipedNumber(numItems);
                    recalculateStats();
                  }
							return true;
						}
				else if (numItems == numToGive)
          {
              int wasEquiped = (*iter).getEquipedNumber();
							inventory_.erase(iter);
              if(wasEquiped >0)
              recalculateStats();
							return true;
          }
        else
						{
							return false;
						}
			}
	else
		return false;
}

int UnitEntity::hasMoney()
{
  // may withdraw cash from faction funds
  return hasItem(cash);
}


/** No descriptions */
void UnitEntity::addToInventory(ItemRule * item, Rational& num)  // should be done better
{
InventoryElementIterator iter;
//cout << printName()<< " adding " <<item->printName() <<endl;
for(iter = inventory_.begin(); iter != inventory_.end();iter++)
{
 if ( (*iter).getItemType() == item)
      {
        Rational old((*iter).getRationalItemNumber());
        (*iter).setRationalItemNumber(old + num);
          return;
      }
}      
  InventoryElement  newItem (item, 1);
  newItem.setRationalItemNumber(num);
  inventory_.push_back(newItem);
}
//	InventoryElement  newItem (item, num);
// 	InventoryElementIterator iter = find(inventory_.begin(), inventory_.end(), newItem);
//	if(iter != inventory_.end())
//			(*iter).setItemNumber((*iter).getItemNumber() + num);
//	else
//	 inventory_.push_back(newItem);




void UnitEntity::addToInventory(ItemRule * item, int num)  // should be done better
{
  Rational number(num);
  addToInventory(item, number);
}
//struct isItem_:binary_function<ItemRule *,ItemRule *, bool>
//{
//  bool operator() (const ItemRule * item,const ItemRule * item) const;
//}
//  bool isItem_::operator() (const ItemRule * item) const
//{
// if(inventory_
//}
/** No descriptions */
int UnitEntity::hasItem(ItemRule * item)
{
	InventoryElement  newItem (item, 1);
 	InventoryElementIterator iter = find(inventory_.begin(), inventory_.end(), newItem);
	if(iter != inventory_.end())
		return (*iter).getItemNumber();
	else
     return 0;
}
int UnitEntity::hasEquiped(ItemRule * item)
{
	InventoryElement  newItem (item, 1);
 	InventoryElementIterator iter = find(inventory_.begin(), inventory_.end(), newItem);
	if(iter != inventory_.end())
		return (*iter).getEquipedNumber();
	else
     return 0;
}
Rational     UnitEntity::getItemAmount(ItemRule * item) 
{
	InventoryElement  newItem (item, 1);
 	InventoryElementIterator iter = find(inventory_.begin(), inventory_.end(), newItem);
	if(iter != inventory_.end())
		return (*iter).getRationalItemNumber();
	else
     return 0;
}



//  ==============================================================
//  ==============================================================
//  ===================   Stacking methods    ====================
//  ==============================================================
//  ============================================== ================


/** Stacks unit under new leader */
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
							cout << "ERROR: Stack too deep  for unit " << unit->printTag()<<endl;
							break;
						}
			}


	if (unit->isTraced() || newLeader->isTraced() )
    cout <<"== TRACING " <<unit->printTag()<< " ==> stacks under "<< newLeader->printName() <<"\n";
	//move_to_location(stacked, 0);
	 unit->stackFollowingTo_ = newLeader;
   newLeader->stackFollowers_.push_back(unit);
}



/** Unstacks unit */
bool UnitEntity::unstack()
{
	if ( this->stackFollowingTo_ == 0)
		return false;

	if (this->isTraced() || this->stackFollowingTo_->isTraced() )
    cout <<"== TRACING " <<this->printTag()<< " ==>  unstacks from "<< this->stackFollowingTo_->printName() <<"\n";

  this->stackFollowingTo_->stackFollowers_.erase (
				find( this->stackFollowingTo_->stackFollowers_.begin(),
							this->stackFollowingTo_->stackFollowers_.end(),
							this )
																									);
	this->stackFollowingTo_ = 0;
  return true;
}
//==========================================================
// ================== set of recursion methods =============
//==========================================================
int recursionCounter = 0;


bool UnitEntity::stayStack()
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
		if ( !( stackFollowers_[i]-> stayStack() ) )
         i++;
    }
	recursionCounter--;
	return false;
}




/** returns list of all units following current unit */
vector< UnitEntity *> &  UnitEntity::createStackMembersList(vector< UnitEntity *> & followers)
{
	recursionCounter++;
	if ( recursionCounter > 100 )
			{
      	cout << "Recursion overflow \n";
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



void UnitEntity::moveStackToLocation()
{
	recursionCounter++;
	if ( recursionCounter > 100 )
			{
      	cout << "Recursion overflow \n";
				return ;
			}
  moveUnitToLocation();
	StackIterator iter;
	for (iter = stackFollowers_.begin(); iter !=  stackFollowers_.end(); iter++)
	{
		(*iter)-> moveStackToLocation();
	}
	recursionCounter--;
	return;
}



bool UnitEntity::addStackLandwalkExperience(bool newLevel)
{
	recursionCounter++;
	if ( recursionCounter > 100 )
			{
      	cout << "Recursion overflow \n";
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


void UnitEntity::movingStackArrived()
{
	recursionCounter++;
	if ( recursionCounter > 100 )
			{
      	cout << "Recursion overflow \n";
				return;
			}
  movingUnitArrived();
  StackIterator iter;
	for (iter = stackFollowers_.begin(); iter !=  stackFollowers_.end(); iter++)
	{
		(*iter)->movingStackArrived();
	}
	recursionCounter--;
}



/** Applies all movement effects to all units in stack */
void UnitEntity::setStackMoving(TravelElement * moving)
{
	recursionCounter++;
	if ( recursionCounter > 100 )
			{
      	cout << "Recursion overflow \n";
				return ;
			}

      moving_ = moving;
      stats.modifyStealth( -1 * raceComposition_->getFigures()/10);
      
      if(moving_->getMode()== flyingMode)
      {
        stats.modifyStealth(1);
      }
  getLocation()->addReport(
            new BinaryPattern(departPublicReporter, this, moving_->getDestination()), 0 ,
                            ObservationCondition::createObservationCondition(getStealth() )
            );
 if(getLeader())
 {
  addReport(
            new QuintenaryPattern(departFollowerReporter, this, getLocation(),
                moving_->getDestination(), getLeader(),
                new IntegerData(moving_->getTravelTime())   )
            );
 }
 else
 {
  addReport(
            new QuartenaryPattern(departLeaderReporter, this, getLocation(),
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



/** recursively calculates weight of all units in stack */
int UnitEntity::calculateStackWeight (int & weight){
	recursionCounter++;
	if ( recursionCounter > 100 )
			{
      	cout << "Recursion overflow \n";
				return weight;
			}
  weight += getWeight();
	StackIterator iter;
	for (iter = stackFollowers_.begin(); iter !=  stackFollowers_.end(); iter++)
	{
		(*iter)-> calculateStackWeight(weight);
	}
	recursionCounter--;
	return weight;
}


/** recursively calculates total capacity of the stack */
void UnitEntity::calculateStackCapacity(int & capacity, int modeIndex)
{
	recursionCounter++;
	if ( recursionCounter > 100 )
			{
      	cout << "Recursion overflow \n";
				return;
			}
     capacity += getCapacity(modeIndex);
  
	StackIterator iter;
	for (iter = stackFollowers_.begin(); iter !=  stackFollowers_.end(); iter++)
	{
		(*iter)-> calculateStackCapacity(capacity, modeIndex);
	}
	recursionCounter--;
}


//  ==============================================================
//  ==============================================================
//  ===================     Skill methods     ====================
//  ==============================================================
//  ============================================== ================
vector < SkillElement>& UnitEntity::getAllSkills()
{
  return skills_;
}



void UnitEntity::mergeUnits( int number, UnitEntity * unit = 0)
{
  SkillIterator iter;
  SkillIterator iter2;
  int oldFigures = raceComposition_->getFigures();
  int oldLevel;
  raceComposition_->setFigures(oldFigures + number);
  vector < SkillElement>      oldSkills(skills_);
  // recalculate skills
  // 1-st step of normalization: multiply experience by old numOfFigures
  for (iter = skills_.begin(); iter != skills_.end(); iter++)
		{
     (*iter).setExpPoints( ( (*iter).getExpPoints() *  oldFigures ));
    }

  if(unit)
  {
    for (iter = unit->getAllSkills().begin(); iter != unit->getAllSkills().end(); iter++)
		  {
       if ( (*iter).getSkill()->mayStudy(this) !=
                          FOLLOWER_CANNOT_STUDY_SECOND_BASIC_SKILL_FAILURE)
            addSkillExperience((*iter).getSkill(),(*iter).getExpPoints() *  number);
        else
          { 
  	        addReport( new   ReportRecord(new BinaryPattern(mergeSkillReporter, unit, (*iter).getSkill())));
          }                 
      }
   }

  // last step of normalization:  multiply experience by new numOfFigures
  for (iter = skills_.begin(); iter != skills_.end(); iter++)
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
  	        addReport( new   ReportRecord(new BinaryPattern(skillLossReporter, (*iter).getSkill(), new IntegerData((*iter).getLevel()) )));
           (*iter).getSkill()->checkConditions(this);
        }
    }
  recalculateStats();

}



void UnitEntity::addNewFigures(int number)
{
  int oldFigures = raceComposition_->getFigures();
  int oldLevel;
  raceComposition_->setFigures(oldFigures + number);
  // recalculate skills
  SkillIterator iter;
	for (iter = skills_.begin(); iter != skills_.end(); iter++)
		{
      oldLevel = (*iter).getLevel();
      (*iter).setExpPoints( ( (*iter).getExpPoints() *  oldFigures ) / (oldFigures + number));
      if( (*iter).getLevel() < oldLevel)
        {
  	        addReport( new   ReportRecord(new BinaryPattern(skillLossReporter, (*iter).getSkill(), new IntegerData((*iter).getLevel()) )));
           (*iter).getSkill()->checkConditions(this);
        }
		}
  recalculateStats();  
}


/** No descriptions */
bool UnitEntity::hasSkill(SkillRule  * skill, int level)
{
  if(( skill == 0) || ( level == 0) )
    return true;
  SkillIterator iter;
	for (iter = skills_.begin(); iter != skills_.end(); iter++)
		{
			if ((skill == (*iter).getSkill()) && (level <= (*iter).getExpPoints()))
				return true;
		}

	return false;

}

/** Adding skill experience without checking  */
void  UnitEntity::addSkillExperience(SkillRule  * skill, int expPoints)
{
	SkillIterator iter;
  for ( iter = skills_.begin(); iter != skills_.end(); iter++)
		{
					if( (*iter).getSkill() == skill)
								{
									 (*iter).setExpPoints((*iter).getExpPoints() + expPoints);
								  break;
								}
		}
    if (iter == skills_.end())
	      skills_.push_back(SkillElement(skill,expPoints));
}

/** Adding skill experience, checking for new level reached */
int UnitEntity::addSkill(SkillRule  * skill, int expPoints)
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

//  UnitEntity::newLevelReporter_ = new Reporter(""," reached level "," in ","");



void UnitEntity::gainNewLevel(SkillRule * skill, int newLevel)
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



//bool UnitEntity::hasTeacher(SkillRule  * )
//{
//  // Location teaches skill
//  // or
//  // Teacher present
//  return true;
//}



int UnitEntity::getSkillLevel(SkillRule  * const skill)
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

int UnitEntity::getSkillPoints(SkillRule  * const skill)
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


int UnitEntity::addSkill(SkillElement  skill)
{
  return addSkill(skill.getSkill(), skill.getExpPoints());
}


/** How many seats occupies this entity in the class. Number of entities that can be tought by one teacher determined by this value. */
int UnitEntity::getLearningCapacity()
{
  return getRace()->getLearningCapacity() * getFiguresNumber();
}



bool UnitEntity::teacherRequired(SkillRule * skill) 
{
  return getRace()->teacherRequired(skill, this);
}

//  ==============================================================
//  ==============================================================
//  ===================     Other methods     ====================
//  ==============================================================
//  ============================================== ================



bool UnitEntity::mayPay(int price)
{
//  ItemRule * cash = items["coin"];
  // Try to pay from the pocket money
  int cashAmount = hasItem(cash);
 if(cashAmount >= price)
  return true;
  else
    price -= cashAmount;
 // Try to borrow
 vector <UnitEntity *>::iterator iter;
  for (iter = location_->unitsPresent().begin(); iter != location_->unitsPresent().end(); iter++)
  {
    if ((*iter)->getFaction() != faction_)
        continue;
    cashAmount = (*iter)->hasItem(cash);
    if(cashAmount >= price)
      return true;
    else
      price -= cashAmount;
  }
  // Try to withdraw
  return faction_->mayWithdraw(cash,price);
}



bool UnitEntity::pay(int price)
{
//  ItemRule * cash = items["coin"];
  // Try to pay from the pocket money
  int cashAmount = takeFromInventory (cash, price);
  if (cashAmount >= price)
  {
    return true;
  }
  else
    price -= cashAmount;
 // Try to borrow
 vector <UnitEntity *>::iterator iter;
  for (iter = location_->unitsPresent().begin(); iter != location_->unitsPresent().end(); iter++)
  {
//    cout << printName()<<" is trying to borrow from " << (*iter)->printName() << " at " << location_->printName()<<endl;
    if((*iter) == this)  // can't borrow from itself
        continue;
    
    if ((*iter)->getFaction() != faction_)
        continue;
    cashAmount = (*iter)->takeFromInventory (cash, price);
    if(cashAmount)
    {
      addReport(new TertiaryPattern(borrowReporter, this,new ItemElementData(cash,cashAmount),(*iter)));
      (*iter)->addReport(new TertiaryPattern(lendReporter, (*iter),new ItemElementData(cash,cashAmount),this));
	    if (isTraced())
      cout <<"== TRACING " <<printName()<< " ==> borrows "<<cashAmount<< " from "<< (*iter)->printName() <<"\n";
    }
    if(cashAmount >= price)
      {
        return true;
      }
    else
      price -= cashAmount;
  }
  // Try to withdraw
  cashAmount = faction_->withdraw(cash,price);
  addReport(new BinaryPattern(withdrawReporter, this,new ItemElementData(cash,cashAmount)));
	if (isTraced() && cashAmount )
    cout <<"== TRACING " <<printName()<< " ==> withdraws "<<cashAmount<< "\n";
  if (cashAmount >= price)
  {
    return true;
  }
    return false; // Can't afford to pay 
}



void UnitEntity::recalculateStats()
{
	SkillIterator iter;
 // Recalculate
  stats.clearStats();
  stats.addStats(raceComposition_->getRace()->getStats());
	for (iter = skills_.begin(); iter != skills_.end(); iter++)
		{
      stats.addStats((*iter).getStats());
  }
 // items
	InventoryElementIterator iter2;
  int figures = raceComposition_->getFigures();
	for (iter2 = inventory_.begin(); iter2 != inventory_.end(); iter2++)
		{
      stats.addPartialStats( (*iter2).getStats(), (*iter2).getEquipedNumber(), figures);      
    }
}



bool UnitEntity::isAccepting(UnitEntity * unit)
{
  return (find(accepting_.begin(),accepting_.end(),unit) != accepting_.end());
}



void UnitEntity::clearAccept(UnitEntity * unit)
{
 vector<UnitEntity * >::iterator iter = find(accepting_.begin(),accepting_.end(),unit);
  if(iter != accepting_.end())
      accepting_.erase(iter);
}



void UnitEntity::accept(UnitEntity * unit)
{
 vector<UnitEntity * >::iterator iter = find(accepting_.begin(),accepting_.end(),unit);
  if(iter == accepting_.end())
  accepting_.push_back(unit);
}



bool UnitEntity::isLeading(UnitEntity * unit)
{
 vector<UnitEntity * >::iterator iter = find(stackFollowers_.begin(),stackFollowers_.end(),unit);
  if(iter != stackFollowers_.end())
   return true;
  else
    return false; 
}



LocationEntity * UnitEntity::getGlobalLocation() const // will be changed with introduction of buildings
{
  if(moving_)
    return 0;
  else
    return location_;
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
   if (unit->getStealth() > this->getObservation())
        return false;
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
//      addReport(new BinaryPattern(cantMoveReporter,this,raceComposition_->getRace()) );
//     return RACE_CANNOT_MOVE;
//   }
// // 1. Some races may not be able to move (followers)
//	if (isTraced())
//    cout <<"== TRACING " <<printName()<< " ==> Attempts to move\n";
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

/** Climate and overloading effects */
int UnitEntity::calculateTravelTime(int time, int weight, int capacity)
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


/** */
void UnitEntity::moveUnitToLocation()
{
  getLocation()->addReport(new UnaryPattern(leavePublicReporter, this), 0 ,
                            ObservationCondition::createObservationCondition(getStealth() ));
  addReport(new UnaryPattern(leavePrivateReporter, getLocation()));
      getLocation()->removeUnit(this);
      moving_->getDestination()->addUnit(this);
	if (isTraced())
    cout <<"== TRACING " <<printName()<< " ==> Enters "<< moving_->getDestination()->printName()<<"\n";
  getLocation()->addReport(new UnaryPattern(enterPublicReporter, this), 0 ,
                            ObservationCondition::createObservationCondition(getStealth() ));
  addReport(new UnaryPattern(enterPrivateReporter, getLocation()));
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
     // check if stack can enter location (it may be guarded or other reasons)    
     moveStackToLocation(); 
   }

   if(addStackLandwalkExperience(false))
   {
        int modeIndex = moving_->getModeIndex();
        if ( modeIndex != -1)
          {
            int capacity = 0;
            int weight;
            calculateStackWeight(weight);
            calculateStackCapacity(capacity, modeIndex);
            int oldTime = moving_->getRemainingTime();
            int totalTime = calculateTravelTime(moving_->getTravelTime(), weight, capacity);
            int time = calculateTravelTime(moving_->getRemainingTime(), weight, capacity);
            moving_->resetRemainingTime(time);
            moving_->resetTravelTime(totalTime - (oldTime - time));
          }
   }
   
   if(moving_->isArrived())
   {
     movingStackArrived();
     delete moving_;
     moving_ = 0;
     return true;
   }
     return false;

}


void UnitEntity::movingUnitArrived()
{

  getLocation()->addReport(new UnaryPattern(arrivePublicReporter, this), 0 ,
                            ObservationCondition::createObservationCondition(getStealth() ));
  addReport(new UnaryPattern(arrivePrivateReporter, getLocation()));

      stats.modifyStealth( raceComposition_->getFigures()/10);

      if(moving_->getMode()== flyingMode)
      {
        stats.modifyStealth(-1);
      }

   if(stackFollowingTo_)  // not a stack leader
            moving_ = 0;
  
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



void UnitEntity::stay()
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
    Rational amount(location_->getWages() *  getFiguresNumber(), game.daysInMonth);
    if(amount.getValue())
    {
      location_->addMonthlyConflictRequest(new WagesCompetitiveRequest(this,0,amount));
//      cout << printName() << " is working at "<< location_->printName()<<"\n";
      return true;
    }
    else
    {
      cout << printName() << " can't work at "<< location_->printName()<<"\n";
      return false;
    }
}


/** Unit is not exist anymore (dead or disbanded) */
bool UnitEntity::isDisbanded()
{
  return (location_ == 0);
}
/** Unit's  can't be destroyed immediatelly because         */
/*  unit should produce it's last report at the end of turn */
/*  also unit's data may be used in reports                 */
void UnitEntity::disband()
{
  // remove from location
  location_->removeUnit(this);
  // remove from faction?
  //faction_->removeUnit(this);
  // remove from stack
	  StackIterator iter;
  if (unstack())
  {
	  for (iter = stackFollowers_.begin(); iter !=  stackFollowers_.end(); iter++)
	  {
	    stack((*iter),stackFollowingTo_);       
	  }
  }
  else
  {
	  for (iter = stackFollowers_.begin(); iter !=  stackFollowers_.end(); iter++)
	  {
	    (*iter)->unstack();
    }
    }
  // RIP
    units.addRIPindex(units.getIndex(tag_));
}

