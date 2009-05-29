/***************************************************************************
                          AcceptOrder.h
                             -------------------
    begin                : Thu Mar 6 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef ACCEPT_ORDER_H
#define ACCEPT_ORDER_H

#include "OrderPrototype.h"

/**
  *@author Alex Dribin
  */
class ReportPattern;
class AbstractData;

class AcceptOrder : public OrderPrototype  {
public:
	AcceptOrder();
	~AcceptOrder(){}
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
    protected:
};

#endif
