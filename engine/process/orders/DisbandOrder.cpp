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
#include "UnaryPattern.h"
extern bool ciStringCompare(const std::string& s1,const std::string& s2);
extern Reporter * disbandReporter;
 
DisbandOrder::DisbandOrder(){

  keyword_ = "disband";
  description = string("DISBAND CONFIRM \n") +
  "Instant.  This order disbands your unit\n" +
  "It dissapears with all it's possesions\n";

  orderType_   = INSTANT_ORDER;
}
STATUS DisbandOrder::loadParameters(Parser * parser, vector <AbstractData *>
                                      &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    string temp = parser->getWord();
    if (ciStringCompare(temp,"confirm"))
//    if( temp != "confirm")
            return IO_ERROR;
  
            return OK;
}



ORDER_STATUS DisbandOrder::process (Entity * entity, vector <AbstractData *>
                                          &parameters, Order * orderId)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
  unit->addReport(new UnaryPattern(disbandReporter, unit));
  unit->disband();
  return SUCCESS;

}