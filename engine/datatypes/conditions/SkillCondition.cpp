/***************************************************************************
                          SkillCondition.cpp  -  description
                             -------------------
    begin                : Sun Jan 26 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "SkillCondition.h"
#include "UnitEntity.h"
#include "SkillLevelElement.h"
GameData * SkillCondition::createInstanceOfSelf()
{
   return CREATE_INSTANCE<SkillCondition> (this);
}



bool SkillCondition::isSatisfied(UnitEntity * unit)
{
	return unit->hasSkill(skillRequirement_);
}



STATUS SkillCondition::initialize ( Parser *parser )
{
  skillRequirement_ = SkillLevelElement::readElement(parser);
  if(skillRequirement_)
      skillRequirement_->getSkill()->bindCondition(this);
  return OK;
}



void SkillCondition::conditionHandler(Entity * entity)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *> ( entity);
  if (entity == 0)
      return;

  if(unit->hasSkill(skillRequirement_))
      return;

  unit->equipItem(subject_,0);      
}



void SkillCondition::setSubject(Rule * subject)
{
  subject_ = dynamic_cast<ItemRule * >(subject);
}



void    SkillCondition::extractKnowledge (Entity * recipient, int parameter = 0)
{
  if(recipient->addSkillKnowledge(skillRequirement_->getSkill(), skillRequirement_->getLevel()))
    skillRequirement_->getSkill()->extractKnowledge(recipient, skillRequirement_->getLevel());
}
