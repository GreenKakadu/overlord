/***************************************************************************
                          EnterOrder.cpp 
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
#include "EnterOrder.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "UnaryPattern.h"
#include "EntitiesCollection.h"
#include "ConstructionEntity.h"
#include "FactionEntity.h"
#include "StanceVariety.h"
#define STEALTH_TO_ENTER_PROTECTED_BUILDING  4
extern EntitiesCollection <ConstructionEntity>  buildingsAndShips;
extern Reporter * protectedEnterReporter;

//EnterOrder instantiateEnterOrder;
EnterOrder * instantiateEnterOrder = new EnterOrder() ;
    
EnterOrder::EnterOrder(){
  keyword_ = "enter";
  registerOrder_();
  description = string("ENTER structure-id \n") +
  "Immediate, one-shot.  Executes if the specified structure is present.  Moves\n" +
  "into the designated structure.  This is valid only for structures.  Inner\n" +
  "locations must still be entered using MOVE.  The order fails if the owner (i.e.\n" +
  "the oldest unit present in the location) does not consider the unit friendly.\n" +

  "Note that followers may issue ENTER/LEAVE orders.  These orders will cause\n" +
  "the unit to leave its stack if the order is succesful.\n";
  
    orderType_   = IMMEDIATE_ORDER;
}



STATUS EnterOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
     if(!entityIsUnit(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, buildingsAndShips, "construction id", parameters))
            return IO_ERROR;

  return OK;
}



ORDER_STATUS EnterOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);

  ConstructionEntity * buildingOrShip   =  DOWNCAST_ENTITY<ConstructionEntity>(parameters[0]);
  if ( buildingOrShip == 0) // building doesn't exist but we don't want to let player to know that
     {
 		    return FAILURE;
     }
     
  if(!unit->mayInterract(buildingOrShip))
    {
 		    return FAILURE;
    }

   if(unit->getContainingConstruction() == buildingOrShip)
   {
      return SUCCESS;
   }

   if(buildingOrShip->getFaction())
   {
    if(!(*(buildingOrShip->getFaction()->getStance(unit)) >= *alliedStance) ||(buildingOrShip->isAccepting(unit)))
      {
        // stealth unit may enter
        if( buildingOrShip->getLocation()->getFactionalObservation (buildingOrShip->getFaction())
                           >= unit->getStealth() - STEALTH_TO_ENTER_PROTECTED_BUILDING)
           {
            unit->addReport(new UnaryPattern(protectedEnterReporter, buildingOrShip));
 		        return FAILURE;
           }
      }
   }   

  unit->enterConstruction(buildingOrShip);
  return SUCCESS;
}
