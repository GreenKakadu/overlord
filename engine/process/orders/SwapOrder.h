/***************************************************************************
                          SwapOrder.h 
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef SWAP_ORDER_H
#define SWAP_ORDER_H

#include "OrderPrototype.h"

/**
  *@author Alex Dribin
  */
class Reporter;
class AbstractData;
class PhysicalEntity;
class ItemRule;

class SwapOrder : public OrderPrototype  {
public: 
	SwapOrder();
	~SwapOrder(){}
  STATUS loadParameters(Parser * parser, vector <AbstractData *>  &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, vector <AbstractData *>  &parameters);
  void doSwap(PhysicalEntity * tokenEntity, ItemRule * item1, int num1,
      PhysicalEntity * synchroTarget, ItemRule * item2, int num2);
    protected:
};

#endif
