/***************************************************************************
                          DisbandOrder.h
                             -------------------
    begin                : Mon Jul 7 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#ifndef DISBAND_ORDER_H
#define DISBAND_ORDER_H

#include "OrderPrototype.h"
/**
  *@author Alex Dribin
  */

class DisbandOrder : public OrderPrototype  {
public: 
	DisbandOrder();
	~DisbandOrder(){}
  STATUS loadParameters(Parser * parser, vector <AbstractData *>  &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, vector <AbstractData *>  &parameters, Order * orderId);
};

#endif
