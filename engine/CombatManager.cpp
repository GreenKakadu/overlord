#include "CombatManager.h"
#include "BasicCombatRequest.h"
#include "TokenEntity.h"
#include "BasicCombatEngine.h"
CombatManager combatManager;
void CombatManager::attackAttempt(TokenEntity * attacker,
                TokenEntity * defender)
{
  // create and add combatRequest
  attackAttempt(new BasicCombatRequest(attacker,defender));
}



void CombatManager::attackAttempt(BasicCombatRequest * request)
{
  // add combatRequest
}


BATTLE_RESULT CombatManager::resolve_()
{
  vector<TokenEntity *> attackers;
  vector<TokenEntity *> defenders;
  return combatEngine.processBattle(attackers,defenders);
}



void CombatManager::process() // Determine combat participants and call resolve
{

}

void CombatManager::insertCombatRequest_(BasicCombatRequest *request)
{

}



void CombatManager::addCombatRequest_(BasicCombatRequest *request)
{

}
