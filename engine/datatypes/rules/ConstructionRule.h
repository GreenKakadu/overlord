/***************************************************************************
                          ConstructionRule.h
                             -------------------
    begin                : Sun Aug 31 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef CONSTRUCTION_RULE_H
#define CONSTRUCTION_RULE_H

#include "Rule.h"
#include "EntityStatistics.h"
#include "MovementMode.h"

/**
  *@author Alex Dribin
  */
class ConstructionWorksElement;
class SkillElement;
class BonusElement;
class BasicCondition;
class SkillCondition;
class ConstructionEntity;
class UnitEntity;
class ConstructionWorksVariety;
class TitleRule;
class LocationEntity;

class ConstructionRule : public Rule  {
public: 
      ConstructionRule ( const string & keyword, GameData * parent) : Rule(keyword, parent){}
      ConstructionRule ( const ConstructionRule * prototype );
      virtual STATUS     initialize      ( Parser *parser);
      GameData * createInstanceOfSelf();
	   ~ConstructionRule();
  ConstructionEntity * startConstruction(UnitEntity * unit);
        void finishConstruction(LocationEntity * location);
        void extractKnowledge (Entity * recipient, int parameter = 0);
        void printDescription(ReportPrinter & out);
  inline BasicCondition * getBuildCondition() const {return buildCondition_;}
  inline SkillCondition * getStaffCondition() const {return staffCondition_;}
  inline EntityStatistics * getStats()  {return &stats_;}
  inline int getLandUse() const {return landUse_;}
         int getResourceRequirement(ConstructionWorksVariety * resource);
  inline int getCapacity(int modeIndex){return capacity_[modeIndex];}
         int getProductionBonusValue(SkillRule * skill,int level);
         int getBonus(SkillRule * skill);
  inline int getWeight() const {return weight_;}
  inline int getMaxStaff() const {return maxStaff_;}
        bool mayMove();
  inline TitleRule * getTitle() const {return generateTitle_;}
	protected:
      vector<ConstructionWorksElement *>    resources_;
      vector <BonusElement *>      skillBonuses_;
      vector <BonusElement *>      useBonuses_;
	    MovementMode<int> capacity_;
      TitleRule * generateTitle_;
      SkillElement * skill_ ;
      int weight_;
      int landUse_;
      BasicCondition * buildCondition_;
      SkillCondition * staffCondition_;
      int maxStaff_;
      EntityStatistics stats_;
      /*const*/ string entityKeyword_ ;
      bool mobile_;
    private:
};
typedef vector <BonusElement *>::iterator BonusIterator;
typedef vector<ConstructionWorksElement *>::iterator ConstructionWorksIterator;
extern ConstructionRule   sampleConstructionRule;
#include "RulesCollection.h"
extern RulesCollection <ConstructionRule>      constructions;

#endif
