/***************************************************************************
                          FleeCombatOrder.h
                             -------------------
    begin                : Thu Nov 09 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef FLEE_COMBAT_ORDER_H
#define FLEE_COMBAT_ORDER_H

#include "CombatOrderPrototype.h"

/**
  *@author Alex Dribin
  */
class ReportPattern;
//class AbstractData;

class FleeCombatOrder : public CombatOrderPrototype  {
public:
	FleeCombatOrder();
	~FleeCombatOrder(){}
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
    protected:
};

#endif
