/***************************************************************************
                             HaveOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "HaveOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "ItemRule.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
extern RulesCollection <ItemRule>      items;
extern ReportPattern *	invalidParameterReporter;
extern ReportPattern *	missingParameterReporter;
extern ReportPattern *	AtReporter;

HaveOrder * instantiateHaveOrder = new HaveOrder();

HaveOrder::HaveOrder(){
  keyword_ = "Have";
  registerOrder_();
  description = string("HAVE item-tag [number] \n") +
  "Immediate, condition.  This order executes if the unit has the specified\n" +
  "number of items in its possession.  If the number is unspecified, it is\n" +
  "considered equal to 1.\n";

  orderType_   = IMMEDIATE_ORDER;
  mayInterrupt_ = true;
}

STATUS HaveOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, items, "item tag", parameters))
            return IO_ERROR;

     parseIntegerParameter(parser, parameters);

  return OK;

}



ORDER_STATUS HaveOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
  ItemRule * item = dynamic_cast<ItemRule *>(parameters[0]);

   if ( item == 0)
    {
 		return FAILURE;
    }
  int parameter = getIntegerParameter(parameters,1);
  if( parameter == 0)
  	parameter = 1;

  if (unit->hasItem(item) >= parameter)
  	return SUCCESS;
  else
	return FAILURE;
}

