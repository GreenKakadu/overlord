/***************************************************************************
                          SwapOrderRequest.h
                             -------------------
    begin                : Mon Dec 29 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef SWAP_ORDER_REQUEST_H
#define SWAP_ORDER_REQUEST_H

#include "BasicOrderSynchronizationRequest.h"
class TokenEntity;
class ItemRule;

/**
  *@author Alex Dribin
  */

class SwapOrderRequest : public BasicOrderSynchronizationRequest  {
public: 
	SwapOrderRequest(OrderPrototype * orderType, TokenEntity * swapTarget, ItemRule * item, int number);
	~SwapOrderRequest(){}
   bool isMatch(BasicOrderSynchronizationRequest  * request);
   inline TokenEntity * getSwapTarget() const {return swapTarget_;}
   inline ItemRule * getSwapItem() const {return item_;}
   inline int  getSwapNumber() const {return number_;}
protected:
   TokenEntity * swapTarget_;
   ItemRule * item_;
   int number_;
};

#endif
