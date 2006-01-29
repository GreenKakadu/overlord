/***************************************************************************
                          SweepCombatOrder.h
                             -------------------
    begin                : Wed Apr 20 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef SWEEP_COMBAT_ORDER_H
#define SWEEP_COMBAT_ORDER_H

#include "CombatOrderPrototype.h"
#include "BattleField.h"

/**
  *@author Alex Dribin
  */
//class ReportPattern;
class AbstractData;
class BattleInstance;

class SweepCombatOrder : public CombatOrderPrototype  {
public:
	SweepCombatOrder();
	~SweepCombatOrder(){}
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
  BATTLE_DIRECTION sweepDirection(BattleInstance * battleInstance);
    protected:
};

#endif
