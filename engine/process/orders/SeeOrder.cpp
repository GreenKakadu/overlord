/***************************************************************************
                             SeeOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "SeeOrder.h"
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

SeeOrder * instantiateSeeOrder = new SeeOrder();

SeeOrder::SeeOrder(){
  keyword_ = "See";
  registerOrder_();
  description = string("SEE [unit-id] \n") +
  "Immediate, condition.  This order executes if the unit specified is visible\n" +
  "at the same location.\n";

  orderType_   = IMMEDIATE_ORDER;
  mayInterrupt_ = true;
}

STATUS SeeOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, units, "unit id", parameters))
            return IO_ERROR;
  return OK;

}



ORDER_STATUS SeeOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
  UnitEntity * target   =  DOWNCAST_ENTITY<UnitEntity>(parameters[0]);

	if(target == 0)
		return INVALID;

   if (!unit->maySee(target)) // Not In the same place or can't see
	  return FAILURE;
   else
	return SUCCESS;
}

