/***************************************************************************
                             WorkOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "WorkOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
extern ReportPattern *	AtReporter;

WorkOrder * instantiateWorkOrder = new WorkOrder();

WorkOrder::WorkOrder(){
  keyword_ = "Work";
  registerOrder_();
  description = string("WORK \n") +
  "Full-day.  Leader/follower only.  Spend the time working for the minimum\n" +
  "wage.  This is the default order.\n";

    fullDayOrder_= true;
  orderType_   = DAY_LONG_ORDER;
}

STATUS WorkOrder::loadParameters(Parser * parser,
                            ParameterList &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

  return OK;

}



ORDER_STATUS WorkOrder::process (Entity * entity, ParameterList &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
  if(unit->work())
  	return SUCCESS;
  else
	return FAILURE;
}

