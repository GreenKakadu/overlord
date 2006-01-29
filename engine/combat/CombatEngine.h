/***************************************************************************
                      CombatEngine.h
               Standard Engine for processing  combat
                          ------------------
    begin                : Mon Oct 26 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef COMBAT_ENGINE_H
#define COMBAT_ENGINE_H

#include "BasicCombatEngine.h"
#include "ItemElement.h"
#include "BattleInstance.h"


class BattleField;
class LocationEntity;
class ProcessingMode;
class BattleInstance;

class CombatEngine : public BasicCombatEngine
{
public:
    CombatEngine();

    ~CombatEngine();

     BATTLE_RESULT processBattle(vector< TokenEntity * >& attackers,
		 vector< TokenEntity * >& defenders);
     void preProcess();
		 void preProcessRound(int round);
		 void postProcessRound(int round);
		 void processRound(int round);
		 void processRoutingRound();
		 void postProcess();
		 bool isBattleEnd();
		 void addToDead(TokenEntity *dead);
		inline void addAttackerLoss(int value) {attackerLossCount_ +=value;}
		inline void addDefenderLoss(int value) {defenderLossCount_ +=value;}
		inline void decreaseAliveAttackers(int value){attackersAliveCount_ -=value;}
		inline void decreaseAliveDefender(int value) {defendersAliveCount_ -=value;}
   inline void clearWoundlessRoundCounter() { woundlessRoundCounter_= 0;}
   inline int getWoundlessRoundCounter() const {return woundlessRoundCounter_;}
	 bool sideMoralCheck(vector <TokenEntity *> * side, int modifier);
//	 bool instanceMoralCheck(BattleInstance * instance);
	inline int getCurrentInitiativeSegment() const {return currentInitiativeSegment_;}
	int calculateSideAmbush(vector <TokenEntity *> * side);
	int calculateSideStrategy(vector <TokenEntity *> * side);
	int calculateSideTactics(vector <TokenEntity *> * side);
	int calculateSideObservation(vector <TokenEntity *> * side);
	int calculateAttackerInitiative(int round);
	int calculateDefenderInitiative(int round);
	void shuffleSide(vector <TokenEntity *> * side, int ambush);
  static bool sortLoot(ItemElement element1, ItemElement element2);
  void extendMinInitiative();
// Side enchantments
	void addAttackerSideEnchantment(EnchantmentElement * data);
	void removeAttackerSideEnchantment(EnchantmentElement * data);
	void removeAllAttackerSideEnchantment();
	void addDefenderSideEnchantment(EnchantmentElement * data);
	void removeDefenderSideEnchantment(EnchantmentElement * data);
	void removeAllDefenderSideEnchantment();


	protected:
  private:
			vector <TokenEntity *> * attackers_;
			vector <TokenEntity *> * defenders_;
			vector <TokenEntity *>  deadCombatants_;
		  EnchantmentAttribute attackerSideEnchantments_;// Battle only enchantments
		  EnchantmentAttribute defenderSideEnchantments_;

		 LocationEntity * location_;
		 BattleField * battleField_;
     int currentInitiativeSegment_;
     BATTLE_RESULT result_;
		 int woundlessRoundCounter_;
		 int maxFile_;
		 int minFile_;
		 int maxRank_;
		 int minRank_;
			// Side stats
		 int attackerStrategy_;// calculated at the beginning of batle
		 int defenderStrategy_;
		 int attackerAmbush_;// calculated at the beginning of batle
		 int defenderAmbush_;
		 int attackerObservation_;// calculated at the beginning of batle
		 int defenderObservation_;
		 int attackerTactics_;// calculated at the beginning of each round
		 int defenderTactics_;
		 int attackerLossCount_; // Fanatics are not counted as losses
		 int defenderLossCount_;
		 int attackersAliveCount_;
		 int defendersAliveCount_;
		 int attackersCount_; // Initial numbers
		 int defendersCount_;
		 int attackerMoralCheckCount_;
		 int defenderMoralCheckCount_;

 		 int initMin_;
 		 int initMax_;


};
typedef  vector <TokenEntity *> ::iterator BattleIterator;
extern CombatEngine realCombatEngine;
#endif
