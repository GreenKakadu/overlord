/***************************************************************************
                             SizeOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "SizeOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
extern ReportPattern *	invalidParameterReporter;


SizeOrder * instantiateSizeOrder = new SizeOrder();

SizeOrder::SizeOrder(){
  keyword_ = "Size";
  registerOrder_();
  description = string("SIZE [number] \n") +
  "Immediate, condition.  This order executes when the unit has at least the\n" +
  "required number of figures.\n";
  orderType_   = IMMEDIATE_ORDER;
  mayInterrupt_ = true;
}

STATUS SizeOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if(!parseIntegerParameter(parser, parameters))
    {

      entity->addReport(new TertiaryMessage(invalidParameterReporter, new StringData(keyword_), new StringData(""), new StringData("integer number")));
      return IO_ERROR;
    }
  return OK;

}



ORDER_STATUS SizeOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
	if (unit->getFiguresNumber() >= getIntegerParameter(parameters,0))
		return SUCCESS;
	else
		return FAILURE;
}

