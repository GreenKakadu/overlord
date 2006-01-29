/***************************************************************************
                          HeavinessCombatAction.h
                             -------------------
    begin                : Tue jul 05 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef HEAVINESS_COMBAT_ACTION_H
#define HEAVINESS_COMBAT_ACTION_H

#include "CombatActionStrategy.h"


/**
  * Heaviness Action
  *@author Alex Dribin
  */

class AbstractData;

class HeavinessCombatAction : public CombatActionStrategy  {
public:
      HeavinessCombatAction ( const string & keyword, GameData * parent):
				 CombatActionStrategy(keyword, parent){}
      HeavinessCombatAction ( const HeavinessCombatAction * prototype ):
				 CombatActionStrategy(prototype){}
		 ~HeavinessCombatAction(){}
      GameData * createInstanceOfSelf();
       BattleTargets getPotentialTargets(BattleInstance * battleInstance,
																		 CombatReport * report);
	MeleeReport		makeAttack(BattleInstance * battleInstance, BattleTargets & 		potentialTargets, CombatReport * report);
	protected:
};

extern HeavinessCombatAction       sampleHeavinessAction;
#endif
