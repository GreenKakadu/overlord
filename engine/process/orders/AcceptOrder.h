/***************************************************************************
                          AcceptOrder.h 
                             -------------------
    begin                : Thu Mar 6 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef ACCEPT_ORDER_H
#define ACCEPT_ORDER_H

#include "OrderPrototype.h"

/**
  *@author Alex Dribin
  */
class Reporter;
class AbstractData;

class AcceptOrder : public OrderPrototype  {
public: 
	AcceptOrder();
	~AcceptOrder(){}
  STATUS loadParameters(Parser * parser, vector <AbstractData *>  &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, vector <AbstractData *>  &parameters, Order * orderId);
    protected:
};

#endif
