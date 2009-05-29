/***************************************************************************
                          RangedCombatAction.h
                             -------------------
    begin                : Sun jul 03 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef RANGED_COMBAT_ACTION_H
#define RANGED_COMBAT_ACTION_H

#include "CombatActionStrategy.h"

/**
  * basic operations for Ranged combat
  *@author Alex Dribin
  */

class AbstractData;

class RangedCombatAction : public CombatActionStrategy  {
public:
      RangedCombatAction ( const string & keyword, GameData * parent):
				 CombatActionStrategy(keyword, parent){range_ = 2;}
      RangedCombatAction ( const RangedCombatAction * prototype );
//:				 CombatActionStrategy(prototype){range_ = 2;}
   ~RangedCombatAction(){}
  GameData * createInstanceOfSelf();
  virtual RangedCombatAction * cloneSelf();
  void performAction(BattleInstance * battleInstance,BattleTargets & target, CombatReport * report);
  MeleeAttackElement attack(BattleInstance * battleInstance, BattleInstance * target, int totalHits);
  BattleTargets getPotentialTargets(BattleInstance * battleInstance, CombatReport * report);
  MeleeAttackElement rangedAttack(BattleInstance * battleInstance,
				int missile, int damage, BattleInstance * target, int attFigNum);
  virtual void debugPrint(){cout<<"This is RangedCombatAction "<<endl;}
	protected:
          bool isRanged_;
};

extern RangedCombatAction       sampleRangedAction;
#endif
