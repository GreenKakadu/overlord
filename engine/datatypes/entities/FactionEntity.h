/***************************************************************************
                          FactionEntity.h  -  description
                             -------------------
    begin                : Tue Nov 19 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef FACTION_ENTITY_H
#define FACTION_ENTITY_H

#include "Entity.h"
#include "Element.h"
#include "StanceElement.h"
#include "SkillLevelElement.h"

class LocationEntity;
class UnitEntity;
class StanceVariety;
class SkillRule;
class ItemElement;
class ItemRule;
class Rule;
class BasicRulesCollection;
class BasicEntitiesCollection;
class TokenEntity;
class ConstructionEntity;

class FactionEntity : public Entity  {
    public:
      FactionEntity (const string & keyword, Entity * parent ) : Entity(keyword, parent){}
      FactionEntity ( const FactionEntity * prototype );
      STATUS  initialize      ( Parser *parser );
       void      save (ostream &out);
      GameData * createInstanceOfSelf();
  void    preprocessData();
  void    postProcessData();
  void loadOrders();
  inline void setEMail(const string& email) { email_ = email;}
  inline void setPassword(const string& password)  {password_ = password;}
  void addVisitedLocation(LocationEntity * location);
  void clearVisitedLocations();
  void dailyReport();
  /** Adds report to the list if it is different from those, that are already there */
  void updateReports(ReportElement * report);
  void saveReport();
  void dailyUpdate();
  void addUnit(UnitEntity * unit);
  void removeUnit(UnitEntity * unit);
  void addConstruction(ConstructionEntity * construction);
  void removeConstruction(ConstructionEntity * construction);
  STATUS loadFactionOrders(Parser * parser, TokenEntity ** entity);
  TokenEntity * currentEntityOrders(BasicEntitiesCollection & collection,
                                  Parser * parser);
  virtual Entity * getReportDestination();
  StanceVariety * getStance(UnitEntity * unit) ;
  StanceVariety * getStance(FactionEntity * entity) ;
  void setStance(Entity * entity, StanceVariety * stance);
  void setDefaultStance(StanceVariety * stance) {defaultStance_ = stance;}
  StanceVariety * getDefaultStance() {return defaultStance_;}
  int  withdraw(ItemRule * item, int number);
  bool mayWithdraw(ItemRule * item, int number);
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
  bool isNPCFaction();
    protected:
	vector<Rule *> knowledge_;
  int newKnowledge; // number of new knowledge learned this turn
	vector<SkillLevelElement *> skillKnowledge_;
  unsigned newSkillKnowledge; // number of new skill learned this turn
  vector < BasicRulesCollection  *> collectionsToReshow_;
  vector < Rule  *> knowledgeToReshow_;
  bool allSkillsToReshow_;
  vector<SkillLevelElement> skillsToReshow_;
  string email_;
  string password_;
	StanceVariety * defaultStance_;
	vector<StanceElement> stances_;
  vector<LocationEntity *> visitedLocations_;
  vector<UnitEntity *> loyalUnits_;
  vector<ConstructionEntity *> loyalConstructions_;
  vector<ItemElement *> funds_;
  int maxControlPoints_;
  int controlPoints_;
  bool terseBattleReport_;
  bool isResigned_;
    private:
 };
typedef	vector<Rule *>::iterator KnowledgeIterator; 
extern FactionEntity    sampleFaction;
#include "EntitiesCollection.h"
extern EntitiesCollection <FactionEntity>   factions;
#endif
