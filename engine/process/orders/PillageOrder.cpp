/***************************************************************************
                             PillageOrder.cpp 
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "PillageOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "TertiaryPattern.h"
#include "EntitiesCollection.h"
extern Reporter *	AtReporter;

PillageOrder * instantiatePillageOrder = new PillageOrder();

PillageOrder::PillageOrder(){
  keyword_ = "Pillage";
  registerOrder_();
  description = string("PILLAGE \n") +
  "Full-day.  Requires 2nd level in combat skill.  Attempts to pillage a region,\n" +
  "causing the unit to harvest the entertainment money as if it were a 1st level\n" +
  "entertainer, tax money as if it were a 1st level tax collector, and work at\n" +
  "the same time.  PILLAGE has priority over taxing/entertaining: the PILLAGing\n" +
  "unit harvests all cash prior to any entertainer or taxer.\n" +
  "\n" +
  "The unit pillaging must be in an unguarded region, or it will be attacked\n" +
  "automatically by all units on guard.  It must also be large enough to pillage\n" +
  "at least half the tax amount.\n" +
  "\n" +
  "Additional combat levels beyond the 2nd will increase the PILLAGE efficiency\n" +
  "by one level equivalence for entertainment and tax for each level in combat.\n" +
  "PILLAGE is a use of the combat skill and yields 1/20th of day of experience.\n";

  orderType_   = DAY_LONG_ORDER;
}

STATUS PillageOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

  return OK;

}



ORDER_STATUS PillageOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
	return FAILURE;
}

