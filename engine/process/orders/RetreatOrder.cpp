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
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "TertiaryPattern.h"
#include "EntitiesCollection.h"
#include "ObservationCondition.h"
	
extern Reporter * retreatGroupReporter;
extern Reporter * retreatPublicReporter;
extern Reporter * retreatPrivateReporter;

RetreatOrder * instantiateRetreatOrder = new RetreatOrder();

RetreatOrder::RetreatOrder(){
  keyword_ = "Retreat";
  registerOrder_();
  description = string("RETREAT \n") +
  "Immediate, special.  Aborts the current movement, and go back to the previous\n" +
  "location.  RETREAT can be initiated only by the leader of the stack, or the\n" +
  "moving unit.  The stack will\n" +
  "spend as much time it had spent moving going back to its starting point.\n";

  orderType_   = STACK_ORDER;
  mayInterrupt_ = true;
}

STATUS RetreatOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsTokenEntity(entity))
            return IO_ERROR;

  return OK;

}

ORDER_STATUS RetreatOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
  TokenEntity * unit = dynamic_cast<TokenEntity *>(entity);
  assert(unit);
  OrderLine * orderId = entity->getCurrentOrder();
  if( unit->retreat())
    {
        unit->addReport(
            new UnaryPattern(retreatPrivateReporter, unit->getLocation()),orderId,0);
         unit->getLocation()->addReport(
            new BinaryPattern(retreatPublicReporter, unit, unit->getLocation()),
             orderId ,ObservationCondition::createObservationCondition(unit->getStealth())
                                       );

        unit->movingGroupReport(ReportRecord(new BinaryPattern(retreatGroupReporter,
                unit->getLocation(), unit), orderId, 0));
	      return SUCCESS;
	  }
	else
    return INVALID;  
}

