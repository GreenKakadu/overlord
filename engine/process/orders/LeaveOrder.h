/***************************************************************************
                          LeaveOrder.h
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

#ifndef LEAVE_ORDER_H
#define LEAVE_ORDER_H

#include "OrderPrototype.h"

/**
  *@author Alex Dribin
  */

class ReportPattern;
class LeaveOrder : public OrderPrototype   {
public:
	LeaveOrder();
	~LeaveOrder(){}
  STATUS loadParameters(Parser * parser, vector <AbstractData *>  &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, vector <AbstractData *>  &parameters);
    protected:
};

#endif
