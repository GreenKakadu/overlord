/***************************************************************************
                          StackOrder.h 
                             -------------------
    begin                : Mon Mar 3 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef STACK_ORDER_H
#define STACK_ORDER_H

#include "OrderPrototype.h"
class Reporter;

/**
  *@author Alex Dribin
  */

class StackOrder : public OrderPrototype  {
public: 
	        StackOrder();
	        ~StackOrder(){}
  STATUS loadParameters(Parser * parser, vector <AbstractData *>  &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, vector <AbstractData *>  &parameters, Order * orderId);
    protected:
};

#endif
