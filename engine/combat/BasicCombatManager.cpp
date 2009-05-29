/***************************************************************************
                          BasicCombatManager.cpp
  Similar to BasicConflict responsible for combat processing
                             -------------------
    begin                : Mon May 24 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "BasicCombatManager.h"
#include "TokenEntity.h"
#include "UnitEntity.h"
#include "ConstructionEntity.h"
#include "LocationEntity.h"
#include "FactionEntity.h"
#include "BinaryMessage.h"
extern ReportPattern *	AttackReporter;


//BasicCombatManager combatManager;

BasicCombatManager::BasicCombatManager()
{
 //cout << "===> BasicCombatManager created." <<endl;
  combatEngine_ = new BasicCombatEngine;
}



/*
 * Attack Attempt immediate combat resolution
 */

void BasicCombatManager::attackAttempt(TokenEntity * attacker, TokenEntity * defender, OrderLine * orderId,
                    void (*funPtr)(TokenEntity * attacker, TokenEntity * defender,const BATTLE_RESULT  result))
{
//cout << "===> BasicCombatManager::attackAttempt invoked." <<endl;
  vector<TokenEntity *> attackers;
  vector<TokenEntity *> defenders;
  // Report to units, factions, location
  BinaryMessage * attackMessage = new BinaryMessage(AttackReporter,attacker,defender);
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



void BasicCombatManager::attackAttempt(FactionEntity * attacker, TokenEntity * defender, OrderLine * orderId,
                    void (*funPtr)(TokenEntity * attacker, TokenEntity * defender,const BATTLE_RESULT  result))
{
cout << "===> BasicCombatManager::attackAttempt 2 invoked." <<endl;
  LocationEntity * location = defender->getLocation();
    for (UnitIterator unitIter  = location->unitsPresent().begin(); unitIter != location->unitsPresent().end(); unitIter++)
    {
        if((*unitIter)->getFaction() == attacker)
        {

          if( (*unitIter)->isGuarding() /* || DEFENSIVE or better */ )
          attackAttempt(*unitIter,defender,orderId,funPtr);
          return;
        }
    }
  for (ConstructionIterator iter  = location->constructionsPresent().begin(); iter != location->constructionsPresent().end(); iter++)
    {
        if((*iter)->getFaction() == attacker)
        {
          if( (*iter)->isGuarding() /* || DEFENSIVE or better */ )
          attackAttempt(*iter,defender,orderId,funPtr);
          return;
        }
    }

  
}


