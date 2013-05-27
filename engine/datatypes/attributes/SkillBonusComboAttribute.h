/***************************************************************************
                          SkillBonusComboAttribute.h
                             -------------------
    begin                : Tue Jun 8 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef SKILL_BONUS_COMBO_ATTRIBUTE_H
#define SKILL_BONUS_COMBO_ATTRIBUTE_H

#include "SkillBonusAttribute.h"


/**
  *@author Alex Dribin
  *
  * used for representing effect on Skill uese or learning
  */
class SkillRule;
class Entity;

class SkillBonusComboAttribute : public BasicAttribute  {
public: 
	SkillBonusComboAttribute();
	~SkillBonusComboAttribute();
  STATUS     initialize      ( Parser *);
   void save(ostream &out);
   void clean();
 ostream& report(ostream &out);
  void extractKnowledge (Entity * recipient, int parameter);
  int getProductionBonus(SkillRule * skill);
  int getStudyBonus(SkillRule * skill);
  int getLearningBonus(SkillRule * skill);
    vector <AbstractArray>  aPrintReport();
    protected:
  SkillBonusAttribute productionBonuses_;
  SkillBonusAttribute studyBonuses_;// affect study time
  SkillBonusAttribute learningBonuses_;// affect max level
};

#endif
