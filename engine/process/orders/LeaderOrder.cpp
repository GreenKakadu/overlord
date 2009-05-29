/***************************************************************************
                             LeaderOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "LeaderOrder.h"
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


LeaderOrder * instantiateLeaderOrder = new LeaderOrder();

LeaderOrder::LeaderOrder(){
  keyword_ = "Leader";
  registerOrder_();
  description = string("LEADER unit-id \n") +
  "Immediate, condition.  This order executes if the unit-id is one of your\n" +
  "leaders, either directly, or at a higher level in the stack.\n";
  orderType_   = IMMEDIATE_ORDER;
}

STATUS LeaderOrder::loadParameters(Parser * parser,
                            ParameterList &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, units, "unit id", parameters))
            return IO_ERROR;
  return OK;

}



ORDER_STATUS LeaderOrder::process (Entity * entity, ParameterList &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);

  UnitEntity * leader =  DOWNCAST_ENTITY<UnitEntity>(parameters[0]);
  if(unit->isFollowingInStackTo(leader))
  	return SUCCESS;
  else
	return FAILURE;
}

