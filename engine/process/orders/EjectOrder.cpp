/***************************************************************************
                             EjectOrder.cpp 
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "EjectOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "FactionEntity.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "TertiaryPattern.h"
#include "EntitiesCollection.h"
#include "StanceVariety.h"
extern EntitiesCollection <UnitEntity>      units;
extern Reporter *	stackingUnacceptableReporter;
extern Reporter *	stackReporter;
extern Reporter *	ejectReporter;

EjectOrder * instantiateEjectOrder = new EjectOrder();

EjectOrder::EjectOrder(){
  keyword_ = "Eject";
  registerOrder_();
  description = string("EJECT unit-id \n") +
  "Immediate, leader-only.  Executes if the specified unit id is stacked at the\n" +
  "first level of stack under your leadership.  That unit is forced to unstack\n" +
  "if you're the overall stack leader, or is moved out of your substack and just\n" +
  "after you if you're stacked below someone else.\n";
  
  orderType_   = IMMEDIATE_ORDER;
}

STATUS EjectOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, units, "unit id", parameters))
            return IO_ERROR;
  return OK;

}



ORDER_STATUS EjectOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
	UnitEntity * follower   =  DOWNCAST_ENTITY<UnitEntity>(parameters[0]);
  if(follower == 0)
  		{
   		  return FAILURE;
  		}
  if(!follower->unstack())
        {
   		    return FAILURE;
        }
  OrderLine * orderId = entity->getCurrentOrder();      
  UnaryPattern * ejectMessage = new UnaryPattern(ejectReporter, follower);      
  entity->addReport(ejectMessage,orderId,0 );
	follower->addReport(ejectMessage,orderId,0);
  UnitEntity * leader = unit->getLeader();
  if(leader)
  {
 	if(!follower->mayInterract(leader))
   		  return SUCCESS; 
   		        
   if((*(leader->getFaction()->getStance(follower)) >= *alliedStance) ||(leader->isAccepting(follower)))
      {
    		stack(follower,leader );
           BinaryPattern * stackMessage = new BinaryPattern(stackReporter, follower, leader);
          follower->addReport(stackMessage,orderId,0 );
          leader->addReport(stackMessage,orderId,0);
   		  return SUCCESS; 
      }
          else // rejected
     {
       BinaryPattern * stackingUnacceptableMessage =new BinaryPattern(stackingUnacceptableReporter, leader , follower);
       follower->addReport (stackingUnacceptableMessage,orderId,0 );
       leader->addReport(stackingUnacceptableMessage,orderId,0  );
   		  return SUCCESS; 
      }
      
  }
   		  return SUCCESS; 
}

