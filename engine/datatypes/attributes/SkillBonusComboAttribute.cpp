/***************************************************************************
                          SkillBonusComboAttribute.cpp  -  description
                             -------------------
    begin                : Feb 16 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "SkillBonusComboAttribute.h"
#include "BonusElement.h"
#include "MovementVariety.h"
#include "Entity.h"

SkillBonusComboAttribute::SkillBonusComboAttribute()
{
 studyBonuses_ = SkillBonusAttribute("STUDY_BONUS", "Study bonuses");
 learningBonuses_ = SkillBonusAttribute("LEARNING_LEVEL_BONUS","Skill level bonuses");
 productionBonuses_ = SkillBonusAttribute("USE_BONUS","Production bonuses");
}






SkillBonusComboAttribute::~SkillBonusComboAttribute(){
}



STATUS
SkillBonusComboAttribute::initialize        ( Parser *parser )
{
		studyBonuses_.initialize(parser);
		learningBonuses_.initialize(parser);
		productionBonuses_.initialize(parser);
   return OK;
}



int SkillBonusComboAttribute::getProductionBonus(SkillRule * skill)
{
	return productionBonuses_.getSkillBonus(skill);
}


int SkillBonusComboAttribute::getStudyBonus(SkillRule * skill)
{
	return studyBonuses_.getSkillBonus(skill);
}


int SkillBonusComboAttribute::getLearningBonus(SkillRule * skill)
{
	return learningBonuses_.getSkillBonus(skill);
}









void    SkillBonusComboAttribute::extractKnowledge (Entity * recipient, int parameter)
{
 	productionBonuses_.extractKnowledge(recipient, 1);
	studyBonuses_.extractKnowledge(recipient, 1);
	learningBonuses_.extractKnowledge(recipient, 1);
}






ostream& SkillBonusComboAttribute::report(ostream &out)
{
	productionBonuses_.report(out);
	studyBonuses_.report(out);
	learningBonuses_.report(out);
		return out;
}

