/***************************************************************************
                             WaitOrder.cpp 
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "WaitOrder.h"
#include "StringData.h"
#include "IntegerData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "TertiaryPattern.h"
#include "EntitiesCollection.h"
extern EntitiesCollection <UnitEntity>      units;
extern Reporter *	invalidParameterReporter;
extern Reporter *	missingParameterReporter;
extern Reporter *	AtReporter;

WaitOrder * instantiateWaitOrder = new WaitOrder();

WaitOrder::WaitOrder(){
  keyword_ = "Wait";
  registerOrder_();
  description = string("WAIT number \n") +
  "Immediate, condition.  This order always executes, and completes when the\n" +
  "specified number of days have elapsed.\n";
  orderType_   = IMMEDIATE_ORDER;
}

STATUS WaitOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if(!parseIntegerParameter(parser, parameters))
    {

      entity->addReport(new TertiaryPattern(invalidParameterReporter, new StringData(keyword_), new StringData(""), new StringData("integer number")));
      return IO_ERROR;
    }
  return OK;

}



ORDER_STATUS WaitOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{

   IntegerData *par       =  dynamic_cast<IntegerData *>(parameters[0]);  
   assert(par);
	int waitingTime = par->getValue(); 
	if ( waitingTime <= 1)
		return SUCCESS;
	else
		{
			waitingTime--;
        	par->setValue(waitingTime);
        	return IN_PROGRESS;
		}

	return FAILURE;
}
