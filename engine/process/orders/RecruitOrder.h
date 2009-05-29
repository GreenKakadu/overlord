/***************************************************************************
                          RecruitOrder.h
                             -------------------
    begin                : Thu Jun 5 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef RECRUIT_ORDER_H
#define RECRUIT_ORDER_H

#include "OrderPrototype.h"
class ReportPattern;

/**
  *@author Alex Dribin
  */

class RecruitOrder : public OrderPrototype  {
public:
	RecruitOrder();
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
  ORDER_STATUS completeOrderProcessing (Entity * entity, ParameterList &parameters, int result);
    protected:
static const  UINT INVALID_RECRUIT_REPORT_FLAG;
};

#endif
