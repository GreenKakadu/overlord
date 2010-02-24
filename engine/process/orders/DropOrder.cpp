/***************************************************************************
                             DropOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "DropOrder.h"
#include "StringData.h"
#include "IntegerData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "ItemRule.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "RulesCollection.h"
extern RulesCollection <ItemRule>      items;
extern ReportPattern * privateDropItemsReporter;
extern ReportPattern * publicDropItemsReporter;

DropOrder * instantiateDropOrder = new DropOrder();

DropOrder::DropOrder(){
  keyword_ = "Drop";
  registerOrder_();
  description = string("DROP [number] item-tag [kept] \n") +
  "Immediate.  Attempts to drop the required amount of items on the ground.\n";
  orderType_   = IMMEDIATE_ORDER;
}

STATUS DropOrder::loadParameters(Parser * parser,
                            ParameterList &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

     parseIntegerParameter(parser, parameters);

    if(!parseGameDataParameter(entity, parser, items, "item tag", parameters))
            return IO_ERROR;

     parseIntegerParameter(parser, parameters);
  return OK;

}



ORDER_STATUS DropOrder::process (Entity * entity, ParameterList &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
  int toDrop;
  // If number of items omitted try to get item tag as first parameter
  ItemRule * item          =  dynamic_cast<ItemRule *>(parameters[0]);
  int nextParameterIndex;
  if(item)
    {
      toDrop = unit->hasItem(item);
      nextParameterIndex = 1;
    }
   else
   {
     toDrop =   getIntegerParameter(parameters,0);
     item          =  dynamic_cast<ItemRule *>(parameters[1]);
     assert(item);
     nextParameterIndex = 2;
   }


  int kept =   getIntegerParameter(parameters,nextParameterIndex);

          // get number of items in iventory unitItemPossesion
  int unitItemPossesion = unit-> hasItem(item);
        if (!unitItemPossesion)
                return FAILURE;

  int reallyDropped = toDrop;
  
  if(nextParameterIndex == 2)
  {
  if (reallyDropped == 0)
    reallyDropped = unitItemPossesion - kept;
  if (reallyDropped + kept > unitItemPossesion)
    reallyDropped = unitItemPossesion - kept;
  }
  if (reallyDropped <= 0)
    return FAILURE;

  unit->takeFromInventory(item, reallyDropped);
  unit->getLocation()->addLocalItem(item, reallyDropped);

  ItemElement * droppedItems = new ItemElement(item, reallyDropped);
//QQQ
  unit->addReport(new UnaryMessage(privateDropItemsReporter,droppedItems));
  unit->getLocation()->addReport(new BinaryMessage(publicDropItemsReporter,unit,droppedItems));

    if(toDrop > reallyDropped)
    {
        IntegerData * par       =  dynamic_cast<IntegerData *>(parameters[0]);
        assert(par);
        par->setValue(toDrop - reallyDropped);
        return IN_PROGRESS;
    }
        return SUCCESS;
}

