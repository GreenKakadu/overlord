/***************************************************************************
                          RangedCombatOrder.h
                             -------------------
    begin                : Wed Apr 20 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef RANGED_COMBAT_ORDER_H
#define RANGED_COMBAT_ORDER_H

#include "CombatOrderPrototype.h"

/**
  *@author Alex Dribin
  */
//class ReportPattern;
class AbstractData;

class RangedCombatOrder : public CombatOrderPrototype  {
public:
	RangedCombatOrder();
	~RangedCombatOrder(){}
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
  bool evaluate(Entity * entity, ParameterList  &);
    protected:
};

#endif
