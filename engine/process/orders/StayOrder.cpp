/***************************************************************************
                             StayOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "StayOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
extern ReportPattern *	stayReporter;

StayOrder * instantiateStayOrder = new StayOrder();

StayOrder::StayOrder(){
  keyword_ = "Stay";
  registerOrder_();
  description = string("STAY \n") +
  "Immediate, special.  This order executes when the unit is part of a stack,\n" +
  "and the stack attempts to move.  It causes the unit to leave the stack\n" +
  "immediately before the move begins.\n";
  orderType_   = IMMEDIATE_ORDER;
  mayInterrupt_ = true;
}

STATUS StayOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

   return OK;

}



ORDER_STATUS StayOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
  if(unit->getLeader()== 0) // unstacked - clear stay
  {
    unit->clearStay();
    OrderLine * orderId = entity->getCurrentOrder();
    unit->addReport(new UnaryMessage(stayReporter, unit),orderId,0 );
    return SUCCESS;
  }
  if(unit->isStaying())
    {
		  return FAILURE;
    }
   else
    {
      unit->setStaying();
    }
	return FAILURE;

}

