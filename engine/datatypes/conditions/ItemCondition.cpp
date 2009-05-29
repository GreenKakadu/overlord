/***************************************************************************
                          ItemCondition.cpp  -  description
                             -------------------
    begin                : Aug 09 2006
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "ItemCondition.h"
#include "UnitEntity.h"

ItemCondition  sampleItemCondition ("ITEM_CONDITION", &sampleGameData);
ItemCondition::ItemCondition(const ItemCondition * prototype):BasicCondition(prototype)
{
	ItemRequirement_ = 0;
  subject_ = 0;
}

GameData * ItemCondition::createInstanceOfSelf()
{
   return CREATE_INSTANCE<ItemCondition> (this);
}



bool ItemCondition::isSatisfied(TokenEntity * unit)
{
	return (unit->hasItem(itemRequirement_->getItemType()) >= itemRequirement_->getItemNumber());
}



STATUS ItemCondition::initialize ( Parser *parser )
{
  itemRequirement_ = ItemElement::readElement(parser);
  if(itemRequirement_)
      itemRequirement_->getItemType()->bindCondition(this);
  return OK;
}





Rule * ItemCondition::getSubject() const
{
  if(itemRequirement_ == 0)
    return 0;
  else
  return itemRequirement_->getItemType();
}



ItemRule * ItemCondition::getItem() const
{
  if(itemRequirement_ == 0)
    return 0;
  else
  return itemRequirement_->getItemType();
}


int  ItemCondition::getNumber() const
{
  if(itemRequirement_ == 0)
    return 0;
  else
  return itemRequirement_->getItemNumber();
}



void ItemCondition::conditionHandler(Entity * entity)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *> ( entity);
  if (entity == 0)
      return;

  if(unit->hasSkill(skillRequirement_))
      return;

  unit->equipItem(subject_,0);      
}



void ItemCondition::setSubject(Rule * subject)
{
  subject_ = dynamic_cast<ItemRule * >(subject);
}



void    ItemCondition::extractKnowledge (Entity * recipient, int parameter )
{
  if(recipient->addSkillKnowledge(skillRequirement_->getSkill(), skillRequirement_->getLevel()))
    skillRequirement_->getSkill()->extractKnowledge(recipient, skillRequirement_->getLevel());
}



//ostream & ItemCondition::reportCondition(ostream & out)
//{
//   skillRequirement_->print(out);
//   return  out;
//}
