/***************************************************************************
                      BattleInstance.h
               Instance taking part in combat
                          ------------------
    begin                : Mon Oct 26 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef BATTLE_INSTANCE_H
#define BATTLE_INSTANCE_H
#include <vector>
#include <string>
#include "OrderPrototype.h"
#include "EnchantmentAttribute.h"
#include "EntityStatistics.h"
#include "SkillElement.h"
#include "TitlesAttribute.h"
#include "InventoryAttribute.h"
#include "SkillsAttribute.h"
#include "RaceRule.h"

using namespace std;
class CombatOrderLine;
class TokenEntity;
class BattleField;
class CombatReport;
class CombatEngine;


class BattleInstance
{
public:
  BattleInstance(TokenEntity * origin, CombatReport * report);
  virtual ~BattleInstance();
	string print();


//--------- Operation ------
	void initialize(BattleField * battleField);
	void parry();
	void destroy();
	void rout();
	void recalculateStats();
	void dropLoot(bool loosing);
	void dropTitles();
	void updateOrigin();
	void lossFigures(int figuresDied);
	 int sufferDamage(int hits, int damage, DAMAGE_TYPE type = PHYSICAL);
   int reCalculateInitiative(int sideBonus, int &initMin, int &initMax );
	void postProcess();
	void preProcess();
	void addSideEnchantment(EnchantmentElement * data, bool targetSide);
	void removeSideEnchantment(EnchantmentElement * data, bool targetSide);
	void removeAllSideEnchantment(bool targetSide);
//--------- State and stats------
	virtual bool isAlive();
  virtual bool isFlying();
	inline bool isAttacker() const {return isAttacker_;}
	inline bool isDefender() const {return !isAttacker_;}
  inline void setAttacker() {isAttacker_ = true;}
  inline void setDefender() {isAttacker_ = false;}
  inline void setRank(int value) { positionRank_= value;}
  inline void setFile(int value) { positionFile_= value;}
	inline  int getFile()const {return positionFile_;}
	inline  int getRank()const {return positionRank_;}
		     void setWounds(int value);
		      int getWounds()const;
		      int getMelee()const ;
		      int getMissile()const ;
		      int getDefence()const ;
			    int getFiguresNumber()const ;
			   void setFiguresNumber(int value);
  		    int getDamage()  const;
			    int getHitNumbers()const ;
			    int getStealth()const ;
			    int getObservation()const ;
			    int getMorale() const;
	inline DAMAGE_TYPE getDamageType() const { return damageType_;}
	DAMAGE_TYPE modifyDamageType(DAMAGE_TYPE value);
	inline bool isFanatic() const {return fanatic_;}
	inline void setFanatic(bool value){fanatic_ = value;}
	inline int getMovementInitiative()const {return movementInitiative_;}

//--------- Flags ------
   bool mayActOnRound(OrderPrototype * orderType,int round );
   void setRoundFlags(ORDER_TYPE orderType,int round );
	 void clearRoundFlags();
  inline void setMovedOnRound(int value) {isMovedOnRound_ = value;}
  inline void setActedOnRound(int value) {isActedOnRound_ = value;}
  inline void setAttackedOnRound() {isAttackedOnRound_++;}
  inline int getAttackedOnRound() const {return isAttackedOnRound_;}
  inline bool isWaiting() const {return isWaiting_;}
  inline void setWaiting(bool value){isWaiting_ = value;}
	inline void setAffectingAction(CombatActionStrategy *action){ affectingAction_ = action;}
//--------- Orders ------
  virtual inline vector < CombatOrderLine*> & getCombatOrderList()
	                                      {return combatOrders_;}
   void recheckOrders();
   void respawnCombatActionOrders();
   void respawnCombatMovementOrders();
	inline CombatOrderLine * getCurrentOrder() const {return currentOrder_;}
	inline void setCurrentOrder(CombatOrderLine * currentOrder)
	   {currentOrder_ = currentOrder;}
	void planRoundOrders();
	inline CombatOrderLine * getCurrentlyRepeatingOrder() const
			{ return currentlyRepeatingOrder_;}
	bool isProcessingRepeatingOrder();
  void processMultiHitting(int hitNumbers);
	void fleeAway();
//	inline void setRepetitionCounter(int value){repetitionCounter_ = value;}
//	inline void setCurrentlyRepeatingOrder(CombatOrderLine * order)
//		{currentlyRepeatingOrder_ = order;}
//--------- Data ------
	inline  CombatReport * getCombatReport()const {return report_;}
	inline TokenEntity * getOrigin() const {return origin_;}
	inline BattleField * getBattleField() const {return battleField_;}
  void addEnchantment(EnchantmentElement *enchantment) {enchantments_.add(enchantment); }
  void removeEnchantment(EnchantmentElement *enchantment) {enchantments_.remove(enchantment); }
	TitlesAttribute   * getAllTitles() {return titles_;}
 void setSideEnchantment(EnchantmentAttribute * sideEnchantment);
//--------- Experience------
	void addActionExperience(SkillRule * skill);
	inline void addCombatExperience(int value) {combatExperience_ +=value;}
	inline void addParryExperience(int value) {parryExperience_ +=value;}
	inline void addMeleeExperience(int value) {meleeExperience_ +=value;}
	inline void addMissileExperience(int value) {missileExperience_ +=value;}
	void addFinalExperience(int survived, int totalEnemies, int survivedEnemies);

	inline int getFleeCounter() const {return fleeCounter_;}
  inline void setFleeCounter(int counter)  { fleeCounter_ = counter;}
  inline void advanceFleeCounter()  { fleeCounter_++;}
	inline bool isFled() const {return fled_;}

	protected:
		TokenEntity * origin_;
		// Attributes
		EnchantmentAttribute enchantments_;// Battle only enchantments
		EnchantmentAttribute * sideEnchantments_;
		EntityStatistics stats_;
    InventoryAttribute inventory_;
    SkillsAttribute      skills_;
		// Unit-specific
    TitlesAttribute   *   titles_; // pointer to unit's TitlesAttribute
		vector< EquipSlot *> * equipSlots_;
		RaceRule * race_;
		//
		CombatOrderLine * currentlyRepeatingOrder_;
		int repetitionCounter_;
		int fleeCounter_;
		//
		vector <int> figures_;
		int figuresNumber_;
		int wounds_;
		DAMAGE_TYPE damageType_;
	  int positionRank_;
		int positionFile_;
		bool isAttacker_;
    vector <CombatOrderLine *> combatOrders_;
		int isMovedOnRound_;
		int isActedOnRound_;
		bool isWaiting_; // waiting to get initiative
		CombatOrderLine * currentOrder_;
		BattleField * battleField_;
		CombatActionStrategy * affectingAction_;
		bool parryFlag_;
		int isAttackedOnRound_;
		int movementInitiative_;
		CombatReport * report_;
		int combatExperience_;
		int parryExperience_;
		int meleeExperience_;
		int missileExperience_;
		vector <SkillElement> actionExperience_;
		bool fanatic_;
		bool routed_;
		bool fled_;
  private:

};

typedef  vector <BattleInstance *> ::iterator BattleInstanceIterator;

#endif

