/***************************************************************************
                          WithdrawOrder.h
                              WITHDRAW order
                             -------------------
    begin                : Tue Nov 26 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef WITHDRAW_ORDER_H
#define WITHDRAW_ORDER_H

#include "OrderPrototype.h"


class WithdrawOrder : public OrderPrototype  {
public: 
	WithdrawOrder();
  STATUS loadParameters(Parser * parser, vector <AbstractData *>  &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, vector <AbstractData *>  &parameters, Order * orderId);
    protected:
static const  UINT WITHDRAW_RESTRICTED_REPORT_FLAG;

};

#endif
