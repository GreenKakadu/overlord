/***************************************************************************
                      BattleUnit.h
               Instance taking part in combat
                          ------------------
    begin                : Mon Nov 01 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef BATTLE_UNIT_H
#define BATTLE_UNIT_H
#include <vector>
#include "BattleInstance.h"
#include "UnitEntity.h"
#include "EntityStatistics.h"
using namespace std;
class TokenEntity;
class UnitEntity;
class EntityStatistics;
class RaceRule;

class BattleUnit : public BattleInstance
{
public:
  BattleUnit(UnitEntity * origin, CombatReport * report);
  ~BattleUnit();
	bool isAlive();
	int getLife();
	int getFigures();
  protected:
	UnitEntity *origin_;
  private:
};

typedef  vector <BattleInstance *> ::iterator BattleInstanceIterator;
#endif

