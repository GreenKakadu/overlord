/***************************************************************************
                          MarchOrder.h
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef MARCH_ORDER_H
#define MARCH_ORDER_H

#include "OrderPrototype.h"
#include "MoveOrder.h"
/**
  *@author Alex Dribin
  */
class ReportPattern;
class AbstractData;

class MarchOrder : public MoveOrder  {
public:
	MarchOrder();
	~MarchOrder(){}
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
    protected:
};

#endif
