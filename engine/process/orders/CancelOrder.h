/***************************************************************************
                          CancelOrder.h
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef CANCEL_ORDER_H
#define CANCEL_ORDER_H

#include "OrderPrototype.h"

/**
  *@author Alex Dribin
  */
class ReportPattern;
class AbstractData;

class CancelOrder : public OrderPrototype  {
public:
	CancelOrder();
	~CancelOrder(){}
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
    protected:
   static const  unsigned CANNOT_CANCEL_OWN_REPORT_FLAG;
   static const  unsigned CANNOT_CANCEL_REPORT_FLAG;
};

#endif
