/***************************************************************************
                             RetreatOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "RetreatOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
#include "ObservationCondition.h"

extern ReportPattern * retreatGroupReporter;
extern ReportPattern * retreatPublicReporter;
extern ReportPattern * retreatPrivateReporter;

RetreatOrder * instantiateRetreatOrder = new RetreatOrder();

RetreatOrder::RetreatOrder(){
  keyword_ = "Retreat";
  registerOrder_();
  description = string("RETREAT \n") +
  "Immediate, special.  Aborts the current movement, and go back to the previous\n" +
  "location.  RETREAT can be initiated only by the leader of the stack, or the\n" +
  "moving unit.  The stack will\n" +
  "spend as much time it had spent moving going back to its starting point.\n";

    fullDayOrder_= true;
  orderType_   = STACK_ORDER;
  mayInterrupt_ = true;
}

STATUS RetreatOrder::loadParameters(Parser * parser,
                            ParameterList &parameters, Entity * entity )
{
   if(!entityIsTokenEntity(entity))
            return IO_ERROR;

  return OK;

}

ORDER_STATUS RetreatOrder::process (Entity * entity, ParameterList &parameters)
{
  TokenEntity * unit = dynamic_cast<TokenEntity *>(entity);
  assert(unit);
  OrderLine * orderId = entity->getCurrentOrder();
  if( unit->retreat())
    {
        unit->addReport(
            new UnaryMessage(retreatPrivateReporter, unit->getLocation()),orderId,0);
         unit->getLocation()->addReport(
            new BinaryMessage(retreatPublicReporter, unit, unit->getLocation()),
             orderId ,ObservationCondition::createObservationCondition(unit->getStealth())
                                       );

        unit->movingGroupReport(ReportRecord(new BinaryMessage(retreatGroupReporter,
                unit->getLocation(), unit), orderId, 0));
	      return SUCCESS;
	  }
	else
    return INVALID;
}

