/***************************************************************************
                          DisbandOrder.cpp
                             -------------------
    begin                : Mon Jul 7 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "DisbandOrder.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "UnaryMessage.h"
extern bool ciStringCompare(const std::string& s1,const std::string& s2);
extern ReportPattern * disbandReporter;

//DisbandOrder instantiateDisbandOrder;
DisbandOrder * instantiateDisbandOrder = new DisbandOrder();

DisbandOrder::DisbandOrder(){

  keyword_ = "disband";
  registerOrder_();
  description = string("DISBAND CONFIRM \n") +
  "Instant.  This order disbands your unit\n" +
  "It dissapears with all it's possesions\n";

  orderType_   = INSTANT_ORDER;
  mayInterrupt_ = true;
}
STATUS DisbandOrder::loadParameters(Parser * parser, vector <AbstractData *>
                                      &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;
// May just use matchKeyword here
    string temp = parser->getWord();
    if (ciStringCompare(temp,"confirm"))
//    if( temp != "confirm")
            return IO_ERROR;

            return OK;
}



ORDER_STATUS DisbandOrder::process (Entity * entity, vector <AbstractData *>
                                          &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
  unit->addReport(new UnaryMessage(disbandReporter, unit));
  unit->disband();
  return SUCCESS;

}
