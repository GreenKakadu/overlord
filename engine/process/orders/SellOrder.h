/***************************************************************************
                          SellOrder.h
                             -------------------
    begin                : Thu Jun 26 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#ifndef SELL_ORDER_H
#define SELL_ORDER_H

#include "OrderPrototype.h"

/**
  *@author Alex Dribin
  */

class SellOrder : public OrderPrototype  {
public: 
	SellOrder();
	~SellOrder(){}
  STATUS loadParameters(Parser * parser, vector <AbstractData *>  &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, vector <AbstractData *>  &parameters, Order * orderId);
  ORDER_STATUS completeProcessing (Entity * entity, vector <AbstractData *>  &parameters, Order * orderId, int result);
};

#endif
