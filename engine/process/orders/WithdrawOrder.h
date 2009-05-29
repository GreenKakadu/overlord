/***************************************************************************
                          WithdrawOrder.h
                              WITHDRAW order
                             -------------------
    begin                : Tue Nov 26 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef WITHDRAW_ORDER_H
#define WITHDRAW_ORDER_H

#include "OrderPrototype.h"


class WithdrawOrder : public OrderPrototype  {
public:
	WithdrawOrder();
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
    protected:
static const  UINT WITHDRAW_RESTRICTED_REPORT_FLAG;

};

#endif
