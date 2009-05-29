/***************************************************************************
                          SpecialCombatAction.h
                             -------------------
    begin                : Sun jul 03 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef SPECIAL_COMBAT_ACTION_H
#define SPECIAL_COMBAT_ACTION_H

#include "CombatActionStrategy.h"


/**
  * basic operations for Special combat
  *@author Alex Dribin
  */

class AbstractData;

class SpecialCombatAction : public CombatActionStrategy  {
public:
      SpecialCombatAction ( const string & keyword, GameData * parent):
				 CombatActionStrategy(keyword, parent){}
      SpecialCombatAction ( const SpecialCombatAction * prototype ):
				 CombatActionStrategy(prototype){}
		 ~SpecialCombatAction(){}
      GameData * createInstanceOfSelf();
       BattleTargets getPotentialTargets(BattleInstance * battleInstance,
																		 CombatReport * report);
	 MeleeReport		makeAttack(BattleInstance * battleInstance, BattleTargets & potentialTargets, CombatReport * report);

	protected:
};

extern SpecialCombatAction       sampleSpecialAction;
#endif
