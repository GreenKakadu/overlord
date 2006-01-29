/***************************************************************************
                          SkillsAttribute.h
                             -------------------
    begin                : Sun Apr 04 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef SKILLS_ATTRIBUTE_H
#define SKILLS_ATTRIBUTE_H

#include "BasicAttribute.h"
#include "SkillElement.h"
#include "SkillLevelElement.h"
class EntityStatistics;
class SkillRule;
class TokenEntity;
class FactionEntity;
/**
  *@author Alex Dribin
  */

class SkillsAttribute : public BasicAttribute  {
public:
	SkillsAttribute(){}
//	SkillsAttribute(TokenEntity * entity);
	SkillsAttribute(vector <SkillElement > &skills);
	~SkillsAttribute();
  STATUS     initialize      ( Parser *);
  void save(ostream &out);
  ostream& report(ReportPrinter &out);
  void reportAll(ReportPrinter &out);
  void reportAllShort(ReportPrinter &out);


	virtual int getProductionBonus(SkillRule * skill);
 	virtual int getMovementBonus(MovementVariety * mode);
  virtual int getLearningBonus(SkillRule * skill);
  virtual int getStudyBonus(SkillRule * skill);

          vector < SkillElement>& getAll();
          virtual int  addSkill(SkillRule  * skill, int expPoints);
          virtual void addSkillExperience(SkillRule  * skill, int expPoints);
          virtual int  getSkillLevel(SkillRule  * const skill);
          virtual int  getSkillPoints(SkillRule  * const skill);
          virtual SkillElement *  getSkillElement(SkillRule  * const skill);
          virtual bool hasSkill(SkillRule  * skill, int experience);

  inline  virtual bool hasSkillLevel(SkillRule  * skill, int level)
        {return hasSkill(skill, skill->getLevelExperience(level));}
  inline  virtual bool hasSkill(SkillLevelElement * skill)
        {return hasSkill(skill->getSkill(),
						 skill->getSkill()->getLevelExperience(skill->getLevel()));}
  inline virtual bool hasSkill(SkillElement  * skill)
        {return hasSkill(skill->getSkill(), skill->getExpPoints());}
  inline virtual bool hasSkill(SkillElement  skill)
        {return hasSkill(skill.getSkill(), skill.getExpPoints());}

				void clear();
         int              getCapacity(int modeIndex);
				 int              getCapacity(MovementVariety * mode);
				 void forgetSkill(SkillRule * skill, TokenEntity * entity);
         void  addStats(EntityStatistics * stats);
				 void proportionallyDiluteAll(int oldNumber, int newNumber,
				 		 TokenEntity * entity);


		protected:
  vector < SkillElement>      skills_;
//	TokenEntity * entity_;
};
#endif
