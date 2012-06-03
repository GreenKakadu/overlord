/***************************************************************************
                          ItemRule.cpp  -  description
                             -------------------
    begin                : Tue Dec 10 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "StringData.h"
#include "GameFacade.h"
#include "ItemRule.h"
#include "EquipmentSlotVariety.h"
#include "SkillLevelElement.h"
#include "BasicCondition.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "CombatActionStrategy.h"
#include "PrototypeManager.h"

ItemRule      sampleItem      ("ITEM",     &sampleGameData);
//RulesCollection <ItemRule>      items(new DataStorageHandler("items.rules"),&sampleItem);

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
		combatAction_ = 0;
		prospectSkill_ = 0;
                isRegeneratingResource_ = true;
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
  if (parser->matchKeyword("NONREGENERATING"))
  {
        isRegeneratingResource_ = false;
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
      if(gameFacade->movementModes.isValidTag(modeTag))
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
      if(gameFacade->movementModes.isValidTag(modeTag))
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
      equipSlot_ = gameFacade->equipments[index - 1];
    }
    else
      equipSlot_ = gameFacade->equipments[parser->getWord()];
    return OK;
  }
  if (parser->matchKeyword("EQUIP_CONDITION"))
  {
    
    equipCondition_ = dynamic_cast<BasicCondition *> (createByKeyword(parser->getWord()));
    if(equipCondition_)
    {
      equipCondition_->initialize(parser) ;
    }
    return OK;
  }
  if (parser->matchKeyword("USE_SKILL"))
  {
    SkillRule *skill = gameFacade->skills[parser->getWord()];
    if (skill == 0)
      return OK;
    else
      useSkill_ = new SkillLevelElement(skill,parser->getInteger());
    return OK;
  }
  
  if (parser->matchKeyword("PROSPECT_SKILL"))
  {
    SkillRule *skill = gameFacade->skills[parser->getWord()];
    if (skill == 0)
      return OK;
    else
      prospectSkill_ = new SkillLevelElement(skill,parser->getInteger());
    return OK;
  }
  
  if (parser->matchKeyword("SPECIAL"))
  {
    special_ = true;
    return OK;
  }
  
  if (parser->matchKeyword ("COMBAT_USE") )
  {
    string keyword = parser->getWord();
    GameData * temp =  prototypeManager->findInRegistry(keyword);
    if(temp == 0)
    {
      cerr << "Unknown combat action " << keyword  << " for item " << print()<< endl;
    }
    else
    {
      combatAction_ =
      dynamic_cast<CombatActionStrategy *>(temp ->createInstanceOfSelf ());
      
    }
    return OK;
  }
  
  if (parser->matchKeyword("COMBAT"))
  {
    if(combatAction_ == 0)
    {
      cerr << "combat parameter "<< parser->getText()<< " defined before combat action  for item " << print()<< endl;
      
      return OK;
    }
    combatAction_->initialize(parser);
    return OK;
  }
  
  
  if (parser->matchKeyword("MAGIC"))
  {
    magic_ = true;
    return OK;
  }

  stats.initialize(parser);
  skillBonuses_.initialize(parser);
  Rule::initialize(parser);
  return OK;
  
}

void ItemRule::save(ostream &out)
{
  Rule::save(out);
  if(!pluralName_.empty()) out << "PLURAL "<<pluralName_<<endl;
  if(weight_) out << "WEIGHT "<<weight_ <<endl;
  if(unique_) out << "UNIQUE "<<endl;
  if(price_) out << "PRICE "<< price_<<endl;
  if(live_) out << "LIVE " <<endl;
  if(!isRegeneratingResource_) out << "NONREGENERATING "<<endl;
  for(int i =0; i <gameFacade->movementModes.size(); ++i)
  {
    if(capacity_[i]) out << "CAPACITY "<<(gameFacade->movementModes[i])->getTag()<<" " << capacity_[i] <<endl;
  }

  for(int i =0; i <gameFacade->movementModes.size(); ++i)
  {
    if(equipCapacity_[i]) out << "EQUIP_CAPACITY "<<(gameFacade->movementModes[i])->getTag()<<" " <<equipCapacity_[i] <<endl;
  }

  if(equipSlot_) out << "EQUIP_CATEGORY "<<equipSlot_->getTag()<<endl;
  if(equipCondition_) 
  {
      out << "EQUIP_CONDITION "<<equipCondition_->getKeyword()<<" ";
      equipCondition_->save(out);
  }
  if(useSkill_) {out << "USE_SKILL "; useSkill_->save(out);}
  if(prospectSkill_ ) {out << "PROSPECT_SKILL ";prospectSkill_->save(out);}
  if(special_) out << "SPECIAL " <<endl;
  if(combatAction_) 
  {
      out << "COMBAT_USE "<< " "<<combatAction_->getKeyword() <<endl;
      combatAction_->save(out);
  }
  if(magic_) out << "MAGIC "<<endl;

  stats.save(out,"",0);
  skillBonuses_.save(out);
}


void    ItemRule::extractKnowledge (Entity * recipient, int parameter)
{
//    if(this==items["adam"])
//    {
//        cout<<"extracting Knowledge for "<< recipient->print()<<" from "<<print()<<endl;
//    }
  Rule::extractKnowledge(recipient);
  if(useSkill_)
  {
    if(recipient->addSkillKnowledge(useSkill_->getSkill(),useSkill_->getLevel()))
      useSkill_->getSkill()->extractKnowledge(recipient,useSkill_->getLevel());
  }
  if(equipCondition_)    
    equipCondition_->extractKnowledge(recipient);

  if(prospectSkill_)
  {
      if(recipient->addSkillKnowledge(prospectSkill_->getSkill(),prospectSkill_->getLevel()))
  {
     prospectSkill_->getSkill()->extractKnowledge(recipient,prospectSkill_->getLevel());
  }
}
}



void ItemRule::printDescription(ReportPrinter & out)
{
  out << print()<< ": ";
  if(weight_)
  {
    out <<"weight "  <<weight_ ;
    bool isFirst = true;
    for(int i =0 ; i < gameFacade->movementModes.size(); ++i)
    {
      if(capacity_[i])
          {
            if(isFirst)
              {
                out << ", capacity: ";
                isFirst = false;
              }
              else
                out << ", ";

              out << capacity_[i]<< "/" << (gameFacade->movementModes[i])->getName();
              if(equipCapacity_[i])
                out << " ("<<equipCapacity_[i]<< " while equiped)";
          }
            

      }
      out<<". ";
    }
  out << getDescription()<<".";

  if(stats.getUpkeep())
    out << " Additional upkeep $" << stats.getUpkeep()<<" per item.";

  if(stats.getControlPoints())
    out << " Required control " << stats.getControlPoints()<<" per item.";

  if(equipCondition_)
  {
    out << " Equipped with "<< *equipCondition_<<".";
  }

   if(equipSlot_)
   {
     out << " Equipment category: " << equipSlot_->getName();
     if(numEquipSlotsRequired_ > 1)
        out << " ( requires "<< numEquipSlotsRequired_ << " "<< equipSlot_->getName() << " to equip )";
     out<<".";
   }
   if(!stats.empty())
   {
      out << " Equipment gives " << stats;
   }
}



void ItemRule::applyEquipementEffects(UnitEntity * unit, int number)
{
/*  if(learningLevelBonus_)
  {
    if(unit->getFiguresNumber() <= number)
    {
      unit->addLearningLevelBonus(learningLevelBonus_);
    }
    else
    {
      unit->removeLearningLevelBonus(learningLevelBonus_);
    }
  }*/
}



int ItemRule::getProductionBonusValue(SkillRule * skill)
{
  return skillBonuses_.getProductionBonus(skill);
}



int ItemRule::getLearningBonus(SkillRule * skill)
{
  return skillBonuses_.getLearningBonus(skill);
}



int ItemRule::getStudyBonus(SkillRule * skill)
{
  return skillBonuses_.getStudyBonus(skill);
}



vector <AbstractData *> ItemRule::aPrint()
{
  vector <AbstractData *> v;
  v.push_back(this);
  v.push_back(new StringData(": "));
  if(weight_)
  {
    v.push_back(new StringData("weight "));
    v.push_back(new IntegerData(weight_));
    bool isFirst = true;
    for(int i =0 ; i < gameFacade->movementModes.size(); ++i)
    {
      if(capacity_[i])
      {
        if(isFirst)
        {
          v.push_back(new StringData(", capacity: "));
          isFirst = false;
        }
        else
        {
          v.push_back(new StringData(", "));
        }

        v.push_back(new IntegerData(capacity_[i]));
        v.push_back(new StringData("/"));
        v.push_back(new StringData((gameFacade->movementModes[i])->getName()));

        if(equipCapacity_[i])
        {
          v.push_back(new StringData(" ("));
          v.push_back(new IntegerData(equipCapacity_[i]));
          v.push_back(new StringData(" while equiped)"));
        }
      }
    }

  }

  v.push_back(new StringData(". "));
  if(stats.getUpkeep())
  {
      v.push_back(new StringData("Additional upkeep $"));
      v.push_back(new IntegerData(stats.getUpkeep()));
      v.push_back(new StringData(" per item. "));
  }
  if(stats.getControlPoints())
  {
      v.push_back(new StringData(" Required control"));
      v.push_back(new IntegerData(stats.getControlPoints()));
      v.push_back(new StringData(" per item. "));
  }

       if(equipCondition_)
     {
           vector <AbstractData *> condition = equipCondition_->aPrint();
           v.push_back(new StringData(" Equipped with "));
           for(vector <AbstractData *>::iterator iter = condition.begin();
           iter != condition.end(); ++iter)
           {
             v.push_back(*iter);
            }
           v.push_back(new StringData(". "));
     }

       if(equipSlot_)
       {
           v.push_back(new StringData(" Equipment category: "));
           v.push_back(new StringData(equipSlot_->getName()));

           if(numEquipSlotsRequired_ > 1)
           {
               v.push_back(new StringData(" ( requires "));
               v.push_back(new IntegerData(numEquipSlotsRequired_));
               v.push_back(new StringData(" "));
               v.push_back(new StringData(equipSlot_->getName()));
               v.push_back(new StringData(" to equip ). "));
           }
           v.push_back(new StringData(". "));
       }
       if(!stats.empty())
       {
           bool isFirst = true;
           v.push_back(new StringData(" Equipment gives "));
           vector<AbstractArray> statPrint = stats.aPrint();
           for(vector <AbstractArray>::iterator iter = statPrint.begin();
           iter != statPrint.end(); ++iter)
           {
               if(!isFirst)
               {
                   v.push_back(new StringData(", "));
               }
               else
               {
                   isFirst = false;
               }
               for(vector <AbstractData *>::iterator iter2 = (*iter).begin();
               iter2 != (*iter).end(); ++iter2)
               {
                   v.push_back(*iter2);
               }
           }
           v.push_back(new StringData(". "));

       }
  v.push_back(new StringData(getDescription()));

  return v;
}

 


