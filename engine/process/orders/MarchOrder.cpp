/***************************************************************************
                             MarchOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "MarchOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
#include "LocationEntity.h"
#include "RaceRule.h"
#include "DirectionVariety.h"
extern EntitiesCollection   <LocationEntity>      locations;
extern VarietiesCollection  <DirectionVariety>      directions;
extern ReportPattern *	invalidParameterReporter;
extern ReportPattern *	missingParameterReporter;
extern ReportPattern *	AtReporter;

MarchOrder * instantiateMarchOrder = new MarchOrder();

MarchOrder::MarchOrder(){
  keyword_ = "March";
  registerOrder_();
  description = string("MARCH [direction|location-id] \n") +
  "Full-day, leader/creature-only, one-shot.  This orders is identical to MOVE,\n" +
  "except that, instead of retreating if a unit on PATROL attempts to prevent your\n" +
  "entry, you will enter a battle.\n";

  orderType_   = STACK_ORDER;
}

STATUS MarchOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

   const string tag = parser->getWord();

   if (tag.size() == 0)  // Missing parameter
        {
        entity->addReport(new BinaryMessage(missingParameterReporter, new StringData(keyword_), new StringData("destination ")));
         return IO_ERROR;
        }

   LocationEntity * destination = locations[tag];
   if( destination != 0)
          {
   		      parameters.push_back(destination);
            return OK;
          }
   DirectionVariety *direction = directions[tag];
   if( direction != 0)
          {
   		      parameters.push_back(direction);
            return OK;
          }
    else
				{
          StringData * dummy = new StringData(tag);
   		     parameters.push_back(dummy);
				}
  return OK;

}



ORDER_STATUS MarchOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
	return FAILURE;
}

