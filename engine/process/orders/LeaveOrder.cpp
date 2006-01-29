/***************************************************************************
                          LeaveOrder.cpp
                             -------------------
    begin                : Mon Sep 29 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "LeaveOrder.h"
#include "ConstructionEntity.h"
#include "UnitEntity.h"

//LeaveOrder instantiateLeaveOrder;
LeaveOrder * instantiateLeaveOrder = new LeaveOrder();

LeaveOrder::LeaveOrder(){
  keyword_ = "Leave";
  registerOrder_();
  description = string("LEAVE\n") +
  "Immediate, one-shot.  Executes if the unit is inside a construction. \n" +
  "The unit moves into the containing location.\n" +

  "Note that followers may issue ENTER/LEAVE orders.  These orders will cause\n" +
  "the unit to leave its stack if the order is succesful.\n";

    orderType_   = IMMEDIATE_ORDER;
}



STATUS LeaveOrder::loadParameters(Parser * parser,
                            ParameterList &parameters, Entity * entity )
{
     if(!entityIsUnit(entity))
            return IO_ERROR;

  return OK;
}



ORDER_STATUS LeaveOrder::process (Entity * entity, ParameterList &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);

  ConstructionEntity * buildingOrShip   =  unit->getContainingConstruction();
  if ( buildingOrShip == 0)
     {
 		  return FAILURE;
      }
  unit->exitConstruction();
  return SUCCESS;
}

