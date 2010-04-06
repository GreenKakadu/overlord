/***************************************************************************
                          SummonCombatAction.h
                             -------------------
    begin                :
    copyright            : (C) 2005 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef SUMMON_COMBAT_ACTION_H
#define SUMMON_COMBAT_ACTION_H

#include "CombatActionStrategy.h"



/**
  * Summon Action
  *@author Alex Dribin
  */

class AbstractData;
class BattleEntity;


class SummonCombatAction : public CombatActionStrategy  {
public:

    SummonCombatAction(const string & keyword, GameData * parent) :
    CombatActionStrategy(keyword, parent) {}
    SummonCombatAction(const SummonCombatAction * prototype);

    ~SummonCombatAction() {}
    GameData * createInstanceOfSelf();
    STATUS initialize(Parser *parser);
    BattleTargets getPotentialTargets(BattleInstance * battleInstance,
            CombatReport * report);
    void performAction(BattleInstance * battleInstance,
            BattleTargets & potentialTargets, CombatReport * report);
protected:
    BattleEntity * entity_;
    bool randomPlace_;
    //vector <string > combatOrders_;
};

extern SummonCombatAction       sampleCombatSummonAction;
#endif
