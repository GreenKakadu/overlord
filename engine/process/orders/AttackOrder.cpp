/***************************************************************************
                             AttackOrder.cpp 
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "AttackOrder.h"
#include "StringData.h"
#include "GameInfo.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "ConstructionEntity.h"
#include "FactionEntity.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "TertiaryPattern.h"
#include "EntitiesCollection.h"
extern EntitiesCollection <UnitEntity>      units;
extern EntitiesCollection <FactionEntity>      factions;
extern EntitiesCollection <ConstructionEntity>      buildingsAndShips;
extern Reporter *	invalidParameterReporter;
extern Reporter *	missingParameterReporter;
extern Reporter *	AttackReporter;
extern GameInfo game;
AttackOrder * instantiateAttackOrder = new AttackOrder();

AttackOrder::AttackOrder(){
  keyword_ = "Attack";
  registerOrder_();
  description = string("ATTACK [ unit-id|faction-id|structure-id ] \n") +
  "Full-day.  This order executes and completes when the designated unit, any\n" +
  "unit identifiable as belonging to the specified faction, or the owner of the\n" +
  "specified structure is present at the same location, and results in a battle.\n" +
  "\n" +
  "The attack does not occur if you are the owner of the target structure.\n";

  orderType_   = DAY_LONG_ORDER;
}

STATUS AttackOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

   string tag = parser->getWord();
   if (tag.size() == 0) 
        {
        entity->addReport(new BinaryPattern(missingParameterReporter, new StringData(keyword_), 
        			new StringData("unit-id od faction-id or construction id")));
         return IO_ERROR;
        }

  	if (units.checkDataType(tag) || game.isNewEntityName(tag)) 
	{
      if(checkParameterTag(entity, tag,  units, parameters))
      	return OK;
      else	
        return IO_ERROR;
   	}

  	if (factions.checkDataType(tag)) 
	{
      if(checkParameterTag(entity, tag,  factions, parameters))
      	return OK;
      else	
        return IO_ERROR;
   	}
  	if (buildingsAndShips.checkDataType(tag) || game.isNewEntityName(tag)) 
	{
      if(checkParameterTag(entity, tag,  buildingsAndShips, parameters))
      	return OK;
      else	
        return IO_ERROR;
   	}

  entity->addReport(new TertiaryPattern(invalidParameterReporter, new StringData(keyword_), 
  			new StringData(tag), new StringData("unit-id od faction-id or construction id")));
  return IO_ERROR;

}



ORDER_STATUS AttackOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
	return FAILURE;
}

