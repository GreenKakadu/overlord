/***************************************************************************
                          TokenEntity.h
                             -------------------
    begin                : Fri Jul 25 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
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
#include "InventoryElement.h"
//#include "ReportPrinter
class UnitEntity;
class FactionEntity;
class LocationEntity;
class ItemRule;
class Reporter;
class BasicExit;
class TravelElement;
class SkillUseElement;
class ConstructionEntity;
class ConstructionRule;
class BasicOrderSynchronizationRequest;

//class SkillElement;
/**subclass of Entities that have physical appearence and may be located in the world
  *@author Alex Dribin
  */
typedef  Element3<TokenEntity,ItemRule *, int > SwapRequestElement;

class TokenEntity : public Entity  {
public: 
	    TokenEntity();
      TokenEntity (const string & keyword, GameData * parent ) : Entity(keyword, parent){}
      TokenEntity ( const TokenEntity * prototype );
	~TokenEntity(){}
          virtual void dailyUpdate();
          virtual bool defaultAction();
  STATUS  initialize      ( Parser *parser );
  void    save (ostream &out);
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
   inline virtual LocationEntity * getGlobalLocation() const;
   inline virtual LocationEntity * getLocation() const{return location_;}
   inline virtual void             setLocation(LocationEntity * location) {location_ = location;}
   inline virtual FactionEntity *  getFaction() const{return faction_;}
   inline virtual void             setFaction( FactionEntity * faction) {faction_ = faction;}
   inline         bool             isTraced() const {return traced_;}  /** used for debugging */
          virtual int              getWeight() {return 0;}
          virtual Rule *           getType() {return 0;}
          virtual bool             isOfType(Rule * type) {return false;}
          virtual bool             isDisbanded();
                  bool             isUnaccessible() const;
   inline         bool             isBusy()  const;
   inline         AbstractData *   getTarget() const {return target_;}
                  void             setTarget(AbstractData * target);
   inline virtual int              getObservation() const {return 0;}
   inline virtual int              getStealth() const {return 0;}
   inline virtual bool             isGuarding() const { return guarding_;}
   inline virtual int              getControlPoints() {return 0;}
   inline virtual bool             isExposed() const {return false;}
   
// Inventory methods ==============================================
          InventoryElement * findInInventory(ItemRule * item);
          void deleteFromInventory(InventoryElement * element);
          virtual int  getCapacity(int modeIndex){return 0;}
         void    addToInventory(ItemRule * item, RationalNumber& num);
         void    addToInventory(ItemRule * item, int num);
         int     takeFromInventory(ItemRule * item, int num);
//         int    takeFromInventory(ItemRule * item, int num);
         int     hasItem(ItemRule * item) ;
          bool isEquiped(InventoryElement * item);
         RationalNumber     getItemAmount(ItemRule * item) ;
         virtual int     equipItem(ItemRule * item, int num);
         int     hasEquiped(ItemRule * item) ;
  vector < InventoryElement *> & getAllInventory();
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
        {return hasSkill(skill->getSkill(), skill->getSkill()->getLevelExperience(skill->getLevel()));}

  inline virtual bool hasSkill(SkillElement  * skill)
        {return hasSkill(skill->getSkill(), skill->getExpPoints());}
  inline virtual bool hasSkill(SkillElement  skill)
        {return hasSkill(skill.getSkill(), skill.getExpPoints());}
         virtual  int getLearningCapacity();
         virtual bool teacherRequired(SkillRule * skill) ;
         virtual bool mayStudySkill(SkillRule * skill);
         virtual LEARNING_RESULT mayLearn(SkillRule * skill);
  inline virtual bool mayGainExperience() const {return true;}
// Skill Use ========================================================
   /** returns true when work comleted. Otherwise - false */
   virtual int addSkillUse(SkillUseElement * skillUse);
   virtual int addCumullativeSkillUse(SkillUseElement * skillUse, int accumulationLimit);
   virtual bool isCurrentlyUsingSkill(SkillRule * skill);
// Movement ========================================================
   virtual void  setEntityMoving(TravelElement * moving);
   inline         bool             isMoving() const {return moving_;}
   virtual bool moveAdvance();
   virtual void moveToLocation();
   virtual void moveGroupToLocation();
   virtual void movingGroupArrived();
   virtual void movingEntityArrived(){}
   void setPassenger(bool value) {passenger_ = value;}
   bool retreat();
   virtual void movingGroupReport(ReportRecord report ){}
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
                  BasicOrderSynchronizationRequest * hasOrderSyncRequest(BasicOrderSynchronizationRequest * request);
                  void removeOrderSyncRequest(BasicOrderSynchronizationRequest * request);
                  void addOrderSyncRequest(BasicOrderSynchronizationRequest * request);
                  bool doneOrderSyncRequest(BasicOrderSynchronizationRequest * request);
                  void markDoneOrderSyncRequest(BasicOrderSynchronizationRequest * request);
// Other ========================================================
          virtual void recalculateStats();
          virtual bool mayMove();
          virtual bool mayHoldTitles(){return false;}
          virtual int  calculateTotalWeight (int & weight);
          virtual void calculateTotalCapacity(int & capacity, int modeIndex);
                  int calculateTravelTime(int time, int weight, int capacity);
                  bool mayInterractTokenEntity(TokenEntity * tokenEntity);
          virtual bool mayInterractFaction(FactionEntity * faction);


   protected:
    LocationEntity * location_ ;
    FactionEntity  * faction_;
    TravelElement  * moving_;
	vector < UnitEntity *>      accepting_;
  vector < SkillElement>      skills_;
  vector < SkillUseElement *> skillUse_;
  vector < InventoryElement *> inventory_;
         AbstractData *  target_;
				 bool    traced_;
         bool    guarding_;
         bool    passenger_;
         bool 	 advertising_;
         bool 	 announcing_;
         bool 	 sharing_;
         bool 	 withdrawingSupport_;
    FactionEntity *    toOath_;
    vector <BasicOrderSynchronizationRequest *> orderSyncRequests_;
};
extern TokenEntity         sampleTokenEntity;
typedef vector <BasicOrderSynchronizationRequest *>::iterator  SyncRequestIterator;
#endif
