/***************************************************************************
                          BuyOrder.h
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
#ifndef BUY_ORDER_H
#define BUY_ORDER_H
#include "OrderPrototype.h"
extern const int ANY_PRICE;
/**
  *@author Alex Dribin
  */

class BuyOrder : public OrderPrototype  {
public:
	BuyOrder();
	~BuyOrder(){}
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
  ORDER_STATUS completeOrderProcessing (Entity * entity, ParameterList &parameters, int result);
  static const int ANY_PRICE;
};

#endif
