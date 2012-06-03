/***************************************************************************
                          CombatActionStrategy.h
                             -------------------
    begin                : Thu Nov 17 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
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
class ItemElement;



/**
  *@author Alex Dribin
  */

class CombatActionStrategy : public Strategy  {
public: 
      CombatActionStrategy ( const string & keyword, GameData * parent);
      CombatActionStrategy ( const CombatActionStrategy * prototype );
		 virtual ~CombatActionStrategy(){};
      GameData * createInstanceOfSelf();
      STATUS initialize        ( Parser *parser );
        virtual void save(ostream &out);

  virtual MeleeReport		makeAttack(BattleInstance * battleInstance, BattleTargets & potentialTargets, CombatReport * report);
  virtual void		performAction(BattleInstance * battleInstance, BattleTargets & potentialTargets, CombatReport * report);
    virtual BattleTargets getPotentialTargets(BattleInstance * battleInstance, CombatReport * report);
    virtual void processStealthTargets(BattleTargets & potentialTargets, BattleInstance * battleInstance);

  virtual MeleeAttackElement attack(BattleInstance * battleInstance, BattleInstance *  target, int totalHits);

  static  bool attackPreference( const BattleTargetElement & trg1, const BattleTargetElement & trg2);
    int calculateHitNumber(int numStrikes, int att, int def);
// assosiated stats may represent modification to token's stats
// alternatively this may be independent action with it's own stats that are not
// cumulative with token's stats
    virtual void debugPrint(){cout<<"This is CombatActionStrategy"<<endl;}
// Some stats like initiative is allways relative, so it can be only modifier
      EntityStatistics  * getNonCumulativeStats() {return  &nonCumulativeStats;}
      EntityStatistics * getModifyingStats()  {return &modifyingStats;}
  int getInitiative();
  inline void setExperienceGainingSkill(SkillRule * skill){expGainingSkill_ = skill;}
  virtual CombatActionStrategy * cloneSelf();
        bool mayApplyAction(BattleInstance * battleInstance);
        void consumeActionResources(BattleInstance * battleInstance);
   virtual  void extractKnowledge (Entity * recipient, int parameter = 0);
    protected:
      EntityStatistics nonCumulativeStats;
      EntityStatistics modifyingStats;
	int range_;
	int mana_;
	ActionRule * action_;
	CombatTargetVariety * target_;
      vector <ItemElement *> resources_;
      SkillRule *expGainingSkill_;
};
extern CombatActionStrategy      sampleCombatAction;

#endif
