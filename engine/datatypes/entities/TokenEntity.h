/***************************************************************************
                          TokenEntity.h
                             -------------------
    begin                : Fri Jul 25 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef TOKEN_ENTITY_H
#define TOKEN_ENTITY_H

#include "Entity.h"
#include "SkillElement.h"
#include "SkillLevelElement.h"
#include "InventoryAttribute.h"
#include "SkillsAttribute.h"
#include "BasicCombatEngine.h"
#include "CombatTactics.h"
#include "EntityStatistics.h"
#include "ItemElement.h"
class UnitEntity;
class FactionEntity;
class LocationEntity;
class ItemRule;
class ReportPattern;
class BasicExit;
class TravelElement;
class SkillUseElement;
class ConstructionEntity;
class ConstructionRule;
class BasicOrderSynchronizationRequest;
class CombatStanceVariety;
class BattleInstance;
class CombatOrderLine;
class BattleField;
class MovementBonusElement;

//class SkillElement;
/**subclass of Entities that have physical appearence and may be located in the world
  *@author Alex Dribin
  */
typedef  Element3<TokenEntity,ItemRule *, int > SwapRequestElement;

class TokenEntity : public Entity  {
public:
      TokenEntity (const string & keyword, GameData * parent );
      TokenEntity ( const TokenEntity * prototype );
	~TokenEntity(){}
          virtual void dailyUpdate();
          virtual bool defaultAction();
  STATUS  initialize      ( Parser *parser );
  void    save (ostream &out);
  void      preprocessData();
  void postProcessData();
  void postPostProcessData();
	virtual inline BattleInstance * getBattleInstantiation() const
	 																									{return battleInstance_;}
// Reporting ==============================================
         /** Return pointer to  Entity which keeps reports from this  */
         Entity *         getReportDestination();
   virtual void  privateReport(ReportPrinter &out);
   virtual void  printOrderTemplate(ReportPrinter &out);
   virtual void    reportAppearence(FactionEntity * faction, ReportPrinter &out){}
   virtual void    reportInventory(FactionEntity * faction, ReportPrinter &out){}
   virtual void    reportSkills(FactionEntity * faction, ReportPrinter &out);
   virtual void    reportFlags(ReportPrinter &out);
// Data access methods ==============================================
   virtual LocationEntity * getGlobalLocation() const;
   inline virtual LocationEntity * getLocation() const{return location_;}
   inline virtual void             setLocation(LocationEntity * location) {location_ = location;}
   inline virtual FactionEntity *  getFaction() const{return faction_;}
   inline virtual void             setFaction( FactionEntity * faction) {faction_ = faction;}
          virtual int              getWeight() {return 0;}
          virtual Rule *           getType() {return 0;}
          virtual bool             isOfType(Rule * type) {return false;}
          virtual bool             isDisbanded();
                  bool             isUnaccessible() const;
                  bool             isBusy()  const;
   inline         AbstractData *   getTarget() const {return target_;}
                  void             setTarget(AbstractData * target);
   inline         AbstractData *   getDefaultTarget() const{return defaultTarget_;}
   inline         void             setDefaultTarget(AbstractData * target) {defaultTarget_ = target;}
   inline virtual int              getObservation() const {return 0;}
   inline virtual int              getStealth() const {return 0;}
   inline virtual bool             isGuarding() const { return guarding_;}
          virtual bool             isNpcGuard() const; 
   inline         void             setGuarding(bool value){ guarding_ = value;}
// Guard and Patrol merged in current implementation
   inline virtual bool             isPatrolling() const { return guarding_;}
   inline         void             setPatrolling(bool value){ guarding_ = value;}
   inline virtual int              getControlPoints() const{return 0;}
   inline virtual int              getInitiative() const {return 0;}

	 inline virtual int              getMelee() const {return 0;}
	 inline virtual int              getMissile() const {return 0;}
	 inline virtual int              getDefence() const {return 0;}
	 inline virtual int              getHits() const {return 0;}
	 inline virtual int              getFiguresNumber() const {return 1;}
	 inline virtual void             sufferDamage(int value)  {}

   inline virtual bool             isExposed() const {return false;}
   inline virtual bool             mayGuard(bool enableReport = true)  const {return false;}
   inline virtual bool             isAlive()  const {return alive_;}
	 inline EntityStatistics * getStats()  {return &stats;}
	 virtual EntityStatistics  getBasicStats(){return stats;}
         inline virtual int getLoyality(){return loyality_;}
         inline virtual void setLoyality(int loyality){loyality_ = loyality;}
// Inventory methods ==============================================
     InventoryElement * findInInventory(ItemRule * item);
     vector < InventoryElement > getSlotContent(EquipmentSlotVariety * slot);
     void deleteFromInventory(InventoryElement * element);
     virtual int  getCapacity(int modeIndex){return 0;}
		 virtual int getCapacity(MovementVariety * mode){return 0;}
     void    addToInventory(ItemRule * item, RationalNumber& num);
     void    addToInventory(ItemRule * item, int num);
     int     takeFromInventory(ItemRule * item, int num);
//         int    takeFromInventory(ItemRule * item, int num);
     int     hasItem(ItemRule * item) ;
     bool isEquiped(InventoryElement * item);
     RationalNumber     getItemAmount(ItemRule * item) ;
     virtual int     equipItem(ItemRule * item, int num);
     int     hasEquiped(ItemRule * item) ;
  vector < InventoryElement > & getAllInventory();
  void giveAllInventory(TokenEntity * unit);
// Stacking/Containment ========================================================
          virtual void            accept(UnitEntity * unit);
          virtual bool isAccepting(UnitEntity * unit);
          virtual void            clearAccept(UnitEntity * unit);
// Skills ========================================================

          vector < SkillElement>& getAllSkills();
          virtual int  addSkill(SkillElement  skill);
          virtual int  addSkill(SkillRule  * skill, int expPoints);
          virtual void gainNewLevel(SkillRule * skill, int newLevel);
          virtual int  getSkillLevel(SkillRule  * const skill);
          virtual int  getSkillPoints(SkillRule  * const skill);
          virtual SkillElement *  getSkillElement(SkillRule  * const skill);
          virtual void cancelTeachingOffer();
          virtual bool hasSkill(SkillRule  * skill, int experience) ;
  inline  virtual bool hasSkillLevel(SkillRule  * skill, int level)
        {return hasSkill(skill, skill->getLevelExperience(level));}
  inline  virtual bool hasSkill(SkillLevelElement * skill)
        {return hasSkill(skill->getSkill(),
			skill->getSkill()->getLevelExperience(skill->getLevel()));}

  inline virtual bool hasSkill(SkillElement  * skill)
        {return hasSkill(skill->getSkill(), skill->getExpPoints());}
  inline virtual bool hasSkill(SkillElement  skill)
        {return hasSkill(skill.getSkill(), skill.getExpPoints());}
         virtual  int getLearningCapacity();
         virtual bool teacherRequired(SkillRule * skill) ;
         virtual bool mayStudySkill(SkillRule * skill);
         virtual bool mayStudyWithTeacher(SkillRule * skill);
         virtual LEARNING_RESULT mayLearn(SkillRule * skill);
  inline virtual bool mayGainExperience() const {return true;}
     TeachingOffer * findTeachingOffer(SkillRule  * skill, int level);
// Skill Use ========================================================
   /** returns true when work comleted. Otherwise - false */
   virtual int addSkillUse(SkillUseElement * skillUse);
   virtual int addCumullativeSkillUse(SkillUseElement * skillUse,
	 																int accumulationLimit);
   virtual bool isCurrentlyUsingSkill(SkillRule * skill);
// Movement ========================================================
   virtual void  setEntityMoving(TravelElement * moving);
   inline         bool             isMoving() const {return moving_;}
   virtual bool moveAdvance();
   virtual void moveToLocation();
   virtual void moveToLocation(LocationEntity * newLocation);
   virtual void moveGroupToLocation();
   virtual void movingGroupArrived();
   virtual void movingEntityArrived(){}
   void setPassenger(bool value) {passenger_ = value;}
   bool retreat();
   virtual void movingGroupReport(ReportRecord report ){}
   static void marchAttackPostprocessing(TokenEntity * attacker,
	 				 TokenEntity * defender,const BATTLE_RESULT  result);
   void tryEnterLocation();
   int getMovementBonus(MovementVariety * mode){return 0;}
	 void recalculateTravelTime();
		MovementVariety * getReservedMode();
// Combat ========================================================
	void checkCombatSetting(string combatOrderText);
	 virtual  BattleInstance * createBattleInstantiation(BattleField * battleField);
          bool combatStanceAtLeast(CombatStanceVariety * combatStance) const;
   void setCombatMove(CombatMoveVariety * value);
   inline void setCombatRank(CombatRankVariety * value)
	 																	{combatTactics_.setCombatRank(value);}
   inline void setCombatFile(CombatFileVariety * value)
	 																	 {combatTactics_.setCombatFile(value);}
   inline CombatMoveVariety * getCombatMove() const
	 																	{return combatTactics_.getCombatMove();}
   inline CombatRankVariety * getCombatRank() const
	 																	{return combatTactics_.getCombatRank();}
   inline CombatFileVariety * getCombatFile() const
	 																	 {return combatTactics_.getCombatFile();}
  inline void setCombatStance(CombatStanceVariety * value)
	 																	 {combatTactics_.setCombatStance(value);}
   inline CombatStanceVariety * getCombatStance()
	 																	{return combatTactics_.getCombatStance();}
   virtual int getAttackRating() const;
   virtual int getDefenceRating() const;
   void addOrder(string newOrder);
	 void clearOrders();
  virtual inline vector < CombatOrderLine*> & getCombatOrderList()
																			{return combatOrders_;}
	 virtual inline vector <CombatOrderLine*> & getDefaultCombatOrders()
	 								{return defaultCombatOrders_;}
	 virtual inline CombatOrderLine* getDefaultCombatMovement()
	 								{return defaultCombatMovement_;}
	 virtual void setDefaultCombatMovement();	
	 virtual inline int getLife() const {return 0;}
	 void addCombatSetting(string combatOrderText);
	virtual inline int getDamage() const { return 0;}
	virtual inline int getRangedDamage() const { return 0;}
	virtual inline DAMAGE_TYPE getDamageType() const { return PHYSICAL;}
	virtual DAMAGE_TYPE modifyDamageType(DAMAGE_TYPE value){return value;}
	inline bool isFanatic() const {return fanatic_;}
	inline void setFanatic(bool value){fanatic_ = value;}
	virtual inline bool mayParticipateInCombat() const {return true;}
// Flags ========================================================
    void setAdvertising(bool value) {advertising_ = value;}
    void setAnnouncing(bool value) {announcing_ = value;}
    void setSharing(bool value) {sharing_ = value;}
    void setWithdrawingSupport(bool value) {withdrawingSupport_ = value;}
    bool getAdvertising() {return advertising_;}
    bool getAnnouncing() {return announcing_;}
    bool getSharing() {return sharing_;}
    bool getWithdrawingSupport() {return withdrawingSupport_;}
    inline  void markToOath(FactionEntity * faction) {toOath_ = faction;}
    ORDER_STATUS oath(FactionEntity * faction);
    virtual bool leaveStaying();
    virtual bool isAccepted(UnitEntity * unit){return false;}
    BasicOrderSynchronizationRequest * hasOrderSyncRequest
												(BasicOrderSynchronizationRequest * request);
    void removeOrderSyncRequest(BasicOrderSynchronizationRequest * request);
    void addOrderSyncRequest(BasicOrderSynchronizationRequest * request);
    bool doneOrderSyncRequest(BasicOrderSynchronizationRequest * request);
    void markDoneOrderSyncRequest(BasicOrderSynchronizationRequest * request);
// Bonuces ========================================================
        virtual int getProductionBonus(SkillRule * skill);
// Other ========================================================
          virtual void disband(){}
          virtual void recalculateStats();
          virtual bool mayMove();
          virtual bool mayHoldTitles(){return false;}
          virtual int  calculateTotalWeight (int & weight);
          virtual void calculateTotalCapacity(int & capacity, int modeIndex);
          virtual void calculateTotalCapacityMode(int & capacity, MovementVariety * mode);
          virtual int calculateMovementBonus(MovementVariety * mode);
                  int calculateTravelTime(int time, MovementVariety * mode);
									int calculateOverloading(int time, int weight, int capacity);
                  bool mayInterractTokenEntity(TokenEntity * tokenEntity);
                  bool mayObserveTokenEntity(TokenEntity * tokenEntity);
          virtual bool mayInterractFaction(FactionEntity * faction);
   inline TravelElement  * getTravelStatus() const {return moving_;}
   inline void  setTravelStatus(TravelElement  * status) {moving_ = status;}
	 virtual string printComposition(){return string("");}
	 virtual Rule * getComposition(){return 0;}// race or construction
   bool takeLoot(vector <ItemElement> & items);
   bool takeTransport(vector <ItemElement> & items, MovementVariety *mode);
	 void explicitInitialization();
         int countElementalMagicSkill();

   protected:
    LocationEntity * location_ ;
    FactionEntity  * faction_;
    TravelElement  * moving_;
    bool isMoving_;
    int loyality_;
		// Attributes
  EntityStatistics stats;
  SkillsAttribute      skills_;
  InventoryAttribute inventory_;
  vector <MovementBonusElement *> movementBonuses_;
		// Attributes end
  vector < UnitEntity *>      accepting_;
  vector < SkillUseElement *> skillUse_;
  vector <CombatOrderLine *> combatOrders_;
  vector <CombatOrderLine *> defaultCombatOrders_;
  CombatOrderLine * defaultCombatMovement_;
  vector <string> combatSettings_;
         AbstractData *  target_;
         AbstractData *  defaultTarget_;
         CombatTactics combatTactics_;
         bool    alive_;
         bool    guarding_;
//         bool    patrolling_;
         bool    passenger_;
         bool 	 advertising_;
         bool 	 announcing_;
         bool 	 sharing_;
         bool 	 withdrawingSupport_;
	bool 	 fanatic_;
    FactionEntity *    toOath_;
    vector <BasicOrderSynchronizationRequest *> orderSyncRequests_;
		BattleInstance * battleInstance_;
};
extern TokenEntity         sampleTokenEntity;
typedef vector <BasicOrderSynchronizationRequest *>::iterator  SyncRequestIterator;
typedef vector <TokenEntity *>::iterator  TokenIterator;
#endif
