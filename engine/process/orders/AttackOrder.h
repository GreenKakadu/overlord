/***************************************************************************
                          AttackOrder.h 
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef ATTACK_ORDER_H
#define ATTACK_ORDER_H

#include "OrderPrototype.h"

/**
  *@author Alex Dribin
  */
class Reporter;
class AbstractData;

class AttackOrder : public OrderPrototype  {
public: 
	AttackOrder();
	~AttackOrder(){}
  STATUS loadParameters(Parser * parser, vector <AbstractData *>  &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, vector <AbstractData *>  &parameters);
  ORDER_STATUS completeOrderProcessing (Entity * entity, vector <AbstractData *>  &parameters, int result);
    protected:
};

#endif
