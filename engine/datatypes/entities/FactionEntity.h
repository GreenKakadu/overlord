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

class FactionEntity : public Entity  {
    public:
      FactionEntity (const string & keyword, Entity * parent ) : Entity(keyword, parent){}
      FactionEntity ( const FactionEntity * prototype );
      STATUS  initialize      ( Parser *parser );
       void      save (ostream &out);
      GameData * createInstanceOfSelf();
      void  print();
  void    preprocessData();
  /** Loads orders */
  void loadOrders();
  inline void setEMail(const string& email) { email_ = email;}
  inline void setPassword(const string& password)  {password_ = password;}
  /** No descriptions */
  void addVisitedLocation(LocationEntity * location);
  /** No descriptions */
  void clearVisitedLocations();
  /** No descriptions */
  void dailyReport();
  /** Adds report to the list if it is different from those, that are already there */
  void updateReports(ReportElement * report);
  /** No descriptions */
  void saveReport();
  /** No descriptions */
  void dailyUpdate();
  /** register unit */
  void addUnit(UnitEntity * unit);
  void removeUnit(UnitEntity * unit);
  /** No descriptions */
  virtual Entity * getReportDestination();
  /** No descriptions */
  StanceVariety * getStance(UnitEntity * unit) ;
  StanceVariety * getStance(FactionEntity * entity) ;
  /** No descriptions */
  void setStance(Entity * entity, StanceVariety * stance);
  /** No descriptions */
  int  withdraw(ItemRule * item, int number);
  bool mayWithdraw(ItemRule * item, int number);
//===============================  Knowledge ============
  bool addKnowledge(Rule * info);
//  void addKnowledge(SkillLevelElement & info);
  bool addSkillKnowledge(SkillRule * knowledge, int level);
  Rule * hasKnowledge(Rule * info);
//SkillLevelElement *   hasSkillKnowledge(SkillLevelElement & info);
  SkillLevelElement * hasSkillKnowledge(SkillRule * knowledge, int level);
  void reshow(Rule * info, ostream &out);
  void reshow(BasicRulesCollection  * collection, ostream &out );
//  void reshow(SkillLevelElement & info, ostream &out);
  void reshowSkill(SkillRule * knowledge, int level, ostream &out);
  void reshowSkills(ostream &out );
  void saveKnowledge(ostream &out);
  void reportNewKnowledge(ostream &out);
  void loadKnowledge(Parser *parser);
    protected:
	vector<Rule *> knowledge_;
  int newKnowledge;
	vector<SkillLevelElement *> skillKnowledge_;
  int newSkillKnowledge;

  string email_;
  string password_;
	StanceVariety * defaultStance_;
	vector<StanceElement *> stances_;
  vector<LocationEntity *> visitedLocations_;
  vector<UnitEntity *> loyalUnits_;
  vector<ItemElement *> funds_;
    private:
 };
typedef vector<StanceElement *>::iterator StanceIterator;
typedef vector<SkillLevelElement *>::iterator SkillLevelIterator; 
typedef	vector<Rule *>::iterator KnowledgeIterator; 
#endif
