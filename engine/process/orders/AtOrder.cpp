/***************************************************************************
                             AtOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "AtOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
extern ReportPattern *	invalidParameterReporter;
extern ReportPattern *	missingParameterReporter;

AtOrder * instantiateAtOrder = new AtOrder();

AtOrder::AtOrder(){
  keyword_ = "At";
  registerOrder_();
  description = string("At location-id \n") +
  "Immediate, condition.  This order executes when the unit is at the location\n" +
  "specified.  If a structure id is specified, it executes when the location is\n" +
  "entered; if a location id is specified and the unit is in a structure at the\n" +
  "location, the order is not executed.\n";
  orderType_   = IMMEDIATE_ORDER;
  mayInterrupt_ = true;
}

STATUS AtOrder::loadParameters(Parser * parser,
                            ParameterList &parameters, Entity * entity )
{
   if(!entityIsTokenEntity(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, gameFacade->locations, "location id", parameters))
            return IO_ERROR;
  return OK;

}



ORDER_STATUS AtOrder::process (Entity * entity, ParameterList &parameters)
{
 TokenEntity * unit = dynamic_cast<TokenEntity *>(entity);
  assert(unit);
  LocationEntity * location = dynamic_cast<LocationEntity *>(parameters[0]);

   if (location  == unit->getLocation())
	return SUCCESS;
   else
	return FAILURE;
}

