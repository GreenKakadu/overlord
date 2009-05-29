/***************************************************************************
                          BestowOrder.h
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef BESTOW_ORDER_H
#define BESTOW_ORDER_H

#include "OrderPrototype.h"

/**
  *@author Alex Dribin
  */
class ReportPattern;
class AbstractData;

class BestowOrder : public OrderPrototype  {
public:
	BestowOrder();
	~BestowOrder(){}
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
    protected:
   static const  unsigned CANNOT_BESTOW_SELF_REPORT_FLAG;
   static const  unsigned BESTOW_CONDITION_REPORT_FLAG;
};

#endif
