/***************************************************************************
                          FactionEntity.h  -  description
                             -------------------
    begin                : Tue Nov 19 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef FACTION_ENTITY_H
#define FACTION_ENTITY_H

#include "Entity.h"
#include "Element.h"
#include "StanceElement.h"
#include "SkillLevelElement.h"
#include "ItemElement.h"

class LocationEntity;
class UnitEntity;
class StanceVariety;
class SkillRule;
class ItemRule;
class Rule;
class BasicRulesCollection;
class BasicEntitiesCollection;
class TokenEntity;
class ConstructionEntity;
class CombatReport;
class TurnReport;
class EffectEntity;

class FactionEntity : public Entity  {
    public:
      FactionEntity (const string & keyword, Entity * parent ) : Entity(keyword, parent){}
      FactionEntity ( const FactionEntity * prototype );
      STATUS  initialize      ( Parser *parser );
       void      save (ostream &out);
      GameData * createInstanceOfSelf();
 virtual STATUS        prepareData();
  void    preprocessData();
  void    postProcessData();
  void loadOrders();
  inline void setEMail(const string& email) { email_ = email;}
  inline void setPassword(const string& password)  {password_ = password;}
  inline string getEMail() const{ return email_ ;}
  inline string getPassword()const  {return password_;}
  void dailyReport();
  void dailyEventProcessing();
  void dailyPreProcess();
  void reportFunds(ReportPrinter &out);  /** Adds report to the list if it is different from those, that are already there */
  void updateReports(ReportElement * report);
  void updateEvents(Event * event);
  void finalizeTurnEvents();
  void saveReport();
  void saveCReport();
  void dailyUpdate();
  virtual Entity * getReportDestination();
  void addCombatReport(CombatReport * combatReport);
  inline TurnReport * getTurnReport() const {return turnReport_;}
//================== Units and Constructions
  void addUnit(UnitEntity * unit);
  void removeUnit(UnitEntity * unit);
  void addConstruction(ConstructionEntity * construction);
  void removeConstruction(ConstructionEntity * construction);
inline vector<UnitEntity *>& getLoyalUnits(){return loyalUnits_;}
inline vector<ConstructionEntity *>& getLoyalConstructions(){return loyalConstructions_;}
 //================== Orders
   STATUS loadFactionOrders(Parser * parser, TokenEntity ** entity);
  TokenEntity * currentEntityOrders(BasicEntitiesCollection & collection,
                                  Parser * parser);

 //==================
  StanceVariety * getStance(TokenEntity * token) ;
  StanceVariety * getStance(FactionEntity * entity) ;
  void setStance(Entity * entity, StanceVariety * stance);
  void setDefaultStance(StanceVariety * stance) {defaultStance_ = stance;}
  StanceVariety * getDefaultStance() {return defaultStance_;}
  int  withdraw(ItemRule * item, int number);
  void addToFunds(ItemRule * item, int number);
  bool mayWithdraw(ItemRule * item, int number);
  bool mayObserveTokenEntity(TokenEntity * tokenEntity, LocationEntity * location);
  void calculateControlPoints();
  bool isVisitedLocation(LocationEntity * location);
  inline void setMaxControlPoints(int maxControlPoints) {maxControlPoints_ = maxControlPoints;}
  inline void addMaxControlPoints(int controlPoints) {maxControlPoints_ += controlPoints;}
  inline void removeMaxControlPoints(int controlPoints) {maxControlPoints_ -= controlPoints;}
  inline int  getMaxControlPoints() const {return maxControlPoints_;}
  inline int  getControlPoints() const {return controlPoints_;}
  inline bool isExeededControlPoints() const {return maxControlPoints_< controlPoints_;}
  inline void setTerseBattleReport(bool value){terseBattleReport_ = value;}
  inline bool getTerseBattleReport(){return terseBattleReport_;}
  void markCollectionToReshow(BasicRulesCollection  * collection);
  void markKnowledgeToReshow(Rule  * knowledge);
  inline void markAllSkillsToReshow(){allSkillsToReshow_ = true;}
  void markSkillToReshow(SkillRule * skill, int level);
  void resign(FactionEntity * faction = 0);
  bool checkAnyUnitsLeft();
  bool isNPCFaction();
  bool stanceAtLeast(FactionEntity * faction, StanceVariety * stance);
  bool stanceAtLeast(TokenEntity * token, StanceVariety * stance);
  void setTemporaryFlag(unsigned long value, unsigned long mask) { temporaryFlags_= ((temporaryFlags_ & ~mask) | (value & mask));}
  vector <ReportElement *> getEvents();
  inline unsigned long     getTemporaryFlag(unsigned long mask) const {return (temporaryFlags_ & mask);}
        inline      vector<ItemElement> &     getFunds(){return funds_;}
        inline      vector<StanceElement>&   getStances(){return stances_;}
        bool isLoyalUnit(UnitEntity * unit);
        bool isLoyalConstruction(ConstructionEntity * unit);
        inline bool isResigned(){return isResigned_;}
//===============================  Knowledge ============
  bool addKnowledge(Rule * info);
//  void addKnowledge(SkillLevelElement & info);
  bool addSkillKnowledge(SkillRule * knowledge, int level);
  Rule * hasKnowledge(Rule * info);
//SkillLevelElement *   hasSkillKnowledge(SkillLevelElement & info);
  SkillLevelElement * hasSkillKnowledge(SkillRule * knowledge, int level);
  void reshow(Rule * info, ReportPrinter &out);
  void reshow(BasicRulesCollection  * collection, ReportPrinter &out );
//  void reshow(SkillLevelElement & info, ostream &out);
  void reshowSkill(SkillRule * knowledge, int level, ReportPrinter &out);
  void reshowSkills(ReportPrinter &out );
  void saveKnowledge(ostream &out);
  void reportNewKnowledge(ReportPrinter &out);
  void loadKnowledge(Parser *parser);
  vector <SkillLevelElement *> getAllSkillKnowledge();
  vector <Rule *> getAllKnowledge();
//===============================  Images ============

      void  updateImage(FactionEntity * imageToAdd);
      FactionEntity *     createFactionImage(FactionEntity * referent);
  bool addVisitedLocation(LocationEntity * location);
  void addDiscoveredLocation(LocationEntity * location);
  void clearVisitedLocations();
//  void addUnitImage(UnitEntity * imageToAdd);
//  void createAndAddUnitImage(UnitEntity * unitToAdd, TokenEntity * referent);
//  void addConstructionImage(ConstructionEntity * imageToAdd);
//  void addEffectImage(EffectEntity * imageToAdd);
//  void addLocationImage(LocationEntity * image);
  void dailyImageProcessing();
 // void addFactionImage(TokenEntity * imageToAdd);
  void addFactionImage(FactionEntity * factionToAdd);
  void extractFactionImage(TokenEntity * tokenToAdd);
  AbstractData * findImage(AbstractData * data);
//========================= Local   =======
  inline int getX0() const {return x0_;}
  inline void setX0(int value)  { x0_=value;}
  inline int getY0() const {return y0_;}
  inline void setY0(int value)  { y0_=value;}

    protected:

  vector<Rule *> knowledge_;
  int newKnowledge; // number of new knowledge learned this turn
  vector<SkillLevelElement *> skillKnowledge_;
  unsigned newSkillKnowledge; // number of new skill learned this turn
  vector < BasicRulesCollection  *> collectionsToReshow_;
  vector < Rule  *> knowledgeToReshow_;
  bool allSkillsToReshow_;
  vector<SkillLevelElement> skillsToReshow_;
  vector<CombatReport *> combatReports_;
  vector <ReportElement *> extractedReports_;
  string email_;
  string password_;
  StanceVariety * defaultStance_;
  vector<StanceElement> stances_;
  vector<UnitEntity *> loyalUnits_;
  vector<ConstructionEntity *> loyalConstructions_;
  vector<ItemElement> funds_;
  int maxControlPoints_;
  int controlPoints_;
  bool terseBattleReport_;
  bool isResigned_;
  bool isDisbanded_;
  unsigned long temporaryFlags_;
  TurnReport * turnReport_;
  int x0_; // origin of coordinates for this faction
  int y0_;// origin of coordinates for this faction
  // ====== Images:
  vector<LocationEntity *> visitedTodayLocations_;
  vector<LocationEntity *> visitedLocations_;
  vector<LocationEntity *> discoveredLocations_;
  vector<UnitEntity *> observedUnits_;
  vector<ConstructionEntity *> observedConstructions_;
  vector<EffectEntity *> observedEffects_;
  vector<FactionEntity *> knownFactions_;
    private:
 };
typedef	vector<Rule *>::iterator KnowledgeIterator;
typedef	vector<FactionEntity *>::iterator FactionIterator; 
extern FactionEntity    sampleFaction;
//#include "EntitiesCollection.h"
//extern EntitiesCollection <FactionEntity>   factions;
#endif
