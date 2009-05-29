/***************************************************************************
                          BuildOrder.h
                             -------------------
    begin                : Sat Dec 2 2006
    copyright            : (C) 2006 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef BUILD_ORDER_H
#define BUILD_ORDER_H

#include "OrderPrototype.h"

/**
  *@author Alex Dribin
  */

class ReportPattern;
class ConstructionRule;
class OrderLine;
class ConstructionEntity;
class SkillRule;

class BuildOrder : public OrderPrototype  {
public:
	BuildOrder();
	~BuildOrder(){}
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
  ORDER_STATUS completeOrderProcessing (Entity * entity, ParameterList &parameters, int result);
  ORDER_STATUS startNewConstruction( ConstructionRule * construction, UnitEntity * unit,ConstructionEntity *buildingOrShip,OrderLine * order, AbstractData * placeholder);
  ORDER_STATUS buildExistingConstruction( ConstructionEntity * construction, UnitEntity * unit, SkillRule * skill, OrderLine * order); 
    protected:
static const  UINT NO_RESOURCE_REPORT_FLAG;
};

#endif
