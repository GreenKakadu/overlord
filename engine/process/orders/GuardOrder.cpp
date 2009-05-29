/***************************************************************************
                             GuardOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "GuardOrder.h"
//#include "StanceVariety.h"
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
//ReportPattern *	guardingReporter = new ReportPattern("guardingReporter");
ReportPattern *	guardingNotAllowedReporter = new
                   ReportPattern("guardingNotAllowedReporter");

GuardOrder * instantiateGuardOrder = new GuardOrder();

GuardOrder::GuardOrder(){
  keyword_ = "Guard";
  registerOrder_();
  description = string("GUARD \n") +
  "Immediate, half-day.  This order is executed during the immediate orders\n" +
  "phase, but does not take effect until the full-day.  The unit spends half of\n" +
  "the full-day time guarding the location, and the remaining half day to\n" +
  "execute whatever full-day order was given.\n" +
  "\n" +
  "If the unit moves, the GUARD order is cancelled.  The unit must have at least\n" +
  "1st level in combat, and the faction have a title covering the location.\n" +
  "\n" +
  "GUARD units always have a tactical settings of FIGHT and FRONT, and\n" +
  "immediately position their settings of ADVERTISE and ANNOUNCE to on.  The\n" +
  "fact that a unit did guard somewhere is advertised in the report, even if\n" +
  "the unit guarded the location only one day during the turn.\n";

    fullDayOrder_= true;
  orderType_   = IMMEDIATE_ORDER;// ?
}

STATUS GuardOrder::loadParameters(Parser * parser,
                            ParameterList &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

  return OK;

}



ORDER_STATUS GuardOrder::process (Entity * entity, ParameterList &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
  if(!unit->mayGuard(true))
	  return INVALID;

    unit->getLocation()->getCombatManager()->addGuardingRequest(
               new GuardingRequest(unit,unit->getCurrentOrder(),0));
    return IN_PROGRESS;
}



ORDER_STATUS
GuardOrder::completeOrderProcessing (Entity * entity, ParameterList &parameters, int result)
{
   assert(entity);
   ORDER_STATUS orderResult = static_cast<ORDER_STATUS>(result);
/*	 if(result == SUCCESS)
	 	cout <<"##         ##"<< *entity << " guards. " <<endl;
	 else
	 	cout <<"##         ##"<< *entity << " cant guard. " <<endl;*/
  entity->updateOrderResults(orderResult);
  return orderResult;
}
