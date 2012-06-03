/***************************************************************************
                             CaravanOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "CaravanOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "TokenEntity.h"
#include "LocationEntity.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
#include "MoveOrder.h"
extern ReportPattern *	invalidParameterReporter;
extern ReportPattern *	missingParameterReporter;
extern ReportPattern *	caravanLocationAddedReporter;

CaravanOrder * instantiateCaravanOrder = new CaravanOrder();

CaravanOrder::CaravanOrder(){
  keyword_ = "Caravan";
  registerOrder_();
  description = string("CARAVAN location-id location-id ... \n") +
  "Full-day, leader/creature-only.  This order executes if you are in one of the\n" +
  "specified locations in the list, and the next location in the list is available\n" +
  "as a direction or is accessible (such as an inner/outer location).  The unit\n" +
  "will execute a MOVE order, subject to all rules and restriction of MOVE, toward\n" +
  "the next location in list.\n" +
  "\n" +
  "When the location moved to is the last in the list, the list of locations is\n" +
  "reversed after the move is initiated, and the order is considered executed.  A\n" +
  "repetition of the order will then execute the reversed move, not the initial\n" +
  "one.\n";
  orderType_   = STACK_ORDER;
}

STATUS CaravanOrder::loadParameters(Parser * parser,
                            ParameterList &parameters, Entity * entity )
{
   if(!entityIsTokenEntity(entity))
            return IO_ERROR;

	LocationEntity * location;
   string tag = parser->getWord();
	while(tag.size() != 0)
	{
		if(!gameFacade->locations.checkDataType(tag))
		{
            entity->addReport(new TertiaryMessage(invalidParameterReporter, new StringData(keyword_),
            				new StringData(tag), new StringData("location-id")));
            return IO_ERROR;
		}
	location = gameFacade->locations[tag];
	if(location)
		parameters.push_back(location);
	else
		{
			StringData * dummy = new StringData(tag);
   			parameters.push_back(dummy);
		}

	tag = parser->getWord();
	}
  if(parameters.size() <2)
  	{
         entity->addReport(new BinaryMessage(missingParameterReporter,
        					new StringData(keyword_), new StringData("at least two location-id")));
            return IO_ERROR;
  	}
  return OK;

}



ORDER_STATUS CaravanOrder::process (Entity * entity, ParameterList &parameters)
{
  TokenEntity * unit = dynamic_cast<TokenEntity *>(entity);
  assert(unit);

  LocationEntity * location = unit->getGlobalLocation();

  if (location == 0)
     {  // Unit is already moving may be special message?
 		  return INVALID;
      }
  LocationEntity * destination;
  ORDER_STATUS result;
  AbstractData * temp;
  int routeLength = parameters.size();
  for (int i = 0; i < routeLength; ++i)
  {
	  destination   =  dynamic_cast<LocationEntity *>(parameters[i]);
    if(destination == location)
        {
          if (i == routeLength - 1) // end of the route should be inverted
           {
            for (int j = 0; j < routeLength/2; ++j)
             {
              temp = parameters[j];
              parameters[j]= parameters[routeLength - 1 - j];
              parameters[routeLength - 1 - j] = temp;
             }
            result = MoveOrder::move(unit,parameters[1],false);
           }
          else
            result = MoveOrder::move(unit,parameters[i+1],false);

        if (result == SUCCESS)
	          return IN_PROGRESS;
        else
	          return result;
        }
  }
  parameters.insert(parameters.begin(),location);
  unit->addReport(new UnaryMessage(caravanLocationAddedReporter, location));
  result = MoveOrder::move(unit,parameters[1],false);

  if (result == SUCCESS)
	  return IN_PROGRESS;
	else
	  return result;
	// find current location in parameters list
	// if not - error (or add current into beginning)
	// if yes find next location and perform move order.
	// if no next location (end point) - revert parameters vector
}

