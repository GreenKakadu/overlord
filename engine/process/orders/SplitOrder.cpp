/***************************************************************************
                          SplitOrder.cpp 
                             -------------------
    begin                : Tue Jul 8 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "SplitOrder.h"
#include "Entity.h"
#include "InventoryElement.h"
#include "UnitEntity.h"
#include "BinaryPattern.h"
#include "EntitiesCollection.h"
#include "RaceElementData.h"
#include "RaceRule.h"
#include "LocationEntity.h"
#include "FactionEntity.h"
extern Reporter * newSplitReporter;
extern Reporter * splitterReporter;
extern EntitiesCollection <UnitEntity>      units;

SplitOrder::SplitOrder(){
  keyword_ = "split";
  description = string("SPLIT unit-id [number]\n") +
  "Immediate, follower/creature-only.  This orders executes when a unit has a\n"+
  "least number+1 figures.  The unit-id, which must be a new unit id, is created\n"+
  "just after the splitting unit, at the same location, or in the same stack.\n"+
  "If the unit had items equipped, figures without equipment will be transferred\n"+
  "first, then figures with the equipment.  Equipped items will remain equipped\n"+
  "if transferred to the new unit.  Unequipped items remain in the unit's\n"+
  "possession.\n";

  orderType_   = IMMEDIATE_ORDER;
}
STATUS SplitOrder::loadParameters(Parser * parser, vector <AbstractData *>
          &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;
    if(!parseGameDataParameter(entity, parser, units, "unit id", parameters))
            return IO_ERROR;

    parseIntegerParameter(parser, parameters);

            return OK;
}
ORDER_STATUS SplitOrder::process (Entity * entity, vector <AbstractData *>
          &parameters, Order * orderId)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
  // race check - can't split
  int totalFigures = unit->getFiguresNumber();
  int number =  getIntegerParameter(parameters,1);
  if( number >= totalFigures)
    {
      // report not enough units to split
        return INVALID;
      }
  if( number == 0 )
        number  = unit->getFiguresNumber();
  UnitEntity * newUnit;
  NewEntityPlaceholder * placeholder = dynamic_cast<NewEntityPlaceholder *>(parameters[0]);
  if (placeholder)  // Is it new entity?
  {
    Entity * realEntity = placeholder->getRealEntity();
    if(realEntity == 0)
    {
	    if (unit->isTraced())
        cout <<"== TRACING " << unit->printName()<< " merges into new unit " << number << " of " <<  unit->getRace()->getName() << "\n";
      newUnit   = new UnitEntity(unit);
      if(units.addNew(newUnit) != OK)
      {
        cout << "Failed to add new unit \n";
        return INVALID;
      }
       unit->setFigures(unit->getFiguresNumber() - number);
       unit->recalculateStats();
       RaceRule * race = unit->getRace();

       placeholder->setRealEntity(newUnit);
       unit->getFaction()->addUnit(newUnit);
       unit->getLocation()->addUnit(newUnit);
       newUnit->setRace(race,number);
       // skills - add all skills
        SkillIterator iter;
        for (iter = unit->getAllSkills().begin(); iter != unit->getAllSkills().end(); ++iter)
		      {
            newUnit->addSkill(*iter);
          }
       // items  - equipped only
        vector < InventoryElement > transferedItems =  unit->updateEquipement();
  InventoryElementIterator iterEquip;

        for (iterEquip = transferedItems.begin(); iterEquip != transferedItems.end(); ++iterEquip)
		      {
            ItemRule * item = (*iterEquip).getItemType();
            int num = (*iterEquip).getItemNumber();
            newUnit->addToInventory(item,num);
            unit->takeFromInventoryExactly(item,num);
            newUnit->equipItem(item,num);
          }


       // efects
       newUnit->recalculateStats();
//        cout << "New unit created: "<<newUnit->printName() <<" \n";

    newUnit ->addReport(new BinaryPattern (newSplitReporter,
                        newUnit, new RaceElementData ( race, number)));

    unit->addReport(new BinaryPattern(splitterReporter, unit,
                    new RaceElementData(race, number) ));
  return SUCCESS;

     }
     else
    {
      // report error unit already exists
	    //newUnit   =  DOWNCAST_ENTITY<UnitEntity>(parameters[0]);
        return INVALID;
      }
   }
 else // existing entity
  {
   //newUnit  = dynamic_cast<UnitEntity *>(parameters[0]);
        return INVALID;
  }
  return SUCCESS;
}


