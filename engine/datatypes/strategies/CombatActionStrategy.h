/***************************************************************************
                          CombatActionStrategy.h
                             -------------------
    begin                : Thu Nov 17 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef COMBAT_ACTION_H
#define COMBAT_ACTION_H

#include "Strategy.h"
#include "EntityStatistics.h"
#include "CombatReport.h"
#include "BattleField.h"
#include "MeleeAttackElement.h"
class TokenEntity;
class ActionRule;
class CombatTargetVariety;


/**
  *@author Alex Dribin
  */

class CombatActionStrategy : public Strategy  {
public: 
      CombatActionStrategy ( const string & keyword, GameData * parent);
      CombatActionStrategy ( const CombatActionStrategy * prototype ): Strategy(prototype){}
		 virtual ~CombatActionStrategy(){};
      GameData * createInstanceOfSelf();
      STATUS initialize        ( Parser *parser );

	virtual MeleeReport		makeAttack(BattleInstance * battleInstance, BattleTargets & potentialTargets, CombatReport * report);
  virtual BattleTargets getPotentialTargets(BattleInstance * battleInstance,
																		 CombatReport * report);
  virtual MeleeAttackElement attack(BattleInstance * battleInstance, BattleInstance *  target, int totalHits);

	static  bool attackPreference( const BattleTargetElement & trg1,
												const BattleTargetElement & trg2);
  int calculateHitNumber(int numStrikes, int att, int def);
// assosiated stats may represent modification to token's stats
// alternatively this may be independent action with it's own stats that are not
// cumulative with token's stats

// Some stats like initiative is allways relative, so it can be only modifier
      EntityStatistics  * getNonCumulativeStats() {return  &nonCumulativeStats;}
      EntityStatistics * getModifyingStats()  {return &modifyingStats;}
			int getInitiative();
    protected:
      EntityStatistics nonCumulativeStats;
      EntityStatistics modifyingStats;
			int range_;
			ActionRule * action_;
			CombatTargetVariety * target_;
};
extern CombatActionStrategy      sampleCombatAction;

#endif
