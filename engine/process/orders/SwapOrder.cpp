/***************************************************************************
                             SwapOrder.cpp 
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "SwapOrder.h"
#include "StringData.h"
#include "IntegerData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "TertiaryPattern.h"
#include "QuartenaryPattern.h"
#include "EntitiesCollection.h"
#include "RulesCollection.h"
#include "ItemRule.h"
#include "SwapOrderRequest.h"
#include "PhysicalEntity.h"

extern EntitiesCollection <UnitEntity>      units;
extern RulesCollection <ItemRule>      items;
extern Reporter *	giveReporter;
extern Reporter *	receiveReporter;

SwapOrder * instantiateSwapOrder = new SwapOrder();

SwapOrder::SwapOrder(){
  keyword_ = "Swap";
  registerOrder_();
  description = string("SWAP unit-id amount-1 item-tag-1 amount-2 item-tag-2 \n") +
  "Immediate.  This order executes when the designated unit is at the same\n" +
  "location, and issues the SWAP order with the amount/item 1 and 2 reversed.\n" +
  "The unit gives the amount-1 of the specified item to the designated unit, and\n" +
  "receives the amount-2 of the specified item in exchange.  Factions need not\n" +
  "to be friendly to SWAP.\n";
  orderType_   = IMMEDIATE_ORDER;
}

STATUS SwapOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, units, "unit id", parameters))
            return IO_ERROR;
    if(!parseIntegerParameter(parser, parameters))
            return IO_ERROR;
    if(!parseGameDataParameter(entity, parser, items, "item tag", parameters))
            return IO_ERROR;
    if(!parseIntegerParameter(parser, parameters))
            return IO_ERROR;
    if(!parseGameDataParameter(entity, parser, items, "item tag", parameters))
            return IO_ERROR;
   return OK;

}



ORDER_STATUS SwapOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
  	
  PhysicalEntity * tokenEntity = dynamic_cast<UnitEntity *>(entity);
  assert(tokenEntity);
  PhysicalEntity * swapTarget =DOWNCAST_ENTITY<PhysicalEntity>(parameters[0]);
  if(swapTarget == 0)
  	return INVALID;
  int num1 = 	getIntegerParameter(parameters,1);
  ItemRule * item1 = dynamic_cast<ItemRule *>(parameters[2]);	
  assert(item1);
  int num2 = 	getIntegerParameter(parameters,3);
  ItemRule * item2 = dynamic_cast<ItemRule *>(parameters[4]);	
  assert(item2);
  
  if(tokenEntity->hasItem(item1) < num1)
  {
    // report
      return FAILURE; 
  } 


  SwapOrderRequest * tempSwapRequest  = new SwapOrderRequest(this,swapTarget,item2,num2);
  // This is temporary request which is used for polymorphc access to OrderRequests
  // stored in tokenEntity.  It may be deleted if request already exists,
  // otherwise it is added to tokenEntity's OrderRequests

  if(tokenEntity->doneOrderSyncRequest(tempSwapRequest))
  {
    SwapOrder::doSwap(tokenEntity,item1,num1,swapTarget,item2,num2);
// delete request of this order
     tokenEntity->removeOrderSyncRequest(tempSwapRequest) ;
     delete tempSwapRequest;
      return SUCCESS;
  }

  BasicOrderSynchronizationRequest * swapRequest  = tokenEntity->hasOrderSyncRequest(tempSwapRequest);
  if(swapRequest == 0)
  {
      swapRequest = tempSwapRequest;
      tokenEntity->addOrderSyncRequest(swapRequest);
  }
  else
   {
     delete  tempSwapRequest;
   }
  SwapOrderRequest * tempTargetSwapRequest  = new SwapOrderRequest(this,tokenEntity,item1,num1);
  BasicOrderSynchronizationRequest * targetSwapRequest = swapTarget->hasOrderSyncRequest(tempTargetSwapRequest);
  delete tempTargetSwapRequest;
  if(targetSwapRequest)
  {
      if(tokenEntity->getFaction() != swapTarget->getFaction() )
      {
      if(!tokenEntity->mayInterractPhysicalEntity(swapTarget)) // Units of the same faction
          return FAILURE;
      }
// Do work here
    SwapOrder::doSwap(tokenEntity,item1,num1,swapTarget,item2,num2);
// delete request of this order and mark recipocal order for completeing
     swapTarget->markDoneOrderSyncRequest(targetSwapRequest);
     tokenEntity->removeOrderSyncRequest(swapRequest) ;
      return SUCCESS;
  }
 else
      return FAILURE;
}

void SwapOrder::doSwap(PhysicalEntity * tokenEntity, ItemRule * item1, int num1,
      PhysicalEntity * swapTarget, ItemRule * item2, int num2)
{
// Do work here
      tokenEntity->takeFromInventory(item1,num1);
      swapTarget->addToInventory(item1,num1);

      if (!tokenEntity->isSilent() && tokenEntity->getCurrentOrder()->isNormalReportEnabled()   )
	        {
      	      tokenEntity  ->addReport( new QuartenaryPattern(giveReporter, tokenEntity,
                              new IntegerData(num1), item1, swapTarget));
           }

      if (!swapTarget->isSilent())
	        {
    	          swapTarget->addReport( new QuartenaryPattern(receiveReporter,
                              swapTarget , new IntegerData(num1), item1, tokenEntity));
         }
}
