/***************************************************************************
                          CombatManager.h
  Similar to BasicConflict responsible for combat processing
                             -------------------
    begin                : Mon May 24 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#if !defined(COMBAT_MANAGER_H)
  #define COMBAT_MANAGER_H

#include  <vector>
#include "BasicCombatManager.h"
class LocationEntity;
class BasicCombatRequest;
class FactionEntity;
class TokenEntity;
class CombatStanceVariety;
class SkillRule;
#define UNINVOLVED    0x0
#define ATTACKER_FLAG 0x1
#define DEFENDER_FLAG 0x2
#define REFRAIN_FLAG  0x4
#define COMBAT_SIDE_MASK 0x7

class GuardingRequest;
class PillageRequest;
class EquipmentSlotVariety;

using namespace std;
typedef BasicCombatRequest * RequestPtr;

class CombatManager : public BasicCombatManager {
public:
    CombatManager(LocationEntity * location);
   ~CombatManager(){}
   void attackAttempt(TokenEntity * attacker,
                    TokenEntity * defender,OrderLine * orderId = 0,
                    void (*funPtr)(TokenEntity * attacker, TokenEntity * defender,const BATTLE_RESULT result) = 0);
   void addGuardingRequest(GuardingRequest *guard);
   void removeGuardingRequest(TokenEntity *guard);
   GuardingRequest * findGuardingRequest(TokenEntity  *guard);
   void addPatrolingRequest(GuardingRequest *patrol);
   void addPillageRequest(PillageRequest *pillage);
   void removePillageRequest(TokenEntity *pillager);
   PillageRequest * findPillageRequest(TokenEntity  *pillager);
   void process();        // Guarding, patrolling Combat
   void processCombat(); // Determine combat participants
//	 void postCombat(vector<TokenEntity *> & attackers,
//                   vector<TokenEntity *> & defenders,const BATTLE_RESULT result);
   void resolve(vector  <BasicCombatRequest *> & requests);
   void initialize(){}

   void determineCombatParticipants(BasicCombatRequest *currentRequest,
                     vector<TokenEntity *> & attackers,
                     vector<TokenEntity *> & defenders,
                     vector <FactionEntity *> &localFactions,
                     vector  <BasicCombatRequest *> &currentRequests);
   static int   getFactionLocalStrategyRanking(BasicCombatRequest *request) ;
   static void  stanceBasedInvolvement(vector<TokenEntity *> & attackers,
                              vector<TokenEntity *> & defenders,
                              TokenEntity * currentEntity,
                              FactionEntity * currentFaction,
                              FactionEntity * attackingFaction,
                              FactionEntity * defendingFaction);
   void joinAttackerSide(FactionEntity * currentFaction,
                vector  <BasicCombatRequest *> &factionCombatRequests,
                vector  <BasicCombatRequest *> &currentRequests,
                vector<TokenEntity *> & attackersattackers,
                vector<TokenEntity *> & attackersdefenders);
   void joinDefenderSide(FactionEntity * currentFaction,
                vector  <BasicCombatRequest *> &factionCombatRequests,
                vector  <BasicCombatRequest *> &currentRequests,
                vector<TokenEntity *> & attackersattackers,
                vector<TokenEntity *> & attackersdefenders);
   void addFactionTokensToBattle(FactionEntity * faction,
	 						  vector<TokenEntity *> & attackers,
                vector<TokenEntity *> & defenders);
     int calculateCombatRating(vector<TokenEntity *> & side);
     int calculateFactionCombatRating(FactionEntity * faction,
		            vector<TokenEntity *> & side,
                CombatStanceVariety * stance);
     static void guardingConflictResolution(TokenEntity * attacker,
		            TokenEntity * defender,const BATTLE_RESULT  result);
		 void CombatManager::attackLocalEnemies(TokenEntity * currentToken);
protected:
// static      bool compareStrategyRating(const RequestPtr  request1, const RequestPtr  request2);
virtual void addCombatRequest_(BasicCombatRequest *request);
  vector  <BasicCombatRequest *> combatRequests_;
  vector  <GuardingRequest *> guardingRequests_;
  vector  <GuardingRequest *> patrolingRequests_;
  vector  <PillageRequest *> pillageRequests_;
  LocationEntity * location_;
private:


};
extern SkillRule * ambushSkill;
extern SkillRule * strategySkill;
extern SkillRule * tacticSkill;
extern SkillRule * combatSkill;
extern SkillRule * parrySkill;
extern SkillRule * meleeSkill;
extern SkillRule * missileSkill;
extern EquipmentSlotVariety * weaponSlot;
//extern CombatManager combatManager;
#endif // COMBAT_MANAGER_H
