/***************************************************************************
                          EjectOrder.h
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef EJECT_ORDER_H
#define EJECT_ORDER_H

#include "OrderPrototype.h"

/**
  *@author Alex Dribin
  */
class ReportPattern;
class AbstractData;

class EjectOrder : public OrderPrototype  {
public:
	EjectOrder();
	~EjectOrder(){}
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
    protected:
};

#endif
