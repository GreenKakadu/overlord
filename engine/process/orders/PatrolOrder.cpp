/***************************************************************************
                             PatrolOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "PatrolOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
#include "BasicCombatManager.h"
#include "GuardingRequest.h"
extern ReportPattern *	guardingReporter;
extern ReportPattern *	guardingNotAllowedReporter;

PatrolOrder * instantiatePatrolOrder = new PatrolOrder();

PatrolOrder::PatrolOrder(){
  keyword_ = "Patrol";
  registerOrder_();
  description = string("PATROL \n") +
  "Full-day.  This order executes immediately as soon as the unit isn't moving.\n" +
  "See the movement description for the details on PATROL mechanism.\n" +
  "\n" +
  "If the unit stops PATROLling, the unit will move back before anything else.\n" +
  "The unit must have at least 1st level in combat, and the faction have a title\n" +
  "covering the location.  Note that PATROL is a move order allowed to follower\n" +
  "units.\n" +
  "\n" +
  "PATROL units always have a tactical settings of FIGHT and FRONT.\n";

    fullDayOrder_= true;
  orderType_   = DAY_LONG_ORDER;
}

STATUS PatrolOrder::loadParameters(Parser * parser,
                            ParameterList &parameters, Entity * entity )

{
   if(!entityIsTokenEntity(entity))
            return IO_ERROR;

  return OK;

}



ORDER_STATUS PatrolOrder::process (Entity * entity, ParameterList &parameters)
{
  TokenEntity * token = dynamic_cast<TokenEntity *>(entity);
  assert(token);
  if(!token->mayGuard())
	  return INVALID;

    token->getLocation()->getCombatManager()->addGuardingRequest(
               new GuardingRequest(token,token->getCurrentOrder(),0));
    return IN_PROGRESS;
}



ORDER_STATUS
PatrolOrder::completeOrderProcessing (Entity * entity, ParameterList &parameters, int result)
{
   assert(entity);
   ORDER_STATUS orderResult = static_cast<ORDER_STATUS>(result);
  entity->updateOrderResults(orderResult);
  return orderResult;
}
