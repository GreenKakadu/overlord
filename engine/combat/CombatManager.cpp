/***************************************************************************
                          CombatManager.cpp
  Similar to BasicConflict responsible for combat processing
                             -------------------
    begin                : Mon May 24 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "CombatManager.h"
#include "BasicCombatRequest.h"
#include "TokenEntity.h"


//CombatManager combatManager;

/*
 * Attack Attempt may cause submitting combat request
 * or immediate combat resolution in simple case
 */
void CombatManager::attackAttempt(TokenEntity * attacker,
                TokenEntity * defender,OrderLine * orderId)
{

  addCombatRequest_(new BasicCombatRequest(attacker,defender,orderId));
}





void CombatManager::process() 
{
 // Determine combat participants
  vector<TokenEntity *> attackers;
  vector<TokenEntity *> defenders;

 // Determine combat engine if more than one supported
    BasicCombatEngine * combatEngine;
 // call combat resolution method
   BATTLE_RESULT result = combatEngine->processBattle(attackers,defenders);

   // go through all requests. Call for  answerRequest  method
   // delete all requests, clean  requests_

}




void CombatManager::addCombatRequest_(BasicCombatRequest *request)
{
  requests_.push_back(request);
}
