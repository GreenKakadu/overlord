/***************************************************************************
                          EquipOrder.h 
                             -------------------
    begin                : Mon Mar 3 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef EQUIP_ORDER_H
#define EQUIP_ORDER_H

#include "OrderPrototype.h"
class Reporter;

/**
  *@author Alex Dribin
  */

class EquipOrder : public OrderPrototype  {
public: 
	        EquipOrder();
	        ~EquipOrder(){}
  STATUS loadParameters(Parser * parser, vector <AbstractData *>  &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, vector <AbstractData *>  &parameters, Order * orderId);
    protected:
static const  UINT SKILL_REQUIRED_REPORT_FLAG;
};
#endif
