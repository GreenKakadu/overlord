/***************************************************************************
                          InventoryAttribute.cpp
                             -------------------
    begin                : Sun Apr 03 2005
    copyright            : (C) 2005 by Alex Dribin
     email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "InventoryAttribute.h"
#include "TokenEntity.h"
#include "FactionEntity.h"
#include "EntityStatistics.h"
#include "BasicCondition.h"
#include "BinaryMessage.h"
extern ReportPattern *	unequipReporter;

InventoryAttribute::InventoryAttribute(TokenEntity * entity)
{
    entity_ = entity;
    if (entity == 0)
        cerr << "creating InventoryAttribute with empty Entity" << endl;
}

InventoryAttribute::InventoryAttribute(InventoryAttribute & p)
{
inventory_ = p.inventory_;
entity_ = p.entity_;
}

void InventoryAttribute::init(TokenEntity * entity)
{
    entity_ = entity;
    if (entity == 0)
        cerr << "creating InventoryAttribute with empty Entity" << endl;
}

InventoryAttribute::~InventoryAttribute()
{
}


// InventoryAttribute::InventoryAttribute(vector < InventoryElement> &inventory)
// {
// 	inventory_ = inventory;
// 
// //	for(InventoryIterator iter = inventory.getAll().begin();
// //			iter != inventory.getAll().end();	 ++iter)
// //  {
// //		inventory_.push_back(*iter);
// //  }
// 
// }

STATUS
InventoryAttribute::initialize        ( Parser *parser )
{

 if (parser->matchKeyword("ITEM"))
 	{
    InventoryElement * newInventory  = InventoryElement::readElement(parser);
    if(newInventory != 0)
	  	  inventory_.push_back(*newInventory);
 	}
  return OK;
}

void InventoryAttribute::save(ostream &out)
{
    for (InventoryIterator iter = inventory_.begin(); iter != inventory_.end(); iter++)
    {
        out << "ITEM ";
        (*iter).save(out);
    }
}

void InventoryAttribute::save(ostream &out, string prefix)
{
    for (InventoryIterator iter = inventory_.begin(); iter != inventory_.end(); iter++)
    {
        out <<prefix<< "ITEM ";
        (*iter).save(out);
    }
}



ostream&  InventoryAttribute::report(ostream &out)
{
  return out;
}

// Inventory methods ==============================================
InventoryElement * InventoryAttribute::findItem(ItemRule * item)
{
  InventoryIterator iter;
  for( iter = inventory_.begin(); iter != inventory_.end(); ++iter)
  {
    if( (*iter).getItemType() == item)
       return &(*iter);
  }
  return 0;
}


// returns list of all items equipped at given slot
vector < InventoryElement > InventoryAttribute::getSlotContent(EquipmentSlotVariety * slot)
{
  vector < InventoryElement > slotInventory;
	InventoryIterator iter;
  for( iter = inventory_.begin(); iter != inventory_.end(); ++iter)
  {
    if(( (*iter).getItemType()->getEquipSlot() == slot)
		   &&((*iter).getEquipedNumber() > 0))
				slotInventory.push_back(*iter);
  }
  return slotInventory;
}



vector < InventoryElement > & InventoryAttribute::getAll()
{
  return inventory_;
}



void InventoryAttribute::giveAll(TokenEntity * unit)
{
  InventoryIterator iter;
  for (iter = inventory_.begin(); iter != inventory_.end(); ++iter)
  {
    unit->addToInventory((*iter).getItemType(), (*iter).getItemNumber() );
  }
  inventory_.clear();
}



///*
// * Takes up to num items from inventory. Returns number of items taken
// */
//int InventoryAttribute::takeFromInventory(ItemRule * item, int num)
//{
//  InventoryElement * itemFound = findItem(item);
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
//              deleteElement(itemFound);
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
 * Returns number of items InventoryAttribute actually has.
 */

int InventoryAttribute::take(ItemRule * item, int num)
{
  InventoryElement * itemFound = findItem(item);
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
			//	item->applyEquipementEffects(this,numItems);
                    		entity_->recalculateStats();
                  	}
			return num;
		}
  else // (numItems <= numToGive)
    	{
              int wasEquiped = itemFound->getEquipedNumber();
              deleteElement(itemFound);
              if(wasEquiped >0)
                {
		//	item->applyEquipementEffects(this,0);
                  entity_->recalculateStats();
                }

		return numItems;
	}


}



void InventoryAttribute::deleteElement(InventoryElement * element)
{
  InventoryIterator iter = find(inventory_.begin(),inventory_.end(),*element);
  if(iter != inventory_.end())
    {
		  inventory_.erase(iter);
    }
}

/*
 * After  checks for equipability and skills do equip item
 */
int InventoryAttribute::equipItem(ItemRule * item, int num) {
	InventoryElement * itemFound = findItem(item);
	if (!itemFound)
		return 0;

	int currentlyEquipedItems = itemFound->getEquipedNumber();
	// Item may demand condition for being equiped.
	BasicCondition * equipCondition = registerEquipCondition(item,currentlyEquipedItems,num);

	if (num <= currentlyEquipedItems) // unequip
	{
		itemFound->setEquipedNumber(num);
		return (num - currentlyEquipedItems);
	}

	// Can't equip more than have
	if (num > itemFound->getItemNumber())
		num = itemFound->getItemNumber();
	if (equipCondition)
	{
		if (!equipCondition->isSatisfied(entity_))
		{
			return 0;
		}
	}
	itemFound->setEquipedNumber(num);
	//		        item->applyEquipementEffects(this,num)
	return (num - currentlyEquipedItems);
}



BasicCondition * InventoryAttribute::registerEquipCondition(ItemRule * item,
		int currentlyEquipedItems, int num)
{
	BasicCondition * equipCondition = item->demandsEquipCondition();
	if (equipCondition)
	{
		if (currentlyEquipedItems > 0 && num == 0)
		{
			removeEquipCondition(item);
		}
		if (currentlyEquipedItems == 0 && num > 0)
		{

			addEquipCondition(item);
		}
	}
	return equipCondition;
}

/*
 * Add I tem to inventory if it was not there
 * Increase it's amount otherwise
 */
void InventoryAttribute::add(ItemRule * item, int num)
{
  RationalNumber number(num);
  add(item, number);
}


// Add element and copy equipment state
void InventoryAttribute::add(InventoryElement  element)
{
 RationalNumber number = element.getRationalItemNumber();
  add(element.getItemType(), number);
  equipItem(element.getItemType(),element.getEquipedNumber());
}



void InventoryAttribute::add(ItemRule * item, RationalNumber& num)  // should be done better
{
//cout << print()<< " adding " <<item->print() <<endl;
  InventoryElement * itemFound = findItem(item);
  if(itemFound)
      {
        RationalNumber old(itemFound->getRationalItemNumber());
        itemFound->setRationalItemNumber(old + num);
          return;
      }
  inventory_.push_back(InventoryElement(item,num));
}



/** No descriptions */
int InventoryAttribute::hasItem(ItemRule * item)
{
  InventoryElement * itemFound = findItem(item);
  if(itemFound)
		return itemFound->getItemNumber();
	else
     return 0;
}



int InventoryAttribute::hasEquiped(ItemRule * item)
{
  InventoryElement * itemFound = findItem(item);
  if(itemFound)
		return itemFound ->getEquipedNumber();
	else
     return 0;
}



bool InventoryAttribute::isEquiped(InventoryElement * item)
{
  InventoryElement * itemFound = findItem(item->getItemType());
  if(itemFound)
		return (itemFound->getEquipedNumber() >= item->getEquipedNumber());
	else
     return false;
}



RationalNumber     InventoryAttribute::getItemAmount(ItemRule * item)
{
  InventoryElement * itemFound = findItem(item);
  if(itemFound)
		return itemFound->getRationalItemNumber();
	else
     return 0;
}






int InventoryAttribute::getProductionBonus(SkillRule * skill)
{
  int bonus = 0;
	for (InventoryIterator iter = inventory_.begin();
        iter != inventory_.end(); ++iter)
        {
         bonus += ((*iter).getItemType()->getProductionBonusValue(skill) *
				           (*iter).getItemNumber()) / entity_->getFiguresNumber();
        }
  return bonus;
}



int InventoryAttribute::getMovementBonus(MovementVariety * mode)
{
  int bonus = 0;
	for (InventoryIterator iter = inventory_.begin();
        iter != inventory_.end(); ++iter)
        {
         bonus += ((*iter).getItemType()->getMovementBonus(mode) *
				           (*iter).getItemNumber()) / entity_->getFiguresNumber();
        }
  return bonus;
}


int InventoryAttribute::getLearningBonus(SkillRule * skill)
{
	int currentBonus = 0;
  int bonus = 0;
	if(entity_==0)
	{
		cerr << "== InventoryAttribute with empty Entity! ("<< skill->getName()<<")"<<endl;
		return 0;
	}
	for (InventoryIterator iter = inventory_.begin();
        iter != inventory_.end(); ++iter)
        {
         currentBonus = (*iter).getItemType()->getLearningBonus(skill);
				 if((*iter).getItemNumber() < entity_->getFiguresNumber())
				 	continue; // all figures should have items
										// in order to benefit from bonus

				 if(currentBonus >bonus)
					bonus = currentBonus;
        }
  return bonus;
}


int InventoryAttribute::getStudyBonus(SkillRule * skill)
{
  int bonus = 0;
	for (InventoryIterator iter = inventory_.begin();
        iter != inventory_.end(); ++iter)
        {
         bonus += ((*iter).getItemType()->getStudyBonus(skill) *
		(*iter).getItemNumber()) / entity_->getFiguresNumber();
         if(entity_->isTraced())
         {
             int b = (((*iter).getItemType()->getStudyBonus(skill) *
		(*iter).getItemNumber()) / entity_->getFiguresNumber());
                cout<< entity_->print()<<" gets "
                     << ((*iter).getItemType()->getStudyBonus(skill))
                     <<" bonus from "<<(*iter).getItemType()->print()
                     << " total: " <<b<<endl;
         }
        }
  return bonus;
}

InventoryAttribute  InventoryAttribute::getObservableImage()
{

 InventoryAttribute  image(entity_);
    for (InventoryIterator iter = inventory_.begin();
            iter != inventory_.end(); ++iter)
    {
        if ((*iter).getItemType()->getWeight() == 0)
        {
            continue;
        } else
        {
            image.add(*iter);
        }
    }
   return image;
}

void InventoryAttribute::combineInventories(InventoryAttribute * inventory)
{

    for (InventoryIterator iter = inventory->getAll().begin();
            iter != inventory->getAll().end(); ++iter)
    {
        if(!this->findItem((*iter).getItemType()))//Not found
        {
          this->add(*iter);
        }
    }

}


void InventoryAttribute::reportInventory(FactionEntity * faction, ReportPrinter &out)
{
 	bool isFirst = true;
	for (InventoryIterator iter = inventory_.begin();
                iter != inventory_.end(); ++iter)
     {
       faction->addKnowledge((*iter).getItemType());
       if( isFirst)
            {
              isFirst = false;
            }
       else
            {
              out << ", ";
            }
       (*iter).reportEquipment(out);
     }
 	out <<". ";
}

void InventoryAttribute::reportPublicInventory(ReportPrinter &out, bool isMultiple)
{
 	bool isFirst = true;
	for (InventoryIterator iter = inventory_.begin();
                iter != inventory_.end(); ++iter)
     {
	if((*iter).getItemType()->getWeight() ==0)
	continue;
       if( isFirst)
            {
              isFirst = false;
              if(isMultiple)
                out << " have: ";
              else
                out << " has: ";
            }
       else
            {
              out << ", ";
            }
       (*iter).reportEquipment(out);
     }
 	out <<". ";
}

vector <AbstractArray>   InventoryAttribute::aPrint()
{
    vector <AbstractArray> out;
    vector <AbstractData *> v;
    for(InventoryIterator iter = inventory_.begin();
            iter != inventory_.end(); ++iter)
    {
        vector <AbstractData *> v = (*iter).aPrintItem();
        out.push_back(v);

      }
    return out;
}


int InventoryAttribute::getCapacity(int modeIndex)
{
    int capacity = 0;

    for(InventoryIterator iter = inventory_.begin();
            iter != inventory_.end(); ++iter)
    {
        capacity += (*iter).getCapacity(modeIndex);
    }
		return capacity;
}




int InventoryAttribute::getCapacity(MovementVariety * mode)
{
    int capacity = 0;

    for(InventoryIterator iter = inventory_.begin();
            iter != inventory_.end(); ++iter)
    {
        capacity += (*iter).getCapacity(mode);
    }
		return capacity;
}



int InventoryAttribute::getWeight()
{
	int sum = 0;
	for (InventoryIterator iter = inventory_.begin();
        iter != inventory_.end(); iter++)
		{
			sum += (*iter).getWeight();
		}
	return sum;
}


void InventoryAttribute::addStats(EntityStatistics * stats, int figuresNumber )
{

	for (InventoryIterator iter = inventory_.begin();
                      iter != inventory_.end(); ++iter)
		{
      stats->addPartialStats( (*iter).getStats(),
				(*iter).getEquipedNumber(), figuresNumber);
    }
}



int InventoryAttribute::equipedInSlot(EquipmentSlotVariety * slot)
{
	int equiped = 0;
	for (InventoryIterator iter = inventory_.begin();
                    iter != inventory_.end(); iter++)
    {
	     if( (*iter).getItemType()-> getEquipSlot() == slot)
				{
//cout<< "======================" << (*iter).getItemType()->print()<< " "<< (*iter.getEquipedNumber()<< " "<< (*iter.getItemType()-> getNumEquipSlotsRequired() <<endl;
          equiped += (*iter).getEquipedNumber() *
					 (*iter).getItemType()-> getNumEquipSlotsRequired();
				}
		}
		return equiped;
}


void InventoryAttribute::updateSlotEquipement(EquipmentSlotVariety * slot, vector < InventoryElement > & unequiped, int equipMax)
{
  int currentEquip;
  int unequippedNum;
  for (InventoryIterator iter = inventory_.begin();
  iter != inventory_.end(); ++iter)
  {
    if( (*iter).getItemType()-> getEquipSlot() != slot)
      continue;
    currentEquip = (*iter).getEquipedNumber() *
    (*iter).getItemType()-> getNumEquipSlotsRequired();
    
    if(equipMax == 0)  // no more slots
    {
      unequippedNum = currentEquip / ((*iter).getItemType()->
      getNumEquipSlotsRequired());
      (*iter).setEquipedNumber(0);
      if(unequippedNum)
      {
	unequiped.push_back(InventoryElement(
	(*iter).getItemType() , unequippedNum ) ) ;
      }
      
    }
    
    if(currentEquip <= equipMax) // enough space
    {
      equipMax -= currentEquip;
    }
    else // not enough space for all. only part will be equiped
    {
      (*iter).setEquipedNumber (equipMax/ (*iter).getItemType()->
      getNumEquipSlotsRequired());
      unequippedNum = (currentEquip - equipMax) /
      ((*iter).getItemType()-> getNumEquipSlotsRequired());
      equipMax = 0 ;
      unequiped.push_back(InventoryElement(
      (*iter).getItemType() , unequippedNum ) ) ;
    }
    
  }
  
}
void InventoryAttribute::extractAndAddKnowledge(Entity * recipient, int )
{
  for (InventoryIterator iter = inventory_.begin();
  iter != inventory_.end(); ++iter)
  {
    recipient->addKnowledge((*iter).getItemType());
  }
}


void InventoryAttribute::addEquipCondition(ItemRule * item)
{
	BasicCondition * equipCondition = item->demandsEquipCondition();
	if (equipCondition)
	{
	equipConditions_[item] = equipCondition;
	}
}


void InventoryAttribute::removeEquipCondition(ItemRule * item)
{
	equipConditions_.erase(item);
}



void InventoryAttribute::checkEquipmentConditions(TokenEntity * entity)
{
  map <ItemRule * , BasicCondition *>::iterator iter;
  for(iter = equipConditions_.begin();iter != equipConditions_.end(); ++iter)
  {
	  if(!(*iter).second->isSatisfied(entity))
	  {
		  entity->equipItem((*iter).first,0);
		  entity->addReport(new BinaryMessage(unequipReporter,entity , (*iter).first),0,0 );
	  }

    }
}
