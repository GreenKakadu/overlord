/***************************************************************************
                          TargetOrder.cpp 
                             -------------------
    begin                : Tue Sep 23 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "TargetOrder.h"
#include "StringData.h"
#include "ConstructionRule.h"
#include "ConstructionEntity.h"
#include "GameInfo.h"
#include "RulesCollection.h"
#include "EntitiesCollection.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "PhysicalEntity.h"

extern RulesCollection <ConstructionRule>      constructions;
extern EntitiesCollection <ConstructionEntity>  buildingsAndShips;
extern EntitiesCollection <UnitEntity>      units;
extern EntitiesCollection <LocationEntity>      locations;
extern GameInfo game;

//TargetOrder instantiateTargetOrder;
TargetOrder * instantiateTargetOrder = new TargetOrder();

TargetOrder::TargetOrder(){

  keyword_ = "target";
  registerOrder_();
  description = string("TARGET unit-id|location-id|structure-id|structure-tag \n") +
  "Immediate, one-shot.  This order executes immediately and specifies the target\n" +
  "for the USE of a skill requiring a specific target (magic skills or building\n" +
  "skills).  If used during the USE of the skill, the amount of work invested\n" +
  "is lost, and the USE cancelled.\n";
    orderType_   = IMMEDIATE_ORDER;
  mayInterrupt_ = true;
}



STATUS TargetOrder::loadParameters(Parser * parser, vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsPhysicalEntity(entity))
            return IO_ERROR;
// Player can't detect not-existing rules and enties by trying them as targets
   string tag = parser->matchWord();
   if (tag.size() != 0)
        {
//          if(TargetOrder::isValidTarget(tag))
//				    {
              parameters.push_back( new StringData (parser->getWord()));
              return OK;
//				    }
        }
  return IO_ERROR;
}


ORDER_STATUS TargetOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
  PhysicalEntity * tokenEntity = dynamic_cast<PhysicalEntity *>(entity);
  assert(tokenEntity);


 if(parameters.size() >0)
    {
     StringData * target       =  dynamic_cast<StringData *>(parameters[0]);
     if(target)
      {
//        if(isValidTarget(target->printName()))
//        {
          tokenEntity->setTarget(target->printName());
//        }
      }
     }
      return SUCCESS;
}



bool TargetOrder::isUnit(const string & target)
{
   if (!units.checkDataType(target)) // this doesn't look like a tag  but it still may be new tag
       {
           if(!game.isNewEntityName(target))
 				  {
            return false;
				  }
       }
    return true;
}



bool TargetOrder::isConstruction(const string & target)
{
   if (!constructions.checkDataType(target))
       {
             return false;
       }
    return true;

}


bool TargetOrder::isBuildingOrShip(const string & target)
{
   if (!buildingsAndShips.checkDataType(target)) // this doesn't look like a tag  but it still may be new tag
       {
           if(!game.isNewEntityName(target))
 				  {
            return false;
				  }
       }
    return true;

}


bool TargetOrder::isLocation(const string & target)
{
   if (!locations.checkDataType(target)) 
       {
            return false;
       }
    return true;

}


bool TargetOrder::isValidTarget(const string & target)
{
 return isLocation(target) || isBuildingOrShip(target) ||
        isConstruction(target)  || isUnit(target);
}
