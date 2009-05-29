/***************************************************************************
                             AttackOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "AttackOrder.h"
#include "StringData.h"
#include "GameConfig.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "ConstructionEntity.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
#include "BasicCombatManager.h"
extern EntitiesCollection <UnitEntity>      units;
extern EntitiesCollection <FactionEntity>      factions;
extern EntitiesCollection <ConstructionEntity>      buildingsAndShips;
extern ReportPattern *	invalidParameterReporter;
extern ReportPattern *	missingParameterReporter;
extern ReportPattern *	ownUnitAttackReporter;
extern ReportPattern *	ownFactionAttackReporter;
extern ReportPattern *	ownConstructionAttackReporter;

AttackOrder * instantiateAttackOrder = new AttackOrder();

AttackOrder::AttackOrder(){
  keyword_ = "Attack";
  registerOrder_();
  description = string("ATTACK [ unit-id|faction-id|structure-id ] \n") +
  "Full-day.  This order executes and completes when the designated unit, any\n" +
  "unit identifiable as belonging to the specified faction, or the owner of\n" +
  "the specified structure is present at the same location, and results in\n" +
  "a battle.\n\n" +
  "The attack does not occur if you are the owner of the target structure.\n";

   fullDayOrder_= false; //Really Attack IS full day order but there are
	                       //possible more than one attack per day.
												 //So full fay flag is set manually for all combat
												 // partticopants.
   orderType_   = STACK_ORDER;
}

STATUS AttackOrder::loadParameters(Parser * parser,
                            ParameterList &parameters, Entity * entity )
{
   if(!entityIsTokenEntity(entity))
            return IO_ERROR;

   string tag = parser->getWord();
   if (tag.size() == 0)
        {
        entity->addReport(new BinaryMessage(missingParameterReporter, new StringData(keyword_),
        			new StringData("unit-id or faction-id or construction id")));
         return IO_ERROR;
        }

  	if (units.checkDataType(tag) || gameConfig.isNewEntityName(tag))
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
  	if (buildingsAndShips.checkDataType(tag) || gameConfig.isNewEntityName(tag))
	{
      if(checkParameterTag(entity, tag,  buildingsAndShips, parameters))
      	return OK;
      else
        return IO_ERROR;
   	}

  entity->addReport(new TertiaryMessage(invalidParameterReporter, new StringData(keyword_),
  			new StringData(tag), new StringData("unit-id od faction-id or construction id")));
  return IO_ERROR;

}



ORDER_STATUS AttackOrder::process (Entity * entity, ParameterList &parameters)
{
  TokenEntity * tokenEntity = dynamic_cast<TokenEntity *>(entity);
  assert(tokenEntity);

  OrderLine * orderId = tokenEntity->getCurrentOrder();
  // if unit - check it is present and attack (unless it is allied)
	UnitEntity * unitTarget   =  DOWNCAST_ENTITY<UnitEntity>(parameters[0]);
  if( unitTarget != 0)
    {
      if (!tokenEntity->mayInterractTokenEntity(unitTarget)) // Not In the same place or can't see
	    return FAILURE;
      if(unitTarget->getFaction() == tokenEntity->getFaction())
        {
          tokenEntity->addReport(new UnaryMessage(ownUnitAttackReporter,unitTarget));
	        return INVALID;
        }
      else
      {
        unitTarget->getLocation()->getCombatManager()->attackAttempt(tokenEntity,unitTarget,orderId,0);
        return IN_PROGRESS;
      }
    }

  // If building - check owner  and attack owner (unless it is allied)
  ConstructionEntity * constructionTarget   =  DOWNCAST_ENTITY<ConstructionEntity>(parameters[0]);
  if( constructionTarget != 0)
    {
      if (!tokenEntity->mayInterractTokenEntity(constructionTarget)) // Not In the same place or can't see
	    return FAILURE;
      if(constructionTarget->getFaction() == 0)
      {

          tokenEntity->addReport(new UnaryMessage(ownConstructionAttackReporter,constructionTarget));
        //Special case: attack on unowned construction
        //tokenEntity->enterConstruction(target);
        return SUCCESS;
      }
      if(constructionTarget->getFaction() == tokenEntity->getFaction())
        {
          // report: can't attack your own construction
	        return INVALID;
        }
      else
      {
        constructionTarget->getLocation()->getCombatManager()->attackAttempt(tokenEntity,constructionTarget,orderId,0);
        return IN_PROGRESS;
      }
    }
  // If faction - find any unit of this faction and attack it (unless it is allied)
  FactionEntity * factionTarget   =  DOWNCAST_ENTITY<FactionEntity>(parameters[0]);
  if( factionTarget != 0)
    {
      if(factionTarget == tokenEntity->getFaction())
        {
          tokenEntity->addReport(new UnaryMessage(ownFactionAttackReporter,factionTarget));
	        return INVALID;
        }

      if (!tokenEntity->mayInterractFaction(factionTarget)) // Not In the same place or can't see
	    return FAILURE;

    // try to find  unit or construction belonging to target faction  to attack
   for(UnitIterator iter = tokenEntity->getLocation()->unitsPresent().begin();
                    iter != tokenEntity->getLocation()->unitsPresent().end(); ++iter)
       {
         if( (*iter)->getFaction() == factionTarget)
          {
            if (tokenEntity->mayInterractTokenEntity(*iter))
            {
              (*iter)->getLocation()->getCombatManager()->attackAttempt(tokenEntity,(*iter),orderId,0);
              return IN_PROGRESS;
            }

          }
       }
// the same for constructions
     }
	return FAILURE;
}



ORDER_STATUS
AttackOrder::completeOrderProcessing (Entity * entity, ParameterList &parameters, int result)
{
  assert(entity);
  ORDER_STATUS orderResult;
   switch(result)
   {
    case BATTLE_ERROR:
    //case BATTLE_UNDEFINED: temporary
    {
     orderResult = INVALID;
     break;
    }
    case ATTACKER_VICTORY:
    case DEFENDER_VICTORY:
    case DRAW:
    default:
     orderResult = SUCCESS;
//		 entity->setFullDayOrderFlag();// If combat happened no more full day orders possible

   }
  entity->updateOrderResults(orderResult);
  return orderResult;
}
