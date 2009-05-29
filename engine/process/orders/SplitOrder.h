/***************************************************************************
                          SplitOrder.h
                             -------------------
    begin                : Tue Jul 8 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#ifndef SPLIT_ORDER_H
#define SPLIT_ORDER_H

#include "OrderPrototype.h"

/**
  *@author Alex Dribin
  */

class SplitOrder : public OrderPrototype  {
public:
	SplitOrder();
	~SplitOrder(){}
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
};

#endif
