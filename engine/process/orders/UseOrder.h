/***************************************************************************
                          UseOrder.h
                             -------------------
    begin                : Mon May 5 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef USE_ORDER_H
#define USE_ORDER_H

#include "OrderPrototype.h"

/**
  *@author Alex Dribin
  */

class Reporter;
class UseOrder : public OrderPrototype  {
public: 
	UseOrder();
	~UseOrder(){}
  STATUS loadParameters(Parser * parser, vector <AbstractData *>  &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, vector <AbstractData *>  &parameters);
  ORDER_STATUS completeOrderProcessing (Entity * entity, vector <AbstractData *>  &parameters, int result);
    protected:
static const  UINT NO_RESOURCE_REPORT_FLAG;
};

#endif
