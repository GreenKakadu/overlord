/***************************************************************************
                          SynchroOrderRequest.h
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
class PhysicalEntity;
class ItemRule;

/**
  *@author Alex Dribin
  */

class SynchroOrderRequest : public BasicOrderSynchronizationRequest  {
public: 
	SynchroOrderRequest(OrderPrototype * orderType, PhysicalEntity * synchroTarget);
	~SynchroOrderRequest(){}
   bool isMatch(BasicOrderSynchronizationRequest  * request);
   inline PhysicalEntity * getSynchroTarget() const {return synchroTarget_;}
protected:
   PhysicalEntity * synchroTarget_;
};

#endif
