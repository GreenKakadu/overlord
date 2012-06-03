/***************************************************************************
                             SeeOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "SeeOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"

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
                            ParameterList &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, gameFacade->units, "unit id", parameters))
            return IO_ERROR;
  return OK;

}



ORDER_STATUS SeeOrder::process (Entity * entity, ParameterList &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
  UnitEntity * target   =  DOWNCAST_ENTITY<UnitEntity>(parameters[0]);

	if(target == 0)
        {
          NewEntityPlaceholder * placeholder = dynamic_cast<NewEntityPlaceholder *>(parameters[0]);
          if (placeholder == 0)
          {
           return INVALID;
          }
          else
          {
            return FAILURE;
          }
        }

   if (!unit->maySee(target)) // Not In the same place or can't see
	  return FAILURE;
   else
	return SUCCESS;
}

