/***************************************************************************
                          MoveOrder.cpp 
                             -------------------
    begin                : Mon Apr 7 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "MoveOrder.h"
#include "StringData.h"
#include "IntegerData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "RaceRule.h"
#include "DirectionVariety.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "EntitiesCollection.h"
#include "RulesCollection.h"
#include "TravelElement.h"
#include "BasicExit.h"
#include "QuartenaryPattern.h"
extern EntitiesCollection <LocationEntity>      locations;
extern VarietiesCollection  <DirectionVariety>      directions;
const UINT MoveOrder::OVERLOADING_REPORT_FLAG = 0x01;
const UINT MoveOrder::NO_MOVEMENT_ABILITY_REPORT_FLAG = 0x02;
extern Reporter *	invalidOrderReporter;
extern Reporter *	invalidParameterReporter;
extern Reporter *	missingParameterReporter;
extern Reporter * cantMoveReporter;
extern Reporter * overloadReporter;
extern Reporter * noMovementAbilityReporter;
extern Reporter *	invaliDirectionReporter;

//MoveOrder instantiateMoveOrder;
MoveOrder * instantiateMoveOrder = new MoveOrder();

MoveOrder::MoveOrder(){

  keyword_ = "move";
  registerOrder_();
  description = string("MOVE direction|location-id \n") +
  "Full-day, leader/creature-only, one-shot.  This order executes if you are in\n" +
  "a location from which the specified direction is available, or from which the\n" +
  "location specified by its ID is accessible.  If the unit was stacked, the unit\n" +
  "begins by unstacking.  The stack then begins to move toward the location.  The\n" +
  "order is finished when the movement is finished, a RETREAT is given to cancel\n" +
  "the movement, or the access to the location is prevented by a unit on PATROL.\n" +
  "\n" +
  "If the movement is prefixed by the infinite repeat request symbol ('@'), it\n" +
  "is retained after execution. Specific duration is ignored.\n";
  
  orderType_   = STACK_ORDER;
}



STATUS MoveOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsPhysicalEntity(entity))
            return IO_ERROR;
            
   const string tag = parser->getWord();
   
   if (tag.size() == 0)  // Missing parameter
        {
        entity->addReport(new BinaryPattern(missingParameterReporter, new StringData(keyword_), new StringData("destination ")));
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



// Currently MOVE supports only one parameter.
// Later parameter list like MOVE L123 N NE SE should be supported.
// take parsing from Caravan, execute one-by one, delete executed locations
// from parameters list. return IN-PROGRESS and SUCCESS only if the end of 
// parameters list reached
ORDER_STATUS MoveOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{

  PhysicalEntity * tokenEntity = dynamic_cast<PhysicalEntity *>(entity);
  assert(tokenEntity);
  return move(tokenEntity,parameters[0]);
}


ORDER_STATUS MoveOrder::move(PhysicalEntity * tokenEntity, AbstractData *parameter)
{
  Order * orderId = tokenEntity->getCurrentOrder();
   
  LocationEntity * location = tokenEntity->getGlobalLocation();
      
  if (location == 0)
     {  // Unit is already moving may be special message?
 		  return INVALID;
      }

  BasicExit * exit = 0;
  string parValue = parameter->printName();
	LocationEntity * destination   =  dynamic_cast<LocationEntity *>(parameter);
  if( destination != 0)
    { 
      exit = location->findExit(destination);
      parValue  = destination->getTag();
    }
  else
    {  
  // directions are relative to current positions
  // That's why they can't be calculated on loading
      DirectionVariety * direction =   dynamic_cast< DirectionVariety*>(parameter);
      if( direction != 0)
        {
          exit = location->findExit(direction);
          parValue  = direction->getTag();
        }
    }
  if (exit == 0)
     {  // direction is wrong or location not connected
      tokenEntity->addReport(new UnaryPattern(invaliDirectionReporter, new StringData(parValue)));
 		  return INVALID;
      }

//=================
   if(!tokenEntity->mayMove())
   {
      tokenEntity->addReport(new BinaryPattern(cantMoveReporter,tokenEntity,tokenEntity->getType()) );
 		  return INVALID;
   }
	if (tokenEntity->isTraced())
    cout <<"== TRACING " <<tokenEntity->printName()<< " ==> Attempts to move\n";

 tokenEntity->leaveStaying();
 
 int weight=0;
 int time = 0;
 int totalTravelTime = 999;
 MovementVariety * movingMode = 0;
 MovementMode<int> capacity;
 tokenEntity->calculateTotalWeight(weight);
 int i;
 int bestCapacity = 0;
 MovementVariety * bestMode = 0;

 for(i = 0; i < movementModes.size(); i++)
  {
   tokenEntity->calculateTotalCapacity(capacity[i], i);
   time = exit->getTravelTime(movementModes[i]);
// cout <<"++++++++ MOVING: "<< tokenEntity->printName() <<" "<< movementModes[i]->printName()<<" capacity "<< capacity[i]<<" time " << time<<endl;
   if(time == 0)
    continue;
    if(capacity[i] > bestCapacity)
    {
        bestCapacity = capacity[i];
        bestMode = movementModes[i];
    }
    if(weight > capacity[i])
    {
      if(movementModes[i] == walkingMode) // only walking entity may be Overloaded 
        time = tokenEntity->calculateTravelTime(time , weight, capacity[i]);
      else
        time = 0;
    }
   if(time == 0)
    continue;
 // 5. Conditions (Skill) may be demanded to enter

    if (time < totalTravelTime)
        {
         totalTravelTime = time;
         movingMode = movementModes[i];
         }
  }

  if(movingMode == 0)
    {
      if (bestMode == 0) // have no ability to move
        {
          if(!orderId->getReportingFlag(NO_MOVEMENT_ABILITY_REPORT_FLAG ))
            {
              tokenEntity->addReport(new BinaryPattern(noMovementAbilityReporter,
                              tokenEntity,exit->getDestination()));     
              orderId->setReportingFlag(NO_MOVEMENT_ABILITY_REPORT_FLAG);
            }
  	      return FAILURE;
         } 
      else // overload
        {
          orderId->clearReportingFlag(NO_MOVEMENT_ABILITY_REPORT_FLAG);
          if(!orderId->getReportingFlag(OVERLOADING_REPORT_FLAG ))
            {
        
              tokenEntity->addReport(new QuartenaryPattern(overloadReporter, tokenEntity,
                                    new IntegerData(weight),
                                    new IntegerData(bestCapacity),
                                    new StringData(bestMode->getName())));
        
              orderId->setReportingFlag(OVERLOADING_REPORT_FLAG);
            }
  	      return FAILURE;
        }
    }
    orderId->clearReportingFlag(OVERLOADING_REPORT_FLAG);
   TravelElement * moving = new TravelElement(movingMode,tokenEntity->getLocation(),exit->getDestination(),
                                totalTravelTime, totalTravelTime);
   tokenEntity->setEntityMoving(moving);
	    return SUCCESS;

}


