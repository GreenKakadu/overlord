/***************************************************************************
                             GetOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "GetOrder.h"
#include "StringData.h"
#include "IntegerData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
#include "RulesCollection.h"
#include "ItemRule.h"
#include "ItemElement.h"
#include "StanceVariety.h"
#include "FactionEntity.h"
extern EntitiesCollection <UnitEntity>      units;
extern EntitiesCollection <LocationEntity>  locations;
extern RulesCollection <ItemRule>      items;
extern ReportPattern *	missingParameterReporter;
extern ReportPattern *	privateGetItemsReporter;
extern ReportPattern *	publicGetItemsReporter;



GetOrder * instantiateGetOrder = new GetOrder();

GetOrder::GetOrder(){
  keyword_ = "Get";
  registerOrder_();
  description = string("GET unit-id | [location-id] [number] item-tag  [left] \n") +
  "Immediate.  Attempts to get the required amount of items from the designated\n" +
  "unit or location.  If no number is specified, attempts to get as much as\n" +
  "possible.  The order executes if the designated unit is there \n" +
  "(or designated locations is here) and has the items.  The designated unit\n" +
  "must belong to your faction.  The location must be unowned or owner to be\n" +
  "allied. The order immediately fails in the other case.  If a number of items\n" +
  "to be left is specified, the designated unit must have at least the requested\n" +
  "number of items, and the remaining number.  In that case, zero may be\n" +
  "specified as the number, and the unit will get all but the specified amount,\n" +
  "as long as one item can be had.\n";
  orderType_   = IMMEDIATE_ORDER;
}

STATUS GetOrder::loadParameters(Parser * parser,
                            ParameterList &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if(
    !parseOptionalGameDataParameter(entity,  parser, units,  parameters) &&
    !parseOptionalGameDataParameter(entity,  parser, locations,  parameters)
      )
      {
       entity->addReport(new BinaryMessage(missingParameterReporter,
 					new StringData(keyword_), new StringData("unit or location id")));
        return IO_ERROR;
      }

     parseIntegerParameter(parser, parameters);

    if(!parseGameDataParameter(entity, parser, items, "item tag", parameters))
            return IO_ERROR;

     parseIntegerParameter(parser, parameters);

 return OK;

}



ORDER_STATUS GetOrder::process (Entity * entity, ParameterList &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
  LocationEntity * location = 0;
  UnitEntity * donor   =  DOWNCAST_ENTITY<UnitEntity>(parameters[0]);
  if(donor == 0)
  {
    location          =  dynamic_cast<LocationEntity *>(parameters[0]);
    assert(location);
  }
    int toGet;
  // If number of items omitted try to get item tag as second parameter
  ItemRule * item          =  dynamic_cast<ItemRule *>(parameters[1]);
  int nextParameterIndex;
  if(item)
    {
      toGet = unit->hasItem(item);
      nextParameterIndex = 2;
    }
   else
   {
     toGet =   getIntegerParameter(parameters,1);
     item          =  dynamic_cast<ItemRule *>(parameters[2]);
     assert(item);
     nextParameterIndex = 3;
   }


  int kept =   getIntegerParameter(parameters,nextParameterIndex);
  int itemPossesion;
  if(donor)
  {
   if(unit->getFaction() != donor->getFaction())
   {
   // can't get items from foreign units
 	  return INVALID;
 	  }
 	  itemPossesion = donor-> hasItem(item);
        if (!itemPossesion)
                return FAILURE;
  }
  else
  {
   assert(location);
   if(location->getRealOwner() != 0 && !(location->getRealOwner()->stanceAtLeast(unit,alliedStance)))
   {
   // can't get items from not-allied owned locations
 	  return INVALID;
 	  }
 	  itemPossesion = location-> hasLocalItem(item);
        if (!itemPossesion)
                return FAILURE;
  }

 int reallyGot = toGet;

  if (reallyGot == 0)
    reallyGot = itemPossesion - kept;
  if (reallyGot + kept > itemPossesion)
    reallyGot = itemPossesion - kept;
  if (reallyGot <= 0)
    return FAILURE;

  unit->getLocation()->addLocalItem(item, reallyGot);

  ItemElement * gotItems = new ItemElement(item, reallyGot);

  if(donor)
  {
    donor->takeFromInventory(item, reallyGot);
//QQQ
    unit->addReport(new BinaryMessage(privateGetItemsReporter,gotItems,donor));
    donor->addReport(new BinaryMessage(publicGetItemsReporter,gotItems, unit));
  }
  else if(location) // public report only if gets from location
  {
    location->removeLocalItem(item, reallyGot);
//QQQ
    unit->addReport(new BinaryMessage(privateGetItemsReporter,gotItems,location));
    location->addReport(new BinaryMessage(publicGetItemsReporter,gotItems,unit));
  }

    if(toGet > reallyGot)
    {
        IntegerData * par       =  dynamic_cast<IntegerData *>(parameters[1]);
        assert(par);
        par->setValue(toGet - reallyGot);
        return IN_PROGRESS;
    }
        return SUCCESS;
}

