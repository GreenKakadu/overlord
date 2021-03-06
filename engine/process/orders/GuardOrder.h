/***************************************************************************
                          GuardOrder.h
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef GUARD_ORDER_H
#define GUARD_ORDER_H

#include "OrderPrototype.h"

/**
  *@author Alex Dribin
  */
class ReportPattern;
class AbstractData;

class GuardOrder : public OrderPrototype  {
public:
	GuardOrder();
	~GuardOrder(){}
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
  ORDER_STATUS completeOrderProcessing (Entity * entity, ParameterList &parameters, int result);
    protected:
};

#endif
