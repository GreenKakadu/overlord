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
#include "LocationEntity.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "TertiaryPattern.h"
#include "EntitiesCollection.h"
#include "BasicCombatManager.h"
extern EntitiesCollection <UnitEntity>      units;
extern EntitiesCollection <FactionEntity>      factions;
extern EntitiesCollection <ConstructionEntity>      buildingsAndShips;
extern Reporter *	invalidParameterReporter;
extern Reporter *	missingParameterReporter;
extern Reporter *	ownUnitAttackReporter;
extern Reporter *	ownFactionAttackReporter;
extern Reporter *	ownConstructionAttackReporter;
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

  orderType_   = STACK_ORDER;
}

STATUS AttackOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsTokenEntity(entity))
            return IO_ERROR;

   string tag = parser->getWord();
   if (tag.size() == 0) 
        {
        entity->addReport(new BinaryPattern(missingParameterReporter, new StringData(keyword_), 
        			new StringData("unit-id or faction-id or construction id")));
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
          tokenEntity->addReport(new UnaryPattern(ownUnitAttackReporter,unitTarget));
	        return INVALID;
        }
      else
      {
        combatManager->attackAttempt(tokenEntity,unitTarget,orderId);
        return SUCCESS;
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

          tokenEntity->addReport(new UnaryPattern(ownConstructionAttackReporter,constructionTarget));
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
        combatManager->attackAttempt(tokenEntity,constructionTarget,orderId);
        return SUCCESS;
      }
    }
  // If faction - find any unit of this faction and attack it (unless it is allied)
  FactionEntity * factionTarget   =  DOWNCAST_ENTITY<FactionEntity>(parameters[0]);
  if( factionTarget != 0)
    {
      if(factionTarget == tokenEntity->getFaction())
        {
          tokenEntity->addReport(new UnaryPattern(ownFactionAttackReporter,factionTarget));
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
              combatManager->attackAttempt(tokenEntity,(*iter),orderId);
              return SUCCESS;
            }

          }
       }
// the same for constructions
     }
	return FAILURE;
}



ORDER_STATUS
AttackOrder::completeOrderProcessing (Entity * entity, vector <AbstractData *>  &parameters, int result)
{
  assert(entity);
  entity->updateOrderResults(SUCCESS);
  return SUCCESS;
}
