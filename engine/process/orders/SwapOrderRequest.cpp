/***************************************************************************
                          SwapOrderRequest.cpp 
                             -------------------
    begin                : Mon Dec 29 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
 

#include "SwapOrderRequest.h"

SwapOrderRequest::SwapOrderRequest(OrderPrototype * orderType,
                  TokenEntity * swapTarget, ItemRule * item, int number)
                    :BasicOrderSynchronizationRequest(orderType)
{
   swapTarget_ = swapTarget;
   item_      = item;
   number_    = number;
  
}


/*
 *
 */
bool SwapOrderRequest::isMatch(BasicOrderSynchronizationRequest  * request)
{
  SwapOrderRequest  * swapRequest  = dynamic_cast<SwapOrderRequest *>(request);
  if(swapRequest == 0)
    return false;
  if(
     this->getSwapTarget() ==  swapRequest->getSwapTarget() &&
     this->getSwapItem()   ==  swapRequest->getSwapItem()   &&
     this->getSwapNumber() ==  swapRequest->getSwapNumber()
     )
     return true;
    else return false;
}

