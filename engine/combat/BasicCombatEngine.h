/***************************************************************************
                      BasicCombatEngine.h
            Provides generic interface to all combat engines
                          ------------------
    begin                : Mon May 24 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#if !defined(BASIC_COMBAT_ENGINE_H)
  #define BASIC_COMBAT_ENGINE_H

#include  <vector>
using namespace std;


enum battle_result {
  BATTLE_ERROR        = 0,
  ATTACKER_VICTORY    = 1,
  DEFENDER_VICTORY    = 2,
  DRAW                = 3,
  BATTLE_UNDEFINED    = 4,

  BATTLE_RESULTS_END  = 10
};
typedef enum battle_result BATTLE_RESULT;

class TokenEntity;
class CombatReport;

class  BasicCombatEngine{
  public:
  BasicCombatEngine();
  virtual ~BasicCombatEngine(){}
  virtual void initialize(){}
  virtual BATTLE_RESULT processBattle(vector<TokenEntity *> &attackers,
                    vector<TokenEntity *> &defenders);
	inline  CombatReport * getCombatReport()const {return report_;}
	static int battleId;
  protected:
		CombatReport * report_;
  private:
  };
extern BasicCombatEngine combatEngine;
#endif // BASIC_COMBAT_ENGINE_H
