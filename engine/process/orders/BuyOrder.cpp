/***************************************************************************
                          BuyOrder.cpp
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
#include "BuyOrder.h"
#include "IntegerData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "UnaryMessage.h"
#include "ItemRule.h"
#include "RaceRule.h"
#include "LocationEntity.h"
#include "MarketRequest.h"
extern ReportPattern * cantTradeReporter;

//BuyOrder instantiateBuyOrder;
BuyOrder * instantiateBuyOrder = new BuyOrder();

BuyOrder::BuyOrder(){
   keyword_ = "buy";
  registerOrder_();
  description = string("BUY number item-tag [price] \n") +
  "Immediate, leader-only.  This order executes when a unit has at least the\n" +
  "amount of cash required, and the local market offers at least the specified\n" +
  "number of items.  The unit will attempt to buy the items at the specified price\n" +
  "or lower.  This happens only at the end of market days.  The report shows market\n" +
  "days.  If the item is not for sale, the unit must have the marketing skill to\n" +
  "attempt the purchase.\n" +
  "\n" +
  "The number of items may be 0, in which case as many items as possible will\n" +
  "be bought, and the order rescheduled if prefixed by a '@'.\n" +
  "\n" +
  "If the number is non zero, and less than the required number of items are\n" +
  "bought, the order will remain with the number of items adjusted down.\n";

  orderType_   = IMMEDIATE_ORDER;
}



STATUS BuyOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if (!parseIntegerParameter(parser, parameters))
      parameters.push_back( new IntegerData (9999)); // means very big number

    if(!parseGameDataParameter(entity,  parser, items, "item tag", parameters))
            return IO_ERROR;


    if (!parseIntegerParameter(parser, parameters))
      parameters.push_back( new IntegerData (9999)); // means very big number

  return OK;
}



ORDER_STATUS BuyOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
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
  unit->getLocation()->addMarketRequest(new MarketRequest(unit, unit->getCurrentOrder(), amount,item,price,BUY));

  return IN_PROGRESS;
}




ORDER_STATUS
BuyOrder::completeOrderProcessing (Entity * entity, vector <AbstractData *>  &parameters, int result)
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
//  cout << "Saving order for "<< unit->print() <<"=[ ";
//  orderId->save(cout);
    return FAILURE;
  }
  entity->updateOrderResults(SUCCESS);
//  cout << "Order completed for "<< unit->print() <<".\n";
  return SUCCESS;
}
