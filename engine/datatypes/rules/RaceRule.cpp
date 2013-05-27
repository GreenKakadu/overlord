/***************************************************************************
RaceRule.cpp  -  description
-------------------
begin                : Tue Dec 10 2002
copyright            : (C) 2002 by Alex Dribin
email                : Alex.Dribin@gmail.com
***************************************************************************/
#include "StringData.h"
#include "GameFacade.h"
#include "RaceRule.h"
#include "EquipmentSlotVariety.h"
#include "RulesCollection.h"
#include "PrototypeManager.h"

RaceRule      sampleRace      ("RACE",     &sampleGameData);
//RulesCollection <RaceRule>      races(new DataStorageHandler("races.rules"),&sampleRace);

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
  controlPointsFraction_ = 20;
  hiringCost_ = 50;
  hiringProbability_ = 1000;
  hiringMax_ = 1000;
  equipmentSlots_.push_back(new EquipSlot(gameFacade->equipments["equip_food"]));
  equipmentSlots_.push_back(new EquipSlot(gameFacade->equipments["equip_mount"]));
  equipmentSlots_.push_back(new EquipSlot(gameFacade->equipments["equip_tool"]));
  equipmentSlots_.push_back(new EquipSlot(gameFacade->equipments["equip_ship"]));
  equipmentSlots_.push_back(new EquipSlot(gameFacade->equipments["equip_weapon"]));
  equipmentSlots_.push_back(new EquipSlot(gameFacade->equipments["equip_armor"]));
  equipmentSlots_.push_back(new EquipSlot(gameFacade->equipments["equip_helmet"]));
  equipmentSlots_.push_back(new EquipSlot(gameFacade->equipments["equip_shield"]));
  equipmentSlots_.push_back(new EquipSlot(gameFacade->equipments["equip_gloves"]));
  equipmentSlots_.push_back(new EquipSlot(gameFacade->equipments["equip_boots"]));
  equipmentSlots_.push_back(new EquipSlot(gameFacade->equipments["equip_amulet"]));
  equipmentSlots_.push_back(new EquipSlot(gameFacade->equipments["equip_ring"], 2));
  equipmentSlots_.push_back(new EquipSlot(gameFacade->equipments["equip_misc"]));
  equipmentSlots_.push_back(new EquipSlot(gameFacade->equipments["equip_cloak"]));
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
  if (parser->matchKeyword("DESCRIPTION"))
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
      if(gameFacade->movementModes.isValidTag(modeTag))
      {
	capacity_[modeTag]  = parser->getInteger();
      }
    }
    return OK;
  }
  if (parser->matchKeyword("SKILL"))
  {
    SkillRule *skill = gameFacade->skills[parser->getWord()];
    if (skill == 0)
      return OK;
    else
    {
      SkillElement newSkill(skill,parser->getInteger());
      for(vector< SkillElement>::iterator iter = intristicSkills_.begin(); iter != intristicSkills_.end();
              ++iter)
      {
          if((*iter).getSkill() == skill)// If skill already present overwrite it
          {
            (*iter).setExpPoints(newSkill.getExpPoints());
            return OK;
          }
      }
      intristicSkills_.push_back(newSkill);
    }
    return OK;
  }
  
  if (parser->matchKeyword("EQUIPMENT_SLOT"))
  {
    string slotId = parser->getWord();
    if(slotId.empty())
      return OK;
    EquipmentSlotVariety * slot = gameFacade->equipments[slotId];
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
  skillBonuses_.initialize(parser);
  movementBonuses_.initialize(parser);
  Rule::initialize(parser);
  return OK;
  
}

void RaceRule::save(ostream &out)
{
  Rule::save(out);
  if(!pluralName_.empty()) out << "PLURAL "<<pluralName_<<endl;
  if(weight_) out << "WEIGHT "<<weight_ <<endl;
  if(hiringProbability_) out<<"FRACTION "<<hiringProbability_<<endl;
  if(hiringMax_) out<<"MAXIMUM "<<hiringMax_<<endl;
  if(hiringCost_) out<<"COST "<<hiringCost_<<endl;
  for(vector< EquipSlot *>::iterator iter = equipmentSlots_.begin(); iter != equipmentSlots_.end(); ++iter)
  {
   out<<"EQUIPMENT_SLOT "<<(*iter)->type->getTag()<<" "<<(*iter)->capacity <<endl;;
  }
  for(vector <SkillElement >::iterator iter = intristicSkills_.begin();
  iter != intristicSkills_.end(); ++iter)
  {
    out<<"SKILL ";
    (*iter).save(out);
  }

    for(int i =0; i <gameFacade->movementModes.size(); ++i)
  {
    if(capacity_[i]) out << "CAPACITY "<<(gameFacade->movementModes[i])->getTag()<<" " << capacity_[i] <<endl;
  }
 stats.save(out,"",0);
  skillBonuses_.save(out);
  movementBonuses_.save(out);
}


void RaceRule::cleanAttributes()
{
      Rule::cleanAttributes();
      skillBonuses_.clean();
}


/*
* Returns number of items that may be equiped in this category.
*/
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



/*
*  Intristic ability of unit to study skill
*  Level limitations also taken into considerations
*/
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



bool RaceRule::mayMove(UnitEntity * unit)
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
  Rule::extractKnowledge(recipient);
  for(vector <SkillElement >::iterator iter = intristicSkills_.begin();
  iter != intristicSkills_.end(); ++iter)
  {
    if((*iter).getSkill())
    {
      if(recipient->addSkillKnowledge((*iter).getSkill(),(*iter).getLevel()))
	(*iter).getSkill()->extractKnowledge(recipient, (*iter).getLevel());
    }
  }
  
  skillBonuses_.extractKnowledge(recipient, 1);
}



void RaceRule::printDescription(ReportPrinter & out)
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
      }
      
      
    }
    out<<". ";
  }
  out << getDescription()<<".";
  
  printTypeSpecificDescription(out);
  
  if(hiringCost_)
    out << " Base recruit price: $" << hiringCost_<< " .";
  
  if(stats.getUpkeep())
    out << " Upkeep $" << stats.getUpkeep()<<" per figure.";
  
  if(stats.getControlPoints())
  {
    out << " Required  " << stats.getControlPoints()<<" control per each ";
    if( controlPointsFraction_ > 1)
      out << controlPointsFraction_<<" figures.";
    else
      out << controlPointsFraction_<<" figure.";
  }
  
  if(!stats.empty())
  {
    out << " Stats: "<< stats;
  }
  
  if(!intristicSkills_.empty())
  {
    out << " Skills: ";
    for (SkillIterator iter = intristicSkills_.begin(); iter != intristicSkills_.end(); iter++)
    {
      if( iter != intristicSkills_.begin())
      {
	out << ", ";
      }
      (*iter).reportExperience(out);
    }
    out <<". ";
  }
  
  skillBonuses_.report(out);
}



vector <AbstractData *> RaceRule::aPrintTypeSpecificDescription()
{
    vector <AbstractData *> out;
    return out;
}



vector <AbstractData *> RaceRule::aPrint()
{
    vector <AbstractData *> out;
    out.push_back(new StringData(getDescription()));
    if(weight_)
    {
        out.push_back(new StringData(" weight "));
        out.push_back(new IntegerData(weight_));

        bool isFirst = true;
        for(int i =0 ; i < gameFacade->movementModes.size(); ++i)
        {
            if(capacity_[i])
            {
                if(isFirst)
                {
                    out.push_back(new StringData(", capacity: "));
                    isFirst = false;
                }
                else
                {
                    out.push_back(new StringData(", "));
                }
                out.push_back(new IntegerData(capacity_[i]));
                out.push_back(new StringData("/"));
                out.push_back(new StringData((gameFacade->movementModes[i])->getName()));
            }
        }
        out.push_back(new StringData(". "));
    }

    vector <AbstractData *> v = aPrintTypeSpecificDescription();
    for(vector<AbstractData *>::iterator iter= v.begin(); iter != v.end(); iter++)
      {
              out.push_back(*iter);
      }

    if(hiringCost_)
    {
        out.push_back(new StringData(" Base recruit price: $"));
        out.push_back(new IntegerData(hiringCost_));
        out.push_back(new StringData("."));
    }
    if(stats.getUpkeep())
    {
        out.push_back(new StringData(" Upkeep $"));
        out.push_back(new IntegerData(stats.getUpkeep()));
        out.push_back(new StringData(" per figure."));
    }
    if(stats.getControlPoints())
    {
        out.push_back(new StringData(" Required "));
        out.push_back(new IntegerData(stats.getControlPoints()));
        out.push_back(new StringData("  control per each "));
      if( controlPointsFraction_ > 1)
        {
          out.push_back(new IntegerData(controlPointsFraction_));
          out.push_back(new StringData(" figures"));
      }
      else
      {
          out.push_back(new IntegerData(controlPointsFraction_));
          out.push_back(new StringData(" figure"));
      }
      out.push_back(new StringData(". "));
    }

    if(!stats.empty())
    {
        bool isFirst = true;
        out.push_back(new StringData("  Stats: "));
        vector<AbstractArray> statPrint = stats.aPrint();
        for(vector <AbstractArray>::iterator iter = statPrint.begin();
        iter != statPrint.end(); ++iter)
        {
            if(!isFirst)
            {
                out.push_back(new StringData(", "));
            }
            else
            {
                isFirst = false;
            }
            for(vector <AbstractData *>::iterator iter2 = (*iter).begin();
            iter2 != (*iter).end(); ++iter2)
            {
                out.push_back(*iter2);
            }
        }
        out.push_back(new StringData(". "));
    }



    if(!intristicSkills_.empty())
    {
        out.push_back(new StringData(" Skills: "));
      for (SkillIterator iter = intristicSkills_.begin(); iter != intristicSkills_.end(); iter++)
      {
        if( iter != intristicSkills_.begin())
        {
            out.push_back(new StringData(", "));
        }
        vector <AbstractData *> v = (*iter).aPrintSkill();
        for(vector<AbstractData *>::iterator iter= v.begin(); iter != v.end(); iter++)
          {
                  out.push_back(*iter);
          }

      }
out.push_back(new StringData(". "));
//out.push_back(new StringData(""));

    }


    vector<AbstractArray> v1 = skillBonuses_.aPrintReport();


    for(vector<AbstractArray>::iterator iter= v1.begin(); iter != v1.end(); iter++)
      {

        for(AbstractArray::iterator iter2= (*iter).begin(); iter2 != (*iter).end(); iter2++)
          {
              out.push_back(*iter2);
          }

      }
    return out;
}

int RaceRule::getProductionBonusValue(SkillRule * skill)
{
  return skillBonuses_.getProductionBonus(skill);
}



int RaceRule::hasIntisticSkill(SkillRule * skill)
{
    for (SkillIterator iter = intristicSkills_.begin(); iter != intristicSkills_.end(); iter++)
    {

      if((*iter).getSkill() == skill)
      {
          return (*iter).getExpPoints();
      }
    }
    return 0;
}



int RaceRule::getLearningBonus(SkillRule * skill)
{
  return skillBonuses_.getLearningBonus(skill);
}



int RaceRule::getStudyBonus(SkillRule * skill)
{
  return skillBonuses_.getStudyBonus(skill);
}
