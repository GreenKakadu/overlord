/***************************************************************************
                             LeadingOrder.cpp 
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "LeadingOrder.h"
#include "StringData.h"
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

LeadingOrder * instantiateLeadingOrder = new LeadingOrder();

LeadingOrder::LeadingOrder(){
  keyword_ = "Leading";
  registerOrder_();
  description = string("LEADING  unit-id \n") +
  "Immediate, condition, leader only.  This order executes if and only if the\n" +
  "unit specified is visible in the stack you are leading, at whatever level.\n";

  orderType_   = IMMEDIATE_ORDER;
}

STATUS LeadingOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, units, "unit id", parameters))
            return IO_ERROR;
  return OK;

}



ORDER_STATUS LeadingOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
  UnitEntity * follower =  DOWNCAST_ENTITY<UnitEntity>(parameters[0]);
  if(follower->isFollowingInStackTo(unit))
  	return SUCCESS;
  else	
	return FAILURE;
}

