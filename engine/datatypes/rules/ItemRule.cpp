/***************************************************************************
                          ItemRule.cpp  -  description
                             -------------------
    begin                : Tue Dec 10 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "ItemRule.h"
#include "EquipmentSlotVariety.h"
#include "SkillLevelElement.h"
#include "BasicCondition.h"
#include "Entity.h"

extern VarietiesCollection <EquipmentSlotVariety>      equipments;

ItemRule::ItemRule ( const string & keyword, GameData * parent): Rule(keyword, parent)
{
}
ItemRule::ItemRule ( const ItemRule * prototype ) : Rule(prototype)
{
		 equipSlot_ = 0;
		 equipCondition_ = 0;
		 useSkill_ = 0;
		 unique_ = false;
		 special_ = false;
		 magic_ = false;
		 live_ = false;
    numEquipSlotsRequired_  = 1;
}



GameData * ItemRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<ItemRule> (this);
}


STATUS
ItemRule::initialize        ( Parser *parser )
{

	
  if (parser->matchKeyword ("NAME") )
    {
      setName(parser->getText());
      return OK;
    }
  if (parser->matchKeyword("DESCRIPTION"))
    {
      setDescription(parser->getText());
      return OK;
    }
  if (parser->matchKeyword("PLURAL"))
    {
      pluralName_ = parser->getText();
      return OK;
    }
  if (parser->matchKeyword("WEIGHT"))
    {
      weight_ = parser->getInteger();
      return OK;
    }
  if (parser->matchKeyword("UNIQUE"))
    {
			unique_ = true;
      return OK;
    }
  if (parser->matchKeyword("PRICE"))
    {
      price_ = parser->getInteger();
      return OK;
    }
  if (parser->matchKeyword("LIVE"))
    {
			live_ = true;
      return OK;
    }
  if (parser->matchKeyword("CAPACITY"))
    {
      if(parser->matchInteger())
				{
					int index =  parser->getInteger();
					capacity_[index]  = parser->getInteger();
				}
			else
					{
						string modeTag = parser->getWord();
						if(movementModes.isValidTag(modeTag))
							{
								capacity_[modeTag]  = parser->getInteger();
							}
					}
      return OK;
    }
  if (parser->matchKeyword("EQUIP_CAPACITY"))
    {
      if(parser->matchInteger())
				{
					int index =  parser->getInteger();
					equipCapacity_[index]  = parser->getInteger();
				}
			else
					{
						string modeTag = parser->getWord();
						if(movementModes.isValidTag(modeTag))
							{
								equipCapacity_[modeTag]  = parser->getInteger();
							}
					}
      return OK;
    }
  if (parser->matchKeyword("EQUIP_CATEGORY"))
    {
      int index =  parser->getInteger();
      if(index)
				{
					equipSlot_ = equipments[index - 1];
				}
			else
					equipSlot_ = equipments[parser->getWord()];
      return OK;
    }
  if (parser->matchKeyword("EQUIP_CONDITION"))
    {
      
      equipCondition_ = dynamic_cast<BasicCondition *> (createByKeyword(parser->getWord()));
      if(equipCondition_)
      {
        equipCondition_->initialize(parser) ;
        equipCondition_->setSubject(this);
        }
      return OK;
    }
if (parser->matchKeyword("USE_SKILL"))
    {
			SkillRule *skill = skills[parser->getWord()];
			if (skill == 0)
					return OK;
			else
					useSkill_ = new SkillLevelElement(skill,parser->getInteger());	
      return OK;
    }
if (parser->matchKeyword("SPECIAL"))
    {
			special_ = true;
      return OK;
    }
if (parser->matchKeyword("MAGIC"))
    {
			magic_ = true;
      return OK;
    }
     stats.initialize(parser);
      return OK;

 }

//void ItemRule::print()
//{
//    cout  << getName() << " [" << getTag()  << "] "<< endl;
// }


void    ItemRule::extractKnowledge (Entity * recipient, int parameter)
{
  if(useSkill_)
  {
    if(recipient->addSkillKnowledge(useSkill_->getSkill(),useSkill_->getLevel()))
      useSkill_->getSkill()->extractKnowledge(recipient,useSkill_->getLevel());
  }
  if(equipCondition_)    
    equipCondition_->extractKnowledge(recipient);
}

