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
SkillCondition  sampleSkillCondition ("SKILL_CONDITION", &sampleGameData);
SkillCondition::SkillCondition(const SkillCondition * prototype):BasicCondition(prototype)
{
	skillRequirement_ = 0;
  subject_ = 0;
}

GameData * SkillCondition::createInstanceOfSelf()
{
   return CREATE_INSTANCE<SkillCondition> (this);
}



bool SkillCondition::isSatisfied(TokenEntity * unit)
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





Rule * SkillCondition::getSubject() const
{
  if(skillRequirement_ == 0)
    return 0;
  else
  return skillRequirement_->getSkill();
}



SkillRule * SkillCondition::getSkill() const 
{
  if(skillRequirement_ == 0)
    return 0;
  else
  return skillRequirement_->getSkill();
}


int  SkillCondition::getLevel() const 
{
  if(skillRequirement_ == 0)
    return 0;
  else
  return skillRequirement_->getLevel();
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



void    SkillCondition::extractKnowledge (Entity * recipient, int parameter )
{
  if(recipient->addSkillKnowledge(skillRequirement_->getSkill(), skillRequirement_->getLevel()))
    skillRequirement_->getSkill()->extractKnowledge(recipient, skillRequirement_->getLevel());
}



//ostream & SkillCondition::reportCondition(ostream & out)
//{
//   skillRequirement_->print(out);
//   return  out;
//}
