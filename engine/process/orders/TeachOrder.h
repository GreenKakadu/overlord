/***************************************************************************
                          TeachOrder.h
                             -------------------
    begin                : Thu Jul 17 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef TEACH_ORDER_H
#define TEACH_ORDER_H

#include "OrderPrototype.h"

/**
  *@author Alex Dribin
  */

class TeachOrder : public OrderPrototype  {
public: 
	TeachOrder();
	~TeachOrder(){}
  STATUS loadParameters(Parser * parser, vector <AbstractData *>  &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, vector <AbstractData *>  &parameters, Order * orderId);
    protected:
   void preProcess_(Entity * entity, vector <AbstractData *>  &parameters, Order * orderId);
   void doProcess_(Entity * entity, vector <AbstractData *>  &parameters, Order * orderId);
};

#endif
