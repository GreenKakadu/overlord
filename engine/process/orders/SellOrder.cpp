/***************************************************************************
                          SellOrder.cpp
                             -------------------
    begin                : Thu Jun 26 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "SellOrder.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "UnaryMessage.h"
#include "ItemRule.h"
#include "RaceRule.h"
#include "LocationEntity.h"
#include "MarketRequest.h"
#include "IntegerData.h"
extern ReportPattern * cantTradeReporter;

//SellOrder instantiateSellOrder;
SellOrder * instantiateSellOrder = new SellOrder();


SellOrder::SellOrder(){
   keyword_ = "sell";
  registerOrder_();
  description = string("SELL number item-tag [price] \n") +
  "Immediate, leader-only.  This order executes when a unit has at least the\n" +
  "amount of items required, and the local market requires some of the specified\n" +
  "items.  The unit will attempt to sell the items at the specified price or\n" +
  "higher.  This happens only at the end of market days.  The report shows market\n" +
  "days.  If the item is not for purchase, the unit must have the marketing skill\n" +
  "to attempt the sale.\n" +
  "\n"+
  "The number of items may be 0, in which case as many items as possible will\n" +
  "be sold, and the order rescheduled if prefixed by a '@'.\n" +
  "\n" +
  "If the number is non zero, and less than the required number of items are\n" +
  "sold, the order will remain with the number of items adjusted down.\n";

  orderType_   = IMMEDIATE_ORDER;
}



STATUS SellOrder::loadParameters(Parser * parser,
                            ParameterList &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if (!parseIntegerParameter(parser, parameters))
      parameters.push_back( new IntegerData (9999)); // means very big number

    if(!parseGameDataParameter(entity,  parser, items, "item tag", parameters))
            return IO_ERROR;

    if (!parseIntegerParameter(parser, parameters))
      parameters.push_back( new IntegerData (1)); // means any price


  return OK;
}



ORDER_STATUS SellOrder::process (Entity * entity, ParameterList &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);

  IntegerData * par0  = dynamic_cast<IntegerData *>(parameters[0]);
  assert(par0);
  int amount = par0->getValue();

  ItemRule * item = dynamic_cast< ItemRule*>(parameters[1]);

  if ( item == 0) // item doesn't exist but we don't want to let player to know that
     {
 		  return FAILURE;
      }

  IntegerData * par2  = dynamic_cast<IntegerData *>(parameters[2]);
  assert(par2);
  int price = par2->getValue();
  if(!unit->getRace()->mayTrade())
  {
    unit->addReport(new UnaryMessage(cantTradeReporter,unit->getRace()));
 	  return INVALID;
  }
  unit->expose(true);
  unit->getLocation()->addMarketRequest(new MarketRequest(unit,unit->getCurrentOrder(),amount,item,price,SELL));

  return IN_PROGRESS;
}



ORDER_STATUS
SellOrder::completeOrderProcessing (Entity * entity, ParameterList &parameters, int result)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
  IntegerData * par0  = dynamic_cast<IntegerData *>(parameters[0]);
  assert(par0);
  int amount = par0->getValue();
  unit->expose(false);
  if ( amount > result)
  {
    par0->setValue(amount - result);
    entity->updateOrderResults(FAILURE);
    return FAILURE;
  }
  entity->updateOrderResults(SUCCESS);
  return SUCCESS;
}
