/***************************************************************************
                          MoveOrder.h 
                             -------------------
    begin                : Mon Apr 7 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef MOVE_ORDER_H
#define MOVE_ORDER_H
#include "OrderPrototype.h"

/**
  *@author Alex Dribin
  */

class MoveOrder : public OrderPrototype  {
public: 
	MoveOrder();
	~MoveOrder(){}
  STATUS loadParameters(Parser * parser, vector <AbstractData *>  &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, vector <AbstractData *>  &parameters, Order * orderId);
    protected:
static const  UINT OVERLOADING_REPORT_FLAG;
static const  UINT NO_MOVEMENT_ABILITY_REPORT_FLAG;
};

#endif
