/***************************************************************************
                          AttackOrder.h
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef ATTACK_ORDER_H
#define ATTACK_ORDER_H

#include "OrderPrototype.h"

/**
  *@author Alex Dribin
  */
class ReportPattern;
class AbstractData;

class AttackOrder : public OrderPrototype  {
public:
	AttackOrder();
	~AttackOrder(){}
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
  ORDER_STATUS completeOrderProcessing (Entity * entity, ParameterList &parameters, int result);
    protected:
};

#endif
