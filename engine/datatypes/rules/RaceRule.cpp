/***************************************************************************
                          RaceRule.cpp  -  description
                             -------------------
    begin                : Tue Dec 10 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "RaceRule.h"
#include "EquipmentSlotVariety.h"
#include "RulesCollection.h"
#include "PrototypeManager.h"
extern RulesCollection <SkillRule>     skills;
extern VarietiesCollection <EquipmentSlotVariety>      equipments;

equip_slot::equip_slot(EquipmentSlotVariety * slotRule, int  cap)
{
	type = slotRule;
  capacity = cap;
	if(capacity == 0)
				capacity = 1;  // default
}
equip_slot::~equip_slot()
{
}
RaceRule::RaceRule ( const RaceRule * prototype ) : Rule(prototype)
{
	weight_ = 0;
  hiringCost_ = 50;
  hiringProbability_ = 1000;
  hiringMax_ = 1000;
 	equipmentSlots_.push_back(new EquipSlot(equipments["equip_food"]));
 	equipmentSlots_.push_back(new EquipSlot(equipments["equip_mount"]));
 	equipmentSlots_.push_back(new EquipSlot(equipments["equip_tool"]));
 	equipmentSlots_.push_back(new EquipSlot(equipments["equip_ship"]));
 	equipmentSlots_.push_back(new EquipSlot(equipments["equip_weapon"]));
 	equipmentSlots_.push_back(new EquipSlot(equipments["equip_armor"]));
 	equipmentSlots_.push_back(new EquipSlot(equipments["equip_helmet"]));
 	equipmentSlots_.push_back(new EquipSlot(equipments["equip_shield"]));
 	equipmentSlots_.push_back(new EquipSlot(equipments["equip_gloves"]));
 	equipmentSlots_.push_back(new EquipSlot(equipments["equip_boots"]));
 	equipmentSlots_.push_back(new EquipSlot(equipments["equip_amulet"]));
 	equipmentSlots_.push_back(new EquipSlot(equipments["equip_ring"], 2));
 	equipmentSlots_.push_back(new EquipSlot(equipments["equip_misc"]));
 	equipmentSlots_.push_back(new EquipSlot(equipments["equip_cloak"]));
}



GameData * RaceRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<RaceRule> (this);
}


STATUS
RaceRule::initialize        ( Parser *parser )
{

	
  if (parser->matchKeyword ("NAME") )
    {
      setName(parser->getText());
      return OK;
    }
  if (parser->matchKeyword("TEXT"))
    {
      setDescription(parser->getText());
      return OK;
    }
  if (parser->matchKeyword("PLURAL"))
    {
      setPluralName(parser->getText());
      return OK;
    }
  if (parser->matchKeyword("WEIGHT"))
    {
      weight_ = parser->getInteger();
      return OK;
    }
  if (parser->matchKeyword("FRACTION"))
    {
      hiringProbability_ = parser->getInteger();
      return OK;
    }
  if (parser->matchKeyword("MAXIMUM"))
    {
      hiringMax_ = parser->getInteger();
      return OK;
    }
  if (parser->matchKeyword("COST"))
    {
      hiringCost_= parser->getInteger();
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
  if (parser->matchKeyword("SKILL"))
    {
			SkillRule *skill = skills[parser->getWord()];
			if (skill == 0)
					return OK;
			else
				{
					SkillElement newSkill(skill,parser->getInteger());
					intristicSkills_.push_back(newSkill);
				}
      return OK;
    }
  if (parser->matchKeyword("FAMILIAR"))
    {
			SkillRule *skill = skills[parser->getWord()];
			if (skill == 0)
					return OK;
			else
				{
					BonusElement newBonus(skill,parser->getInteger());
					intristicSkillBonuses_.push_back(newBonus);
				}
      return OK;
    }
  if (parser->matchKeyword("EQUIPMENT_SLOT"))
    {
      string slotId = parser->getWord();
			if(slotId.empty())
				return OK;
    	EquipmentSlotVariety * slot = equipments[slotId];
			if(slot)	
						{		
							EquipSlotIterator iter;
							for(iter = equipmentSlots_.begin(); iter != equipmentSlots_.end(); iter++)
							{
								if( (*iter)->type == slot )
									equipmentSlots_.erase (iter);
								}

 							equipmentSlots_.push_back(new EquipSlot(slot, parser->getInteger()));
						}
       return OK;
    }
     stats.initialize(parser);
      return OK;

 }

//void RaceRule::print()
//{
//    cout  << getName() << " [" << getTag()  << "] "<< endl;
// }


/** Returns number of items that may be equiped in this category. */
int RaceRule::getEquipCapacity(EquipmentSlotVariety *  type)
{
 EquipSlotIterator iter;
	if(type == 0)
			return 0;
	for(iter = equipmentSlots_.begin(); iter != equipmentSlots_.end(); iter++)
		{
			if( (*iter)->type == type )
				return (*iter)->capacity;
		}
	return 0;
}
/** No descriptions */
int RaceRule::getBonus(SkillRule * skill){
  vector <BonusElement >::iterator iter;
  for(iter = intristicSkillBonuses_.begin();
    iter != intristicSkillBonuses_.end();
      iter++)
    {
        if ((*iter).getSkill() == skill)
          return (*iter).getBonusPoints();
    }
  return 0;
}

LEARNING_RESULT RaceRule::mayLearn(SkillRule * skill, UnitEntity * unit)
{
   cout <<  keyword_ << " considered as a basic Student "<<endl;
   return CANNOT_STUDY_FAILURE;
//  return   learningAbilities_->mayLearn(skill, unit);
}
bool RaceRule::teacherRequired(SkillRule * skill, UnitEntity * unit)
{
  return false;
}

void   RaceRule::setPluralName(const string name)
{
  pluralName_ = name;
}



bool RaceRule::mayMove()
{
  return true;
}

/** No descriptions */
bool RaceRule::mayWork()
{
  return true;
}



bool RaceRule::mayRectuit()
{
  return false;
}



bool RaceRule::mayTrade()
{
  return false;
}

bool RaceRule::mayTransferFigures()
{
  return true;
}



vector< EquipSlot *>& RaceRule::getAllEquipmentSlots()
{
  return equipmentSlots_;
}



/** How many seats occupies this entity in the class. Number of entities that can be tought by one teacher determined by this value. */
int RaceRule::getLearningCapacity()
{
  return 10000;  // very big. can't study
}



void    RaceRule::extractKnowledge (Entity * recipient, int parameter)
{
  for(vector <SkillElement >::iterator iter = intristicSkills_.begin();
                                 iter != intristicSkills_.end(); ++iter)
    {
      if((*iter).getSkill())
      {
        if((*iter).getSkill())
          {
            if(recipient->addSkillKnowledge((*iter).getSkill(),(*iter).getLevel()))
                (*iter).getSkill()->extractKnowledge(recipient, (*iter).getLevel());
          }
      }
  }

  for(vector <BonusElement >::iterator iter = intristicSkillBonuses_.begin();
                                 iter != intristicSkillBonuses_.end(); ++iter)
    {
      if((*iter).getSkill())
      {
        if((*iter).getSkill())
          {
            if(recipient->addSkillKnowledge((*iter).getSkill(),0))
                (*iter).getSkill()->extractKnowledge(recipient, 0);
          }
      }
  }
}
