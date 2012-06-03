/***************************************************************************
CombatManager.cpp
Similar to BasicConflict responsible for combat organizing
Determines when start combat and which tokens will take part in it
determines sides
-------------------
begin                : Mon May 24 2004
copyright            : (C) 2004 by Alex Dribin
email                : Alex.Dribin@gmail.com
***************************************************************************/
#include <algorithm>
#include "CombatManager.h"
#include "BasicCombatRequest.h"
#include "GuardingRequest.h"
#include "UnitEntity.h"
#include "ConstructionEntity.h"
#include "LocationEntity.h"
#include "FactionEntity.h"
#include "CombatStanceVariety.h"
#include "PillageRequest.h"
#include "UnaryMessage.h"
#include "TertiaryMessage.h"
#include "GameConfig.h"
#include "SimpleMessage.h"
#include "BinaryMessage.h"
#include "IntegerData.h"

//CombatManager combatManager;
ReportPattern  *	pillagingOwnerReporter = new
ReportPattern("pillagingOwnerReporter");
ReportPattern  *	pillagingGuardedReporter = new
ReportPattern("pillagingGuardedReporter");
ReportPattern  *	guardBattleLossReporter = new
ReportPattern("guardBattleLossReporter");
ReportPattern  *	pillagerBattleLossReporter = new
ReportPattern("pillagerBattleLossReporter");
ReportPattern  *	alreadyGuardedReporter= new
ReportPattern("alreadyGuardedReporter");
ReportPattern  *	uncoordinatedGuardReporter = new
ReportPattern("uncoordinatedGuardReporter");
ReportPattern  *	pillager20FiguresReporter = new
ReportPattern("pillager20FiguresReporter");
ReportPattern  *	pillagerNotEnoughReporter = new
ReportPattern("pillagerNotEnoughReporter");
extern ReportPattern *	attackParticipatonReporter;
extern ReportPattern *	defenseParticipatonReporter;
extern ReportPattern *	AttackReporter;
extern ReportPattern *	AttackedReporter;
SkillRule * ambushSkill;
SkillRule * strategySkill;
SkillRule * tacticSkill;
SkillRule * combatSkill;

/*
* Attack Attempt may cause submitting combat request
* or immediate combat resolution in simple case
*/

CombatManager::CombatManager(LocationEntity * location)
{
  location_ = location;
  //cout << "===> CreatingCombatManager for" <<location->print()<<endl;
  combatEngine_ = new BasicCombatEngine;
}





void CombatManager::attackAttempt(TokenEntity * attacker,
				  TokenEntity * defender,OrderLine * orderId,
				  void (*funPtr)(TokenEntity * attacker,
						 TokenEntity * defender,const BATTLE_RESULT result))
{
//   if(attacker->isTraced())
//   {
//     cout << "===> CombatManager::attackAttempt invoked. "<<attacker->print()<< " vs. "<<defender->print() <<endl;
//   }
  addCombatRequest_(new BasicCombatRequest(attacker,defender,orderId,funPtr));
}



void CombatManager::addGuardingRequest(GuardingRequest *guard)
{
  //cout << "()()()()() Guarding request added for " << *(guard->getGuard())<<endl;
  guardingRequests_.push_back(guard);
}



void CombatManager::addPatrolingRequest(GuardingRequest *patrol)
{
  patrolingRequests_.push_back(patrol);
}


void CombatManager::addPillageRequest(PillageRequest *pillage)
{
  pillageRequests_.push_back(pillage);
}



void CombatManager::removePillageRequest(TokenEntity *pillager, bool isReporting)
{
  for (PillageRequestIterator iter  = pillageRequests_.begin();
  iter != pillageRequests_.end(); iter++)
  {
    if((*iter)->getPillager() == pillager)
    {
      delete *iter;
      pillageRequests_.erase(iter);
      return;
    }
  }
  if(isReporting)
  {
    cerr << *pillager << " not found in the list of pillaging units.\n";
  }
}



PillageRequest * CombatManager::findPillageRequest(TokenEntity  *pillager)
{
  for (PillageRequestIterator iter  = pillageRequests_.begin();
  iter != pillageRequests_.end(); iter++)
  {
    if((*iter)->getPillager() == pillager)
    {
      return (*iter);
    }
  }
  return 0;
}




bool compareStrategyRating(const RequestPtr  request1, const RequestPtr  request2)
{
  cout << "  $$$$  Comparing SR of "<< request1->getAttacker() << " vs. "
  << request1->getDefender()<< " with SR of " << request2->getAttacker()
  << " vs. "<< request2->getDefender()<<endl;
  
  if( CombatManager::getFactionLocalStrategyRanking(request1) >=
    CombatManager::getFactionLocalStrategyRanking(request2)    )
    return true;
  else
    return false;
  
}




// add all units of this faction
void CombatManager::addFactionTokensToBattle(FactionEntity * faction,vector<TokenEntity *> & attackers,
					     vector<TokenEntity *> & defenders)
{
  TokenEntity * currentEntity=0;
  FactionEntity * currentFaction = 0;
  // determine side
  if (faction->getTemporaryFlag(COMBAT_SIDE_MASK) == ATTACKER_FLAG)
  {
    for (UnitIterator unitIter  = location_->unitsPresent().begin();
    unitIter != location_->unitsPresent().end(); unitIter++)
    {
      currentEntity = *unitIter;
      currentFaction = currentEntity->getFaction();
      if(currentFaction != faction)
	continue;
      if(currentEntity->combatStanceAtLeast(attackStance))
      {
	if(find(attackers.begin(),attackers.end(),currentEntity) ==
	  attackers.end())
	  attackers.push_back(currentEntity);
      }
    }
    for (ConstructionIterator iter  = location_->constructionsPresent().begin();
    iter != location_->constructionsPresent().end(); iter++)
    {
      currentEntity = *iter;
      currentFaction = currentEntity->getFaction();
      if(currentFaction != faction)
	continue;
      if(currentFaction ==0) // unowned construction
	continue;
      if(currentEntity->combatStanceAtLeast(attackStance))
      {
	if(find(attackers.begin(),attackers.end(),currentEntity) ==
	  attackers.end())
	  attackers.push_back(currentEntity);
      }
    }
    return;
  }
  
  if (faction->getTemporaryFlag(COMBAT_SIDE_MASK) == DEFENDER_FLAG)
  {
    for (UnitIterator unitIter  = location_->unitsPresent().begin();
    unitIter != location_->unitsPresent().end(); unitIter++)
    {
      currentEntity = *unitIter;
      currentFaction = currentEntity->getFaction();
      if(currentFaction != faction)
	continue;
      if(currentEntity->combatStanceAtLeast(defenceStance))
      {
	if(find(defenders.begin(),defenders.end(),currentEntity) ==
	  defenders.end())
	  defenders.push_back(currentEntity);
      }
    }
    for (ConstructionIterator iter  = location_->constructionsPresent().begin();
    iter != location_->constructionsPresent().end(); iter++)
    {
      currentEntity = *iter;
      currentFaction = currentEntity->getFaction();
      if(currentFaction != faction)
	continue;
      if(currentFaction ==0) // unowned construction
	continue;
      if(currentEntity->combatStanceAtLeast(defenceStance))
      {
	if(find(defenders.begin(),defenders.end(),currentEntity) ==
	  defenders.end())
	  defenders.push_back(currentEntity);
      }
    }
    
    return;
  }
}



void CombatManager::process()
{
  FactionEntity * currentFaction;
  FactionEntity * pillagerFaction;
  
  // Check guarding/patrolling requests for conflicts:
  //  any request hostile to another - generate attackRequest
  // (defeated will be deleted from requests)
  for (GuardingRequestIterator iter  = guardingRequests_.begin();
  iter != guardingRequests_.end(); iter++)
  {
    currentFaction = (*iter)->getGuard()->getFaction();
    for (GuardingRequestIterator iter2  = iter+1;
    iter2 != guardingRequests_.end(); ++iter2)
    {
      if(!currentFaction->stanceAtLeast((*iter2)->getGuard(),neutralStance))
      {
	attackAttempt((*iter)->getGuard(),(*iter2)->getGuard(),0,CombatManager::guardingConflictResolution);
      }
      if(!(*iter2)->getGuard()->getFaction()->stanceAtLeast(currentFaction,neutralStance))
      {
	attackAttempt((*iter2)->getGuard(),(*iter)->getGuard(),0,CombatManager::guardingConflictResolution);
      }
    }
  }
  //
  // Pillaging initiated combat
  //
  // Each pillaging request generates combat request against each guarding request // that is non-allied to pillager
  // Also pillaging request generates combat request against each
  // non-friendly pillaging request
  for (PillageRequestIterator iter  = pillageRequests_.begin();
  iter != pillageRequests_.end(); iter++)
  {
    pillagerFaction = (*iter)->getPillager()->getFaction();
    for (GuardingRequestIterator iter2  = guardingRequests_.begin();
    iter2 != guardingRequests_.end(); iter2++)
    {
      currentFaction = (*iter2)->getGuard()->getFaction();
      if(!currentFaction->stanceAtLeast(pillagerFaction,alliedStance))
      {
	attackAttempt((*iter)->getPillager(),(*iter2)->getGuard(),0,0);
      }
    }
    for (PillageRequestIterator iter2  = pillageRequests_.begin();
    iter2 != pillageRequests_.end(); iter2++)
    {
      currentFaction = (*iter2)->getPillager()->getFaction();
      if(!currentFaction->stanceAtLeast(pillagerFaction,friendlyStance))
      {
	attackAttempt((*iter)->getPillager(),(*iter2)->getPillager(),0,0);
      }
    }
  }
  
  
  
  
  
  //
  // Stance-based attacks
  //
  for (UnitIterator iter = location_->unitsPresent().begin();
  iter != location_->unitsPresent().end(); ++iter)
  {
    if( *((*iter)->getCombatStance()) >= *attackStance)
    {
      attackLocalEnemies(*iter);
    }
  }
  for (ConstructionIterator iter = location_->constructionsPresent().begin();
  iter != location_->constructionsPresent().end(); ++iter)
  {
    if( *((*iter)->getCombatStance()) >= *attackStance)
    {
      attackLocalEnemies(*iter);
    }
  }
  
  
  
  
  processCombat();
  // determine guarding/patrolling status
  // 1. Legally owned land
  FactionEntity *legalOwner = location_->getLegalOwner();
  bool alliedGuardsPresent = false;
  bool semiAlliedGuardsPresent = false;
  bool allAllied = true;
  if(legalOwner)
  {
    //    check for requests mutually allied to owner - allied guards
    //		cout << location_<< " Has Legal owner: " << legalOwner<<endl;
    for (GuardingRequestIterator iter  = guardingRequests_.begin();
    iter != guardingRequests_.end(); iter++)
    {
      currentFaction = (*iter)->getGuard()->getFaction();
      if(!legalOwner->stanceAtLeast(currentFaction,alliedStance))
	continue;
      if(!currentFaction->stanceAtLeast(legalOwner,alliedStance))
	continue;
      alliedGuardsPresent = true;
      //		cout << "Allied guards present at " << location_<<endl;
      break;
    }
    
    
    //   check for requests where guard allied to owner
    if(!alliedGuardsPresent)
    {
      for (GuardingRequestIterator iter  = guardingRequests_.begin();
      iter != guardingRequests_.end(); iter++)
      {
	currentFaction = (*iter)->getGuard()->getFaction();
	if(currentFaction ->stanceAtLeast(legalOwner,alliedStance))
	{
	  semiAlliedGuardsPresent = true;
	  break;
	}
      }
    }
  }
  
  if(alliedGuardsPresent || semiAlliedGuardsPresent)
  {
    for (GuardingRequestIterator iter  = guardingRequests_.begin();
    iter != guardingRequests_.end(); iter++)
    {
      
      currentFaction = (*iter)->getGuard()->getFaction();
      if(alliedGuardsPresent)
      {
	if(legalOwner->stanceAtLeast(currentFaction,alliedStance))
	{
	  (*iter)->answerRequest(SUCCESS);
	}
	else
	{
	  (*iter)->getGuard()->addReport(new UnaryMessage
	  (alreadyGuardedReporter,location_),(*iter)->getOrderId(),0);
	  (*iter)->answerRequest(FAILURE);
	}
      }
      else //if (semiAlliedGuardsPresent)
      {
	if(currentFaction ->stanceAtLeast(legalOwner,alliedStance))
	{
	  (*iter)->getGuard()->addReport(new UnaryMessage
	  (alreadyGuardedReporter,location_),(*iter)->getOrderId(),0);
	  (*iter)->answerRequest(SUCCESS);
	  semiAlliedGuardsPresent = true;
	}
	else
	  (*iter)->answerRequest(FAILURE);
      }
      
    }
  }
  else //unowned  land
  {
    allAllied = true;
    for (GuardingRequestIterator iter  = guardingRequests_.begin();
    iter != guardingRequests_.end(); iter++)
    {
      currentFaction = (*iter)->getGuard()->getFaction();
      for (GuardingRequestIterator iter2  = iter+1;
      iter2 != guardingRequests_.end(); ++iter2)
      {
	if(!currentFaction->stanceAtLeast((*iter2)->getGuard(),alliedStance))
	{
	  allAllied = false;
	  break;
	}
	if(!(*iter2)->getGuard()->getFaction()
	  ->stanceAtLeast(currentFaction,alliedStance))
	{
	  allAllied = false;
	  break;
	}
      }
    }
    //   if all requests are mutually allied -
    //         set all of them to guard, ownership  changed to first unit
    //   else all requests fails
    for (GuardingRequestIterator iter  = guardingRequests_.begin();
    iter != guardingRequests_.end(); iter++)
    {
      if(allAllied)
      {
	(*iter)->answerRequest(SUCCESS);
      }
      else
      {
	(*iter)->getGuard()->addReport(new UnaryMessage
	(uncoordinatedGuardReporter,location_),(*iter)->getOrderId(),0);
	(*iter)->answerRequest(FAILURE);
      }
    }
    
    if(allAllied && !(guardingRequests_.empty()))
    {
      //		cout << "Set guarded: " << location_<<endl;
      location_->setGuard((*(guardingRequests_.begin()))->getGuard());
    }
  }
  guardingRequests_.clear();
  
  // Pillage processing:
  // If non-allied to pillager guard present - all pillaging requests fail
  
  
  for (PillageRequestIterator iter  = pillageRequests_.begin();
  iter != pillageRequests_.end(); )
  {
    if(location_->getGuard() == 0)
    {
      iter++;
      continue;
    }
    currentFaction = (*iter)->getPillager()->getFaction();
    if(alliedGuardsPresent || semiAlliedGuardsPresent)
    {
      if(!legalOwner->stanceAtLeast(currentFaction,alliedStance))
      {
	(*iter)->answerRequest(0);
	(*iter)->getPillager()->addReport(new
	UnaryMessage(pillagingGuardedReporter,location_),(*iter)->getOrderId(),0);
	delete *iter;
	pillageRequests_.erase(iter);
	continue;
      }
    }
    else if(!location_->getGuard()->getFaction()->
      stanceAtLeast(currentFaction,alliedStance))
    {
      (*iter)->answerRequest(0);
      (*iter)->getPillager()->addReport(new
      UnaryMessage(pillagingGuardedReporter,location_),(*iter)->getOrderId(),0);
      delete *iter;
      pillageRequests_.erase(iter);
      continue;
    }
    else
    {
      iter++;
    }
  }
  // Otherwise distribute money proportionally to number of pillagers
  int numPillagers = 0;
  int pillageShare = 1;
  for (PillageRequestIterator iter  = pillageRequests_.begin();
  iter != pillageRequests_.end(); iter++)
  {
    numPillagers += (*iter)->getPillager()->getFiguresNumber();
  }
  UnitEntity * unit;
  if(numPillagers < 10 )  // Pillagers should have at least 20 figures in order to pillage
  {
    for (PillageRequestIterator iter  = pillageRequests_.begin();
    iter != pillageRequests_.end(); iter++)
    {
      unit = (*iter)->getPillager();
      unit->addReport(new SimpleMessage(pillager20FiguresReporter),0,0);
    }
    
    pillageRequests_.clear();
    return;
  }
  
  
  if(location_->getPopulation()/ 50 > numPillagers) // Need 1 soldier for 50 population to pillage
  {
    for (PillageRequestIterator iter  = pillageRequests_.begin();
    iter != pillageRequests_.end(); iter++)
    {
      unit = (*iter)->getPillager();
      unit->addReport(new BinaryMessage( pillagerNotEnoughReporter,
					 new IntegerData(location_->getPopulation()/50), location_),0,0);
    }
    
    pillageRequests_.clear();
    return;
  }
  
  
  
  
  if(numPillagers)
  {
    location_ ->pillage();
    pillageShare = location_->getTaxes() * 2 * 100 / numPillagers;
    
    /*	if(location_->getLegalOwner())
    {
      location_->getLegalOwner()->addReport(new UnaryMessage(pillagingOwnerReporter, location_));
  }*/
    for (PillageRequestIterator iter  = pillageRequests_.begin();
    iter != pillageRequests_.end(); iter++)
    {
      (*iter)->answerRequest(pillageShare * numPillagers / gameFacade->getGameConfig()->daysInMonth/ 100);
    }
  }
  pillageRequests_.clear();
  
}
//===================================================================
// Determine combat particicipants  and launch combat
//
//================================================================
void CombatManager::processCombat()
{
  if(combatRequests_.empty())
    return;
  // Determine combat participants
  vector<TokenEntity *> attackers;
  vector<TokenEntity *> defenders;
  vector  <BasicCombatRequest *> currentRequests; // requests that are served
  // by one battle
  
  // mark all factions as uninvolved and create list of factions
  vector <FactionEntity *> localFactions;
  FactionEntity * currentFaction = 0;
  BATTLE_RESULT result;
  
  for (UnitIterator unitIter  = location_->unitsPresent().begin();
  unitIter != location_->unitsPresent().end(); unitIter++)
  {
    currentFaction = (*unitIter)->getFaction();
    if(find(localFactions.begin(),localFactions.end(),currentFaction)
      ==localFactions.end())
    {
      currentFaction->setTemporaryFlag(UNINVOLVED,COMBAT_SIDE_MASK);
      localFactions.push_back(currentFaction);
    }
  }
  for (ConstructionIterator iter  = location_->constructionsPresent().begin();
  iter != location_->constructionsPresent().end(); iter++)
  {
    currentFaction = (*iter)->getFaction();
    if(currentFaction ==0) // unowned construction
      continue;
    if(find(localFactions.begin(),localFactions.end(),currentFaction)
      ==localFactions.end())
    {
      currentFaction->setTemporaryFlag(UNINVOLVED,COMBAT_SIDE_MASK);
      localFactions.push_back(currentFaction);
    }
  }
  
  
  
  while (!combatRequests_.empty())
  {
    // Check all remaining requests for validity
    // And select request with worst SR
    BasicCombatRequest * currentRequest = *(combatRequests_.begin());
    for(BasicCombatRequestIterator iter = combatRequests_.begin();
    iter != combatRequests_.end();)
    {
      assert(*iter);
     // cout << (*iter)->print()<<endl;
      if((*iter)->isValid())
      {
	if( getFactionLocalStrategyRanking(currentRequest) >
	  getFactionLocalStrategyRanking(*iter)    )
	  currentRequest = *iter;
	++iter;
      }
      else
      {
	cout <<" -- invalid request ["<<(*iter)->print()<<"] deleted\n";
	combatRequests_.erase(iter);
      }
    }
    if(combatRequests_.empty()) // all remained request appeared to be invalid
      break;
    // launch attack
    
    cout <<"--------------------- Combat  Start ---------------------"<<endl;
    determineCombatParticipants(currentRequest,attackers,defenders,
				localFactions,currentRequests);
  BinaryMessage * attackMessage = new BinaryMessage(AttackReporter,attackers[0],defenders[0]);
  BinaryMessage * attackedMessage = new BinaryMessage(AttackedReporter,defenders[0],attackers[0]);
  location_->addReport(attackMessage,0,0);
  location_->addBattle();
  //  attackers[0]->getFaction()->addReport(attackMessage,orderId,0);
  //  defenders[0]->getFaction()->addReport(attackMessage,orderId,0);
  //  attackers[0]->addReport(attackMessage,0,0);
  //  defenders[0]->addReport(attackMessage,0,0);
  // participation message
  for(TokenIterator iter = attackers.begin();
  iter != attackers.end();++iter)
  {
    if(iter == attackers.begin())
    {
      (*iter)->addReport(attackMessage,0,0);
    }
    else
    {
      (*iter)->addReport(new TertiaryMessage 
      (attackParticipatonReporter,(*iter),attackers[0],defenders[0]), 0,0);
    }
    
  }
  for(TokenIterator iter = defenders.begin();
  iter != defenders.end();++iter)
  {
    if(iter == defenders.begin())
    {
      (*iter)->addReport(attackedMessage,0,0);
    }
    else
    {
      (*iter)->addReport(new TertiaryMessage 
      (defenseParticipatonReporter,(*iter),defenders[0],attackers[0]), 0,0);
    }
    
  }
  
  result = currentRequest->getCombatEngine()->processBattle(attackers,defenders);
  cout <<"---- "<<attackers[0]->print()<< " vs. "<<defenders[0]->print()<<endl;
  cout <<"--------------------- Combat End ------------------------"<<endl;
  // postprocess all request participated in battle and remove them all
  //		 cout<< "current Combat Requests:"<< endl;
  //    for(BasicCombatRequestIterator iter = currentRequests.begin();
  //          iter != currentRequests.end(); ++iter)
  //          {
    //             cout << (*iter)->print() << endl;
    //           }
    //     cout<< "current Combat Requests end"<< endl;
    //     cout<< "processing current Combat Requests"<< endl;
    for(BasicCombatRequestIterator iter = currentRequests.begin();
    iter != currentRequests.end();++iter)
    {
                  // cout << "request: "<<(*iter)->print() << endl;
      (*iter)->answerRequest(result);
      BasicCombatRequestIterator current =
      find(combatRequests_.begin(),combatRequests_.end(),*iter);
      assert(current !=combatRequests_.end());
      delete *iter;
      combatRequests_.erase(current);
    }
    currentRequests.clear();
    //     cout<< "end of processing current Combat Requests"<< endl;
    // Remove dead requests
    GuardingRequest * request;
    PillageRequest * pillage;
    for(TokenIterator iter = attackers.begin();
    iter != attackers.end();++iter)
    {
      if(!(*iter)->isAlive()) // unit is dead
      {
	request =  findGuardingRequest(*iter);
	pillage =  findPillageRequest(*iter);
	
	if(request)
	{
	  removeGuardingRequest(*iter);
	}
	
	if(pillage)
	{
	  removePillageRequest(*iter);
	}
      }
    }
    for(TokenIterator iter = defenders.begin();
    iter != defenders.end();++iter)
    {
      if(!(*iter)->isAlive()) // unit is dead
      {
	request =  findGuardingRequest(*iter);
	pillage =  findPillageRequest(*iter);
	
	if(request)
	{
	  removeGuardingRequest(*iter);
	}
	
	if(pillage)
	{
	  removePillageRequest(*iter);
	}
      }
    }
    // Remove guarding and pillaging request of loosing side
    switch(result)
    {
      case DEFENDER_VICTORY:
      {
	for(TokenIterator iter = attackers.begin();
	iter != attackers.end();++iter)
	{
	  if(!(*iter)->isAlive()) // unit is dead
	    continue; // all requests are already removed
	    request =  findGuardingRequest(*iter);
	  if(request)
	  {
	    // report can't guard after loosing the battle
	    (*iter)->addReport(new
	    UnaryMessage( guardBattleLossReporter, *iter),
			       request->getOrderId(), 0);
			       removeGuardingRequest(*iter);
	  }
	  
	  pillage =  findPillageRequest(*iter);
	  if(pillage)
	  {
	    // report can't pillage after loosing the battle
	    (*iter)->addReport(new
	    UnaryMessage( pillagerBattleLossReporter, *iter),
			       pillage->getOrderId(), 0);
			       removePillageRequest(*iter);
	  }
	}
	break;
      }
      case ATTACKER_VICTORY:
      {
	for(TokenIterator iter = defenders.begin();
	iter != defenders.end();++iter)
	{
	  if(!(*iter)->isAlive()) // unit is dead
	    continue; // all requests are already removed
	    request =  findGuardingRequest(*iter);
	  if(request)
	  {
	    // report can't guard after loosing the battle
	    (*iter)->addReport(new
	    UnaryMessage( guardBattleLossReporter, *iter),
			       request->getOrderId(), 0);
			       removeGuardingRequest(*iter);
	  }
	  
	  pillage =  findPillageRequest(*iter);
	  if(pillage)
	  {
	    (*iter)->addReport(new
	    UnaryMessage( pillagerBattleLossReporter, *iter),
			       pillage->getOrderId(), 0);
			       // report can't pillage after loosing the battle
			       removePillageRequest(*iter);
	  }
	}
	break;
      }
      case DRAW:
      default:
	break;
    }
    attackers.clear();
    defenders.clear();
  }//  end of while
}


  
  
  //===================================================================
  // Determine combat particicipants
  //
  //================================================================
  void CombatManager::determineCombatParticipants(BasicCombatRequest *currentRequest,
						  vector<TokenEntity *> & attackers,
						  vector<TokenEntity *> & defenders,
						  vector <FactionEntity *> &localFactions,
						  vector  <BasicCombatRequest *> &currentRequests)
{
  TokenEntity * currentEntity=0;
  FactionEntity * currentFaction = 0;
  FactionEntity * currentTarget = 0;
  FactionEntity * attackingFaction= 0;
  FactionEntity * defendingFaction = 0;
  
  assert(currentRequest) ;
  currentRequests.push_back(currentRequest);
  
  attackers.push_back(currentRequest->getAttacker());
  defenders.push_back(currentRequest->getDefender());
  attackingFaction = currentRequest->getAttacker()->getFaction();
  attackingFaction->setTemporaryFlag(ATTACKER_FLAG,COMBAT_SIDE_MASK);
  defendingFaction = currentRequest->getDefender()->getFaction();
  defendingFaction->setTemporaryFlag(DEFENDER_FLAG,COMBAT_SIDE_MASK);
  // Check all Tokens:
  // Tokens of attacker and defender join battle if combat stances allow
  // For other Tokens: if their factions ally to one side and not allied to another - join to ally.
  
  for (UnitIterator unitIter  = location_->unitsPresent().begin(); unitIter != location_->unitsPresent().end(); unitIter++)
  {
    currentEntity = *unitIter;
    currentFaction = currentEntity->getFaction();
    if(currentEntity == currentRequest->getAttacker())
      continue; // it is already involved
      if(currentEntity == currentRequest->getDefender())
	continue; // it is already involved
	
	if(currentFaction->getTemporaryFlag(COMBAT_SIDE_MASK) != REFRAIN_FLAG)
	  stanceBasedInvolvement(attackers, defenders, currentEntity,
				  currentFaction,attackingFaction,defendingFaction);
  }
  for (ConstructionIterator iter  = location_->constructionsPresent().begin(); iter != location_->constructionsPresent().end(); iter++)
  {
    currentEntity = *iter;
    if(!currentEntity->mayParticipateInCombat())
      continue;
    currentFaction = currentEntity->getFaction();
    if(currentFaction ==0) // unowned construction
      continue;
    if(currentEntity == currentRequest->getAttacker())
      continue; // it is already involved
      if(currentEntity == currentRequest->getDefender())
	continue; // it is already involved
	
	if(currentFaction->getTemporaryFlag(COMBAT_SIDE_MASK) != REFRAIN_FLAG)
	  stanceBasedInvolvement(attackers, defenders, currentEntity,
				  currentFaction,attackingFaction,defendingFaction);
  }
  
  
  // Check all guarding requests:
  // If faction not ally of attacker and friend of defender - join defender
  
  for (GuardingRequestIterator iter  = guardingRequests_.begin();
  iter != guardingRequests_.end(); iter++)
  {
    
    currentEntity = (*iter)->getGuard();
    currentFaction = currentEntity->getFaction();
    if(currentFaction->getTemporaryFlag(COMBAT_SIDE_MASK) != UNINVOLVED)
      continue; // Refraining faction allied to both sides and will not join anyway
      if(!currentFaction->stanceAtLeast(attackingFaction,alliedStance ))
      {
	if( currentFaction->stanceAtLeast(defendingFaction,friendlyStance ) )
	{
	  defenders.push_back(currentEntity);
	  currentFaction->setTemporaryFlag(DEFENDER_FLAG,COMBAT_SIDE_MASK);
	  addFactionTokensToBattle(currentFaction,attackers, defenders);
	}
      }
      
  }
  // Check all patrolling requests:
  // If faction not ally of attacker and friend of defender - join defender
  for (GuardingRequestIterator iter  = patrolingRequests_.begin();
  iter != patrolingRequests_.end(); iter++)
  {
    
    currentEntity = (*iter)->getGuard();
    currentFaction = currentEntity->getFaction();
    if(currentFaction->getTemporaryFlag(COMBAT_SIDE_MASK) != UNINVOLVED)
      continue; // Refraining faction allied to both sides and will not join anyway
      if(!currentFaction->stanceAtLeast(attackingFaction,alliedStance ))
      {
	if( currentFaction->stanceAtLeast(defendingFaction,friendlyStance ) )
	{
	  defenders.push_back(currentEntity);
	  currentFaction->setTemporaryFlag(DEFENDER_FLAG,COMBAT_SIDE_MASK);
	  addFactionTokensToBattle(currentFaction,attackers, defenders);
	}
      }
      
  }
  
	  // If uninvolved  faction has combat request against one of the faction
	  // already participating in battle it may join battle
	  
	  unsigned long involvement= 0;
	  vector  <BasicCombatRequest *> factionCombatRequests;
	  for (FactionIterator iter  = localFactions.begin();
	  iter != localFactions.end(); iter++)
	  {
	    currentFaction = *iter;
	    involvement= 0;
	    if((*iter)->getTemporaryFlag(COMBAT_SIDE_MASK) == UNINVOLVED)
	    {
	      factionCombatRequests.clear();
	      // create list of faction's combat requests
	      // and  calculate involvement
	      for (BasicCombatRequestIterator requestIter  = combatRequests_.begin();
	      requestIter != combatRequests_.end(); requestIter++)
	      {
		if ((*requestIter)->getAttacker()->getFaction() == (*iter))
		{
		  involvement |= (*requestIter)->getDefender()->getFaction()->getTemporaryFlag(COMBAT_SIDE_MASK);
		  factionCombatRequests.push_back(*requestIter);
		}
	      }
	      // ignore refraining and consider only attackers or defenders
	      involvement = involvement &  ( DEFENDER_FLAG | ATTACKER_FLAG);
	      // All combat request against defenders. Then faction joins to attacker
	      if(involvement == DEFENDER_FLAG )
	      {
		joinAttackerSide(*iter, factionCombatRequests, currentRequests,
				  attackers,defenders);
				  continue;
	      }
	      // All combat request against attacker side. Then faction joins to defender
	      if(involvement == ATTACKER_FLAG )
	      {
		joinDefenderSide(*iter, factionCombatRequests, currentRequests,
				  attackers,defenders);
				  continue;
	      }
	      
	      // faction has requests both against attackers and defenders.
	      // Calculate combat rating
	      if(involvement == (DEFENDER_FLAG | ATTACKER_FLAG))
	      {
		//calculate combat rating of both sides
		int defenceRating = calculateCombatRating(defenders);
		int attackRating = calculateCombatRating(attackers);
		int factionDefenceRating =calculateFactionCombatRating((*iter),defenders,defenceStance);
		int factionAttackRating =calculateFactionCombatRating((*iter),attackers,attackStance);
		cout << "           Attackers CR " << attackRating <<endl;
		cout << "           Defenders CR " << defenceRating <<endl;
		cout << "           Faction  ACR " <<  factionAttackRating<<endl;
		cout << "           Faction  DCR " <<  factionDefenceRating<<endl;
		if( (attackRating - defenceRating) > factionDefenceRating)
		{
		  joinDefenderSide(*iter, factionCombatRequests, currentRequests,
				    attackers,defenders);
				    continue;
		}
		
		if( (defenceRating - attackRating) > factionAttackRating)
		{
		  joinAttackerSide(*iter, factionCombatRequests, currentRequests,
				    attackers,defenders);
				    continue;
		}
		// else stay aside
	      }
	    }
	  }
	  // If already involved  faction has combat request against one of the faction
	  // already participating in battle, this request will be considered as
	  // already served (i.e. added to current requests) if request target is on opposite side
	  for (BasicCombatRequestIterator requestIter  = combatRequests_.begin();
	  requestIter != combatRequests_.end(); requestIter++)
	  {
	    if((*requestIter) == currentRequest)// It is already included
	      continue;
	    currentFaction = (*requestIter)->getAttacker()->getFaction();
	    currentTarget = (*requestIter)->getDefender()->getFaction();
	    if( (currentFaction->getTemporaryFlag(COMBAT_SIDE_MASK) == ATTACKER_FLAG) &&
	      (currentTarget->getTemporaryFlag(COMBAT_SIDE_MASK) == DEFENDER_FLAG) )
	    {
	      if(find(currentRequests.begin(),currentRequests.end(),*requestIter) ==currentRequests.end())
	      {
		currentRequests.push_back(*requestIter);
		// Now add attacker and defender to participants
		if(find(attackers.begin(),attackers.end(),(*requestIter)->getAttacker())==attackers.end())
		{
		  attackers.push_back((*requestIter)->getAttacker());
		}
		if(find(defenders.begin(),defenders.end(),(*requestIter)->getDefender())==defenders.end())
		{
		  defenders.push_back((*requestIter)->getDefender());
		}
	      }
	      continue;
	    }
	    if( (currentFaction->getTemporaryFlag(COMBAT_SIDE_MASK) == DEFENDER_FLAG) &&
	      (currentTarget->getTemporaryFlag(COMBAT_SIDE_MASK) == ATTACKER_FLAG ) )
	    {
	      
	      if(find(currentRequests.begin(),currentRequests.end(),*requestIter) ==currentRequests.end())
	      {
		currentRequests.push_back(*requestIter);
		// Now add attacker and defender to participants
		if(find(attackers.begin(),attackers.end(),(*requestIter)->getDefender())==attackers.end())
		{
		  attackers.push_back((*requestIter)->getDefender());
		}
		if(find(defenders.begin(),defenders.end(),(*requestIter)->getAttacker())==defenders.end())
		{
		  defenders.push_back((*requestIter)->getAttacker());
		}
	      }
	      continue;
	    }
	  }
	}
	
	//================================================================
	//
	//================================================================
	void CombatManager::joinAttackerSide(FactionEntity * currentFaction,
					     vector  <BasicCombatRequest *> &factionCombatRequests,
					     vector  <BasicCombatRequest *> &currentRequests,
					     vector<TokenEntity *> & attackers,
					     vector<TokenEntity *> & defenders)
	{
	  currentFaction->setTemporaryFlag(ATTACKER_FLAG,COMBAT_SIDE_MASK);
	  for (BasicCombatRequestIterator requestIter  = factionCombatRequests.begin();
	  requestIter != factionCombatRequests.end(); requestIter++)
	  {
	    // add attack initiators and  units to attacker
	    // and attack targets to defender
	    // add requests
	    if((*requestIter)->getDefender()->getFaction()->getTemporaryFlag(COMBAT_SIDE_MASK) == DEFENDER_FLAG)
	    {
	      attackers.push_back((*requestIter)->getAttacker());
	      if(find(defenders.begin(),defenders.end(),(*requestIter)->getDefender()) == defenders.end())
		defenders.push_back((*requestIter)->getDefender());
	      currentRequests.push_back(*requestIter);
	    }
	  }
	  // add appropriate units to attacker
	  addFactionTokensToBattle(currentFaction,attackers, defenders);
	}
	
	//================================================================
	//
	//================================================================
	void CombatManager::joinDefenderSide(FactionEntity * currentFaction,
					     vector  <BasicCombatRequest *> &factionCombatRequests,
					     vector  <BasicCombatRequest *> &currentRequests,
					     vector<TokenEntity *> & attackers,
					     vector<TokenEntity *> & defenders)
	{
	  currentFaction->setTemporaryFlag(DEFENDER_FLAG,COMBAT_SIDE_MASK);
	  for (BasicCombatRequestIterator requestIter  = factionCombatRequests.begin();
	  requestIter != factionCombatRequests.end(); requestIter++)
	  {
	    // add attack initiators and  units to defender
	    // and attack targets to attacker (if they are not already there)
	    // add requests
	    if((*requestIter)->getDefender()->getFaction()->getTemporaryFlag(COMBAT_SIDE_MASK) == ATTACKER_FLAG)
	    {
	      defenders.push_back((*requestIter)->getAttacker());
	      if(find(attackers.begin(),attackers.end(),(*requestIter)->getDefender()) == attackers.end())
		attackers.push_back((*requestIter)->getDefender());
	      currentRequests.push_back(*requestIter);
	    }
	  }
	  // add appropriate units to attacker
	  addFactionTokensToBattle(currentFaction,attackers, defenders);
	}
	
	//================================================================
	// Tokens of Attacker and Defender faction join based on combat stances
	// If faction allied to one side and not allied to another
	// It joins the battle
	//================================================================
	void CombatManager::stanceBasedInvolvement(vector<TokenEntity *> & attackers,
						   vector<TokenEntity *> & defenders,
						   TokenEntity * currentEntity,
						   FactionEntity * currentFaction,
						   FactionEntity * attackingFaction,
						   FactionEntity * defendingFaction)
{
  // Tokens of Attacker faction join based on combat stances only
  if(currentFaction == attackingFaction)
  {
    if(currentEntity->combatStanceAtLeast(attackStance))
    {
      attackers.push_back(currentEntity);
    }
    return;
  }
  
  // Tokens of Defender faction join based on combat stances  only
  if(currentFaction == defendingFaction)
  {
    if(currentEntity->combatStanceAtLeast(defenceStance))
    {
      defenders.push_back(currentEntity);
    }
    return;
  }
  
  // Token's faction allied to attacker and not allied to defender - join attacker
  if( currentFaction->stanceAtLeast(attackingFaction,alliedStance ) )
  {
    if( !currentFaction->stanceAtLeast(defendingFaction,alliedStance ) )
    {
      if(currentEntity->combatStanceAtLeast(attackStance))
      {
	attackers.push_back(currentEntity);
	currentFaction->setTemporaryFlag(ATTACKER_FLAG,COMBAT_SIDE_MASK);
      }
      return;
    }
    else
      currentFaction->setTemporaryFlag(REFRAIN_FLAG,COMBAT_SIDE_MASK);
    return;
  }
  
  // Token's faction allied to defender and not allied to attacker - join defender
  if( currentFaction->stanceAtLeast(defendingFaction,alliedStance ) )
  {
    if( !currentFaction->stanceAtLeast(attackingFaction,alliedStance ) )
    {
      if(currentEntity->combatStanceAtLeast(defenceStance))
      {
	defenders.push_back(currentEntity);
	currentFaction->setTemporaryFlag(DEFENDER_FLAG,COMBAT_SIDE_MASK);
      }
      return;
    }
    else
      currentFaction->setTemporaryFlag(REFRAIN_FLAG,COMBAT_SIDE_MASK);
    return;
  }
}



void CombatManager::resolve(vector  <BasicCombatRequest *> & requests)
{
  
}



void CombatManager::addCombatRequest_(BasicCombatRequest *request)
{
  combatRequests_.push_back(request);
}



int CombatManager::getFactionLocalStrategyRanking(BasicCombatRequest *request)
{
  FactionEntity * faction = request->getAttacker()->getFaction();
  LocationEntity * location = request->getAttacker()->getLocation();
  // SR = Strategy * 100 + tactics * 10 + combat
  TokenEntity * currentEntity;
  int bestStrategyExperience =0;
  int bestTacticExperience   =0;
  int bestCombatExperience   =0;
  int currentExperience =0;
  for (UnitIterator unitIter  = location->unitsPresent().begin(); unitIter != location->unitsPresent().end(); unitIter++)
  {
    currentEntity = *unitIter;
    if(currentEntity->getFaction() != faction)
      continue;
    currentExperience = currentEntity->getSkillPoints(strategySkill);
    if(currentExperience > bestStrategyExperience)
      bestStrategyExperience = currentExperience;
    
    currentExperience = currentEntity->getSkillPoints(tacticSkill);
    if(currentExperience > bestTacticExperience)
      bestTacticExperience = currentExperience;
    
    currentExperience = currentEntity->getSkillPoints(combatSkill);
    if(currentExperience > bestCombatExperience)
      bestCombatExperience = currentExperience;
  }
  for (ConstructionIterator iter  = location->constructionsPresent().begin(); iter != location->constructionsPresent().end(); iter++)
  {
    currentEntity = *iter;
    if(currentEntity->getFaction() != faction)
      continue;
    currentExperience = currentEntity->getSkillPoints(strategySkill);
    if(currentExperience > bestStrategyExperience)
      bestStrategyExperience = currentExperience;
    
    currentExperience = currentEntity->getSkillPoints(tacticSkill);
    if(currentExperience > bestTacticExperience)
      bestTacticExperience = currentExperience;
    
    currentExperience = currentEntity->getSkillPoints(combatSkill);
    if(currentExperience > bestCombatExperience)
      bestCombatExperience = currentExperience;
  }
  return  bestStrategyExperience * 100 +
  bestTacticExperience * 10 +
  bestCombatExperience;
}



int CombatManager::calculateCombatRating(vector<TokenEntity *> & side)
{
  int attackRating = 0;
  int defenceRating = 0;
  
  for (TokenIterator iter  = side.begin();
  iter != side.end(); iter++)
  {
    attackRating +=  (*iter)->getAttackRating();
    defenceRating +=  (*iter)->getDefenceRating();
  }
  return   attackRating * defenceRating;
}


// add all units of this faction
int CombatManager::calculateFactionCombatRating(FactionEntity * faction,vector<TokenEntity *> & side, CombatStanceVariety * stance)
{
  TokenEntity * currentEntity=0;
  int attackRating = 0;
  int defenceRating = 0;
  // determine side
  for (UnitIterator unitIter  = location_->unitsPresent().begin(); unitIter != location_->unitsPresent().end(); unitIter++)
  {
    currentEntity = *unitIter;
    if(currentEntity->getFaction() != faction)
      continue;
    if(currentEntity->combatStanceAtLeast(stance))
    {
      attackRating  +=  currentEntity->getAttackRating();
      defenceRating +=  currentEntity->getDefenceRating();
    }
  }
  for (ConstructionIterator iter  = location_->constructionsPresent().begin(); iter != location_->constructionsPresent().end(); iter++)
  {
    currentEntity = *iter;
    if(currentEntity->getFaction() != faction)
      continue;
    if(currentEntity->getFaction() ==0) // unowned construction
      continue;
    if(currentEntity->combatStanceAtLeast(stance))
    {
      attackRating  +=  currentEntity->getAttackRating();
      defenceRating +=  currentEntity->getDefenceRating();
    }
  }
  return   attackRating * defenceRating;
}




void CombatManager::guardingConflictResolution(TokenEntity * attacker, TokenEntity * defender,const BATTLE_RESULT  result)
{
  CombatManager * combat = dynamic_cast<CombatManager *>(defender->getLocation()->getCombatManager());
  GuardingRequest * attackerGuardRequest  = combat->findGuardingRequest(attacker);
  GuardingRequest * defenderGuardRequest  = combat->findGuardingRequest(defender);
  switch(result)
  {
    case ATTACKER_VICTORY:
    {
      combat->removeGuardingRequest(defender);
      defenderGuardRequest->answerRequest(INVALID);
      break;
    }
    case DEFENDER_VICTORY:
    {
      combat->removeGuardingRequest(attacker);
      attackerGuardRequest->answerRequest(INVALID);
      break;
    }
    case DRAW:
    {
      combat->removeGuardingRequest(defender);
      combat->removeGuardingRequest(attacker);
      defenderGuardRequest->answerRequest(FAILURE);
      attackerGuardRequest->answerRequest(FAILURE);
      break;
    }
    case BATTLE_ERROR:
      //case BATTLE_UNDEFINED: temporary
    default:
      return;
  }
  
}



void CombatManager::removeGuardingRequest(TokenEntity *guard)
{
  for (GuardingRequestIterator iter  = guardingRequests_.begin();
  iter != guardingRequests_.end(); iter++)
  {
    if((*iter)->getGuard() == guard)
    {
      delete *iter;
      guardingRequests_.erase(iter);
      return;
    }
  }
  cerr << *guard << " not found in the list of guarding units.\n";
}



GuardingRequest * CombatManager::findGuardingRequest(TokenEntity *guard)
{
  for (GuardingRequestIterator iter  = guardingRequests_.begin();
  iter != guardingRequests_.end(); iter++)
  {
    if((*iter)->getGuard() == guard)
    {
      return *iter;
    }
  }
  return 0;
}



void CombatManager::attackLocalEnemies(TokenEntity * currentToken)
{
  for (UnitIterator iter = location_->unitsPresent().begin();
  iter != location_->unitsPresent().end(); ++iter)
  {
    if(!currentToken->getFaction()->stanceAtLeast(*iter,hostileStance))
    {
      if(currentToken->mayInterractTokenEntity(*iter))
	attackAttempt(currentToken,*iter,0,0);
    }
  }
  for (ConstructionIterator iter = location_->constructionsPresent().begin();
  iter != location_->constructionsPresent().end(); ++iter)
  {
    if(!currentToken->getFaction()->stanceAtLeast(*iter,hostileStance))
    {
      if(currentToken->mayInterractTokenEntity(*iter))
	attackAttempt(currentToken,*iter,0,0);
    }
  }
  
}
