/***************************************************************************
                             AtOrder.cpp 
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "AtOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "TertiaryPattern.h"
#include "EntitiesCollection.h"
extern EntitiesCollection <LocationEntity>      locations;
extern Reporter *	invalidParameterReporter;
extern Reporter *	missingParameterReporter;

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
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsPhysicalEntity(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, locations, "location id", parameters))
            return IO_ERROR;
  return OK;

}



ORDER_STATUS AtOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
 PhysicalEntity * unit = dynamic_cast<PhysicalEntity *>(entity);
  assert(unit);
  LocationEntity * location = dynamic_cast<LocationEntity *>(parameters[0]);
	
   if (location  == unit->getLocation())
	return SUCCESS;
   else			
	return FAILURE;
}

