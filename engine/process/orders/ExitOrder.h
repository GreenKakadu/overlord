/***************************************************************************
                          ExitOrder.h
                             -------------------
    begin                : Mon Sep 29 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef EXIT_ORDER_H
#define EXIT_ORDER_H

#include "OrderPrototype.h"

/**
  *@author Alex Dribin
  */

class ReportPattern;
class ExitOrder : public OrderPrototype   {
public:
	ExitOrder();
	~ExitOrder(){}
  STATUS loadParameters(Parser * parser, vector <AbstractData *>  &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, vector <AbstractData *>  &parameters);
    protected:
};

#endif
