/***************************************************************************
                          MeleeCombatAction.h
                             -------------------
    begin                : Wed Apr 27 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef MELEE_COMBAT_ACTION_H
#define MELEE_COMBAT_ACTION_H

#include "CombatActionStrategy.h"

/**
  * basic operations for melee combat
  *@author Alex Dribin
  */

class AbstractData;

class MeleeCombatAction : public CombatActionStrategy  {
public:
      MeleeCombatAction ( const string & keyword, GameData * parent):
				 CombatActionStrategy(keyword, parent){}
      MeleeCombatAction ( const MeleeCombatAction * prototype ):
				 CombatActionStrategy(prototype){}
		 ~MeleeCombatAction(){}
      GameData * createInstanceOfSelf();
  MeleeAttackElement meleeAttack(BattleInstance * battleInstance,
								int melee, int damage, BattleInstance * target, int attFigNum);
	MeleeAttackElement attack(BattleInstance * battleInstance,
					BattleInstance * target, int totalHits);
	protected:
};

extern MeleeCombatAction       sampleMeleeAction;
#endif
