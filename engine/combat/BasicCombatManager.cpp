/***************************************************************************
                          BasicCombatManager.cpp
  Similar to BasicConflict responsible for combat processing
                             -------------------
    begin                : Mon May 24 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "BasicCombatManager.h"
#include "TokenEntity.h"
#include "LocationEntity.h"
#include "FactionEntity.h"
#include "BasicCombatEngine.h"
#include "BinaryPattern.h"
extern Reporter *	AttackReporter;


//BasicCombatManager combatManager;

BasicCombatManager::BasicCombatManager()
{
  combatEngine_ = new BasicCombatEngine;
}



/*
 * Attack Attempt immediate combat resolution
 */
void BasicCombatManager::attackAttempt(TokenEntity * attacker,
                TokenEntity * defender,OrderLine * orderId)
{
  vector<TokenEntity *> attackers;
  vector<TokenEntity *> defenders;
  // Report to units, factions, location
  BinaryPattern * attackMessage = new BinaryPattern(AttackReporter,attacker,defender);
  defender->getLocation()->addReport(attackMessage,orderId,0);
  attacker->getFaction()->addReport(attackMessage,orderId,0);
  defender->getFaction()->addReport(attackMessage,orderId,0);
  attacker->addReport(attackMessage,orderId,0);
  defender->addReport(attackMessage,orderId,0);
  // Determine combat participants:
  attackers.push_back(attacker);
  defenders.push_back(defender);


 // call combat resolution method
   BATTLE_RESULT result = combatEngine_->processBattle(attackers,defenders);
//    if(orderId)
//        orderId->completeProcessing(attacker,result);
        
//   attackers.clear();
//   defenders.clear();     
}



