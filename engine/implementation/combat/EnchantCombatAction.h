/***************************************************************************
                          EnchantCombatAction.h
                             -------------------
    begin                : 23 apr 2006
    copyright            : (C) 2005 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef ENCHANT_COMBAT_ACTION_H
#define ENCHANT_COMBAT_ACTION_H

#include "CombatActionStrategy.h"

/**
  * Enchanting Action
  *@author Alex Dribin
  */

class AbstractData;
class EnchantmentRule;

class EnchantCombatAction : public CombatActionStrategy  {
public:
      EnchantCombatAction ( const string & keyword, GameData * parent):
				 CombatActionStrategy(keyword, parent){}
      EnchantCombatAction ( const EnchantCombatAction * prototype ):
				 CombatActionStrategy(prototype){nextCombatAction_ = false;secondaryCombatAction_ =0;}
		 ~EnchantCombatAction(){}
      GameData * createInstanceOfSelf();
       BattleTargets getPotentialTargets(BattleInstance * battleInstance,
																		 CombatReport * report);
      STATUS initialize        ( Parser *parser );
        virtual void save(ostream &out);
	void		performAction(BattleInstance * battleInstance, BattleTargets & 		potentialTargets, CombatReport * report);
	protected:
		int duration_;
		EnchantmentRule * enchant_;
		CombatActionStrategy * secondaryCombatAction_;
                bool  nextCombatAction_;
};

extern EnchantCombatAction       sampleEnchantAction;
#endif
