/***************************************************************************
                          SynchroOrderRequest.h
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

#ifndef SWAP_ORDER_REQUEST_H
#define SWAP_ORDER_REQUEST_H

#include "BasicOrderSynchronizationRequest.h"
class TokenEntity;
class ItemRule;

/**
  *@author Alex Dribin
  */

class SynchroOrderRequest : public BasicOrderSynchronizationRequest  {
public: 
	SynchroOrderRequest(OrderPrototype * orderType, TokenEntity * synchroTarget);
	~SynchroOrderRequest(){}
   bool isMatch(BasicOrderSynchronizationRequest  * request);
   inline TokenEntity * getSynchroTarget() const {return synchroTarget_;}
protected:
   TokenEntity * synchroTarget_;
};

#endif
