/***************************************************************************
                             PromoteOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "PromoteOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
extern EntitiesCollection <UnitEntity>      units;
extern ReportPattern *	promotedReporter;
extern ReportPattern *	promoteReporter;

PromoteOrder * instantiatePromoteOrder = new PromoteOrder();

PromoteOrder::PromoteOrder(){
  keyword_ = "Promote";
  registerOrder_();
  description = string("PROMOTE unit-id \n") +
  "Immediate, one-shot.  This order executes as soon as you are at the same\n" +
  "location as the unit specified, and at the same level of stack if in a stack.\n" +
  "If the unit is located after you in the report order, it is moved in front of\n" +
  "you and the order completes.\n";

  orderType_   = IMMEDIATE_ORDER;
}

STATUS PromoteOrder::loadParameters(Parser * parser,
                            ParameterList &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, units, "unit id", parameters))
            return IO_ERROR;
  return OK;

}



ORDER_STATUS PromoteOrder::process (Entity * entity, ParameterList &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
 	UnitEntity * target   =  DOWNCAST_ENTITY<UnitEntity>(parameters[0]);
  if(target == 0)
   		  return FAILURE;

  if(!unit->mayInterract(target))
  {
  	return FAILURE;
 	}

  if(unit->getLeader() !=  target->getLeader() )
  {
  	return FAILURE;
  }

  if(unit->getLeader() == 0 ) // both not following anyone
  {
    if(unit->getLocation()->promoteUnit(unit,target))
        {
          unit->addReport(new UnaryMessage(promoteReporter,target));
          target->addReport(new UnaryMessage(promotedReporter,unit));
                return SUCCESS;
        }
    else
        return FAILURE;
  }
  else // both following the same leader
  {
    if(unit->getLeader()->promoteUnit(unit,target))
        {
                return SUCCESS;
        }
    else
        return FAILURE;
  }

	// determine own position (iterator) in location's units list
	// insert target before
	// what about constructions? add keyword BUILDING?
}

