/***************************************************************************
                             LeadingOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "LeadingOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
extern EntitiesCollection <UnitEntity>      units;
extern ReportPattern *	invalidParameterReporter;
extern ReportPattern *	missingParameterReporter;
extern ReportPattern *	AtReporter;

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
                            ParameterList &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, units, "unit id", parameters))
            return IO_ERROR;
  return OK;

}



ORDER_STATUS LeadingOrder::process (Entity * entity, ParameterList &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
  UnitEntity * follower =  DOWNCAST_ENTITY<UnitEntity>(parameters[0]);
  if(follower ==0)
    {
      NewEntityPlaceholder * placeholder 
          = dynamic_cast<NewEntityPlaceholder *>(parameters[0]);	
      if (placeholder)  // Is it new entity?
      {
              Entity * realEntity = placeholder->getRealEntity();
              if(realEntity == 0)
              {
                      return FAILURE;
              }
              follower =  DOWNCAST_ENTITY<UnitEntity>(realEntity);
              if(follower ==0)
              {
                      return FAILURE;		
              }
      }
      else
        return FAILURE;		
    }
  if(follower->isFollowingInStackTo(unit))
  	return SUCCESS;
  else
	return FAILURE;
}

