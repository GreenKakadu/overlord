//---------------------------------------------------------------------------
// BasicCombatEngine
// Provides generic interface to all combat engines
//---------------------------------------------------------------------------
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
class  BasicCombatEngine{
  public:
  BasicCombatEngine(){}
  virtual ~BasicCombatEngine(){}
  virtual void initialize(){}
  virtual BATTLE_RESULT processBattle(vector<TokenEntity *> &attackers,
                    vector<TokenEntity *> &defenders){return BATTLE_UNDEFINED;}

  protected:
  private:
  };
extern BasicCombatEngine combatEngine;
#endif // BASIC_COMBAT_ENGINE_H
