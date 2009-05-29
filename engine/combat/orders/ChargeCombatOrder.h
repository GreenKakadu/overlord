/***************************************************************************
                          ChargeCombatOrder.h
                             -------------------
    begin                : Thu Dec 09 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef CHARGE_COMBAT_ORDER_H
#define CHARGE_COMBAT_ORDER_H

#include "CombatOrderPrototype.h"

/**
  *@author Alex Dribin
  */
//class ReportPattern;
class AbstractData;

class ChargeCombatOrder : public CombatOrderPrototype  {
public:
	ChargeCombatOrder();
	~ChargeCombatOrder(){}
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
    protected:
};

#endif
