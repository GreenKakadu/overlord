/***************************************************************************
                             GuardOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "GuardOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
extern ReportPattern *	AtReporter;

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

  orderType_   = IMMEDIATE_ORDER;// ?
}

STATUS GuardOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

  return OK;

}



ORDER_STATUS GuardOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
	return FAILURE;
}

