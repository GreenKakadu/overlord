/***************************************************************************
                          ExitOrder.h
                             -------------------
    begin                : Mon Sep 29 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
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
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
    protected:
};

#endif
