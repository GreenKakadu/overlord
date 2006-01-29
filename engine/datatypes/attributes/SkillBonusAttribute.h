/***************************************************************************
                          SkillBonusAttribute.h
                             -------------------
    begin                : Tue Jun 8 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef SKILL_BONUS_ATTRIBUTE_H
#define SKILL_BONUS_ATTRIBUTE_H

#include "BasicAttribute.h"
#include "BonusElement.h"

/**
  *@author Alex Dribin
  *
  * used for representing effect on Skill uese or learning
  */
class SkillRule;
class Entity;

class SkillBonusAttribute : public BasicAttribute  {
public: 
	SkillBonusAttribute();
	SkillBonusAttribute(const char * keyword, const char * reportTitle= "Skill bonuses");
	~SkillBonusAttribute();
  STATUS     initialize      ( Parser *);
	int getSkillBonus(SkillRule * skill);
  void add( BonusElement * data);
  void save(ostream &out);
  ostream& report(ostream &out);
  void remove( BonusElement * data);
  void extractKnowledge (Entity * recipient, int parameter);
    protected:
  vector <BonusElement> skillBonuses_;
	const char * keyword_;
	const char * reportTitle_;
};

#endif
