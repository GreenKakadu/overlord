/***************************************************************************
                          ConstructionRule.h
                             -------------------
    begin                : Sun Aug 31 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
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
#include "SkillBonusComboAttribute.h"

/**
  *@author Alex Dribin
  */
class ConstructionWorksElement;
class SkillElement;
class BasicCondition;
class SkillCondition;
class ConstructionEntity;
class UnitEntity;
class ConstructionWorksVariety;
class TitleRule;
class LocationEntity;
class BuildUsingStrategy;

class ConstructionRule : public Rule  {
public: 
      ConstructionRule ( const string & keyword, GameData * parent) : Rule(keyword, parent){}
      ConstructionRule ( const ConstructionRule * prototype );
      virtual STATUS     initialize      ( Parser *parser);
      virtual void save(ostream &out);
      GameData * createInstanceOfSelf();
	   ~ConstructionRule();
              ConstructionEntity * startConstruction(UnitEntity * unit);
        void finishConstruction(LocationEntity * location);
        void extractKnowledge (Entity * recipient, int parameter = 0);
        void printDescription(ReportPrinter & out);
        vector <AbstractData *> aPrint();
  inline BasicCondition * getBuildCondition() const {return buildCondition_;}
  inline BasicCondition * getStaffCondition() const {return staffCondition_;}
  inline EntityStatistics * getStats()  {return &stats_;}
  inline EntityStatistics * getBonusStats()  {return &bonuses_;}
  inline int getLandUse() const {return landUse_;}
         int getResourceRequirement(ConstructionWorksVariety * resource);
  inline int getCapacity(int modeIndex){return capacity_[modeIndex];}
  inline int getCapacity(MovementVariety * mode){return capacity_[mode];}
         int getProductionBonusValue(SkillRule * skill);
         int getStudyBonus(SkillRule * skill);
  inline int getEconomyBonus() const {return economyBonus_;};
         int getLearningBonus(SkillRule * skill);
         inline SkillElement * getBuildingSkill(){return buildingSkill_;};
  inline int getWeight() const {return weight_;}
  inline int getMaxStaff() const {return maxStaff_;}
  inline bool isBattle()const {return isBattle_;}
         bool mayMove();
  inline TitleRule * getTitle() const {return generateTitle_;}
  USING_RESULT startNewConstruction(UnitEntity * unit,ConstructionEntity *buildingOrShip, AbstractData * target =0);
  USING_RESULT buildExistingConstruction( UnitEntity * unit, SkillRule * skill, int &useCounter,OrderLine * order);
	protected:
      vector<ConstructionWorksElement *>    resources_;
      BuildUsingStrategy * buildingParadigm_;
      MovementMode<int> capacity_;
      TitleRule * generateTitle_;
      SkillElement * skill_ ;
      SkillElement * buildingSkill_ ;
      int weight_;
      int landUse_;
      BasicCondition * buildCondition_;
      BasicCondition * staffCondition_;
      int maxStaff_;
      EntityStatistics stats_;
      EntityStatistics bonuses_;
      /*const*/ string entityKeyword_ ;
      bool mobile_;
      bool isBattle_;
      int economyBonus_;
      // Attributes
      SkillBonusComboAttribute skillBonuses_;
//		  SkillBonusAttribute useBonuses_;
//		  SkillBonusAttribute studyBonuses_;
//		  SkillBonusAttribute learningBonuses_;
    private:
};
typedef vector <BonusElement *>::iterator BonusIterator;
typedef vector<ConstructionWorksElement *>::iterator ConstructionWorksIterator;
extern ConstructionRule   sampleConstructionRule;
//extern RulesCollection <ConstructionRule>      constructions;

#endif
