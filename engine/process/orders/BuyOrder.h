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

/**
  *@author Alex Dribin
  */

class BuyOrder : public OrderPrototype  {
public: 
	BuyOrder();
	~BuyOrder(){}
  STATUS loadParameters(Parser * parser, vector <AbstractData *>  &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, vector <AbstractData *>  &parameters);
  ORDER_STATUS completeOrderProcessing (Entity * entity, vector <AbstractData *>  &parameters, int result);
};

#endif
