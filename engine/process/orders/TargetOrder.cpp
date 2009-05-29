/***************************************************************************
                          TargetOrder.cpp
                             -------------------
    begin                : Tue Sep 23 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
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
#include "GameConfig.h"
#include "RulesCollection.h"
#include "EntitiesCollection.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "TokenEntity.h"
#include "DataManipulator.h"

extern DataManipulator * dataManipulatorPtr;
extern RulesCollection <ConstructionRule>      constructions;
extern EntitiesCollection <ConstructionEntity>  buildingsAndShips;
extern EntitiesCollection <UnitEntity>      units;
extern EntitiesCollection <LocationEntity>      locations;

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



STATUS TargetOrder::loadParameters(Parser * parser, ParameterList &parameters, Entity * entity )
{
   if(!entityIsTokenEntity(entity))
            return IO_ERROR;
// Player can't detect not-existing rules and enties by trying them as targets
//
	if(!parseStringParameter(entity, parser,parameters))
        return IO_ERROR;
    else
  		return OK;
}



ORDER_STATUS TargetOrder::process (Entity * entity, ParameterList &parameters)
{
  TokenEntity * tokenEntity = dynamic_cast<TokenEntity *>(entity);
  assert(tokenEntity);


 if(parameters.size() >0)
    {
      string tag = (parameters[0])->print();
      if (tag.size() != 0)
        {
          tokenEntity->setTarget(TargetOrder::findTarget(tag));
        }
     }
      return SUCCESS;
}



bool TargetOrder::isUnit(const string & target)
{
   if (!units.checkDataType(target)) // this doesn't look like a tag  but it still may be new tag
       {
           if(!gameConfig.isNewEntityName(target))
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
           if(!gameConfig.isNewEntityName(target))
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



// if entity / rule - return pointer
// if name may be newEntityPlaceholder -create it and return pointer
// otherwise - just return stringdata
AbstractData * TargetOrder::findTarget(const string & tag)
{
  AbstractData * target;
  assert(dataManipulatorPtr);

// Placeholder?
  if(gameConfig.isNewEntityName(tag))
    {
      NewEntityPlaceholder * placeholder = dataManipulatorPtr->findOrAddPlaceholder(tag);
      if(placeholder != 0)  // this is  placeholder.
        {
          GameData* realEntity = placeholder->getRealEntity();
          if(realEntity) // We can get real entity id from placeholder
   		      return realEntity;
          else   // placeholder is still empty
   		      return placeholder;
        }
		}

// Tag?
	target =  dataManipulatorPtr->findGameData(tag);
  if(target)
		{
      return target;
		}
//		parameter is not a tag and not a placeholder
      return (new StringData (tag));
}
//AbstractData * TargetOrder::findTarget(const string & tag)
//{
//  AbstractData * target;
//
//      target = units.findByTag(tag,false);
//      if (target)
//        return target;
//
//      target = locations.findByTag(tag,false);
//      if (target)
//        return target;
//
//      target = buildingsAndShips.findByTag(tag,false);
//      if (target)
//        return target;
//
//      target = constructions.findByTag(tag,false);
//      if (target)
//        return target;
//
//  if(gameConfig.isNewEntityName(tag))
// 	  {
//      return (new StringData(tag));
//		}
//
//      return 0;
//}
