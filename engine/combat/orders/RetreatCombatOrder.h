/***************************************************************************
                          RetreatCombatOrder.h
                             -------------------
    begin                : Wed Apr 20 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef RETREAT_COMBAT_ORDER_H
#define RETREAT_COMBAT_ORDER_H

#include "CombatOrderPrototype.h"

/**
  *@author Alex Dribin
  */
//class ReportPattern;
class AbstractData;
class BattleInstance;

class RetreatCombatOrder : public CombatOrderPrototype  {
public:
	RetreatCombatOrder();
	~RetreatCombatOrder(){}
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
  static bool retreatAway(BattleInstance * instance);
  static bool retreatBack(BattleInstance * instance);
   static bool mayRetreatBackward(BattleInstance * instance);
   static bool mayRetreatForward(BattleInstance * instance);
   static bool mayRetreatLeft(BattleInstance * instance);
   static bool mayRetreatRight(BattleInstance * instance);
   protected:
};

#endif
