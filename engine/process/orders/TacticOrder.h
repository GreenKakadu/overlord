/***************************************************************************
                          TacticOrder.h
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef TACTIC_ORDER_H
#define TACTIC_ORDER_H

#include "OrderPrototype.h"

/**
  *@author Alex Dribin
  */
class ReportPattern;
class AbstractData;

class TacticOrder : public OrderPrototype  {
public:
	TacticOrder();
	~TacticOrder(){}
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
    protected:
};

#endif
