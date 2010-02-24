/***************************************************************************
                             PillageOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "PillageOrder.h"
#include "StringData.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
#include "PillageRequest.h"
#include "BasicCombatManager.h"
extern ReportPattern *	AtReporter;

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

    fullDayOrder_= true;
  orderType_   = REQUEST_ORDER;
  //orderType_   = IMMEDIATE_ORDER;
}

STATUS PillageOrder::loadParameters(Parser * parser,
                            ParameterList &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

  return OK;

}



ORDER_STATUS PillageOrder::process (Entity * entity, ParameterList &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
	LocationEntity * location = unit->getLocation();
	assert(location);
  if(!location->unitMayPillage(unit,true))
	  return INVALID;
	// try to pillage
	// guarding units will attack pilager unless allied
	// submit pillaging request to Location
	// Check all guarding requests and generate attack requests.
	// if unit loses battle cancel pillaging request
	// Answer Request in PillageRequest
	// if survived attack still may pillage if land is unguarded or guard allied
	// pillaging money will be divided between pillagers proportionally to number of figures
    unit->getLocation()->getCombatManager()->addPillageRequest(
               new PillageRequest(unit,unit->getCurrentOrder()));

	return IN_PROGRESS;
}

ORDER_STATUS
PillageOrder::completeOrderProcessing (Entity * entity, ParameterList &parameters, int result)
{
   assert(entity);
	 if(result)
	 {
  	entity->updateOrderResults(SUCCESS);
		return SUCCESS;
	 }
	 else
	 {
  	entity->updateOrderResults(FAILURE);
		return FAILURE;
	}
}
