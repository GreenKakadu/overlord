/***************************************************************************
                          ConstructionRule.cpp 
                             -------------------
    begin                : Sun Aug 31 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
 

#include "ConstructionRule.h"
#include "ConstructionWorksElement.h"
#include "SkillElement.h"
#include "BonusElement.h"
#include "BasicCondition.h"
#include "SkillCondition.h"
#include "ConstructionEntity.h"
#include "UnitEntity.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
#include "EntitiesCollection.h"
#include "ConstructionWorksVariety.h"
#include "RulesCollection.h"
#include "TitleRule.h"
#include "TitleElement.h"

extern EntitiesCollection <ConstructionEntity>   buildingsAndShips;
//ConstructionRule   sampleConstructionRule =   ConstructionRule("CONSTRUCTION", &sampleGameData);
extern RulesCollection <TitleRule>   titles;

ConstructionRule::ConstructionRule( const ConstructionRule * prototype ) : Rule(prototype)
{
 landUse_ = 0;
 entityKeyword_  = "BUILDING";
 mobile_ = false;
 weight_ = 10000; // default weight
 maxStaff_ = 0;
 generateTitle_ = 0;
 skill_ = 0;
 buildCondition_ = 0;
 staffCondition_ = 0;
}

ConstructionRule::~ConstructionRule(){}
GameData * ConstructionRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<ConstructionRule> (this);
}


STATUS
ConstructionRule::initialize        ( Parser *parser )
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

  if (parser->matchKeyword("RESOURCE"))
	  {
        ConstructionWorksElement * newResource = ConstructionWorksElement::readElement(parser);
        if(newResource)
          resources_.push_back(newResource);
	    return OK;
	  }

  if (parser->matchKeyword("SKILL"))
	  {
      skill_ = SkillElement::readElement(parser);
	    return OK;
	  }
  if (parser->matchKeyword("TITLE"))
	  {
      generateTitle_ = titles[parser->getWord()];
	    return OK;
	  }
    
   
  if (parser->matchKeyword("LEARNING_BONUS"))
	{
    BonusElement * newBonus = BonusElement::readElement(parser);
    if(newBonus)
          skillBonuses_.push_back(newBonus);
	  return OK;
	}
  if (parser->matchKeyword("USE_BONUS"))
	{
    BonusElement * newBonus = BonusElement::readElement(parser);
    if(newBonus)
          useBonuses_.push_back(newBonus);
	  return OK;
	}
  if (parser->matchKeyword("STAFF_MEN"))
    {
      maxStaff_ = parser->getInteger();
//      if( maxStaff_ == 0)
//          maxStaff_ = 1;
	  return OK;
	  }

  if (parser->matchKeyword("STAFF_CONDITION"))
    {

      staffCondition_ = dynamic_cast<SkillCondition *> (createByKeyword(parser->getWord()));
      if(staffCondition_)
      {
        staffCondition_->initialize(parser) ;
        staffCondition_->setSubject(this);
        }
      return OK;
    }

  if (parser->matchKeyword("STAFF_CAPACITY"))
    {
      if(parser->matchInteger())
				{
					int index =  parser->getInteger();
					capacity_[index]  = parser->getInteger();
          if(capacity_[index] )
                  mobile_ = true;         
				}
			else
					{
						string modeTag = parser->getWord();
						if(movementModes.isValidTag(modeTag))
							{
								capacity_[modeTag]  = parser->getInteger();
                if(capacity_[modeTag] )
                  mobile_ = true;
							}
					}
      return OK;
    }
  
  if (parser->matchKeyword("LAND"))
	{
	  landUse_ = parser->getInteger();
	  return OK;
	}
  if (parser->matchKeyword("WEIGHT"))
	{
	  weight_ = parser->getInteger();
	  return OK;
	}
  if (parser->matchKeyword("BUILD_CONDITION"))
    {

      buildCondition_ = dynamic_cast<BasicCondition *> (createByKeyword(parser->getWord()));
      if(buildCondition_)
      {
        buildCondition_->initialize(parser) ;
        buildCondition_->setSubject(this);
        }
      return OK;
    }

     stats_.initialize(parser);

      return OK;
}



ConstructionEntity * ConstructionRule::startConstruction(UnitEntity * unit)
{
  ConstructionEntity  * newBuilding   = dynamic_cast<ConstructionEntity *> (createByKeyword(entityKeyword_));
  if(newBuilding)
  {
    if(buildingsAndShips.addNew(newBuilding) != OK)
      {
        cout << "Failed to add new building \n";
        return 0;
      }
     newBuilding->setConstructionType(this);
     newBuilding->setName(name_);
     newBuilding->initializeBuildingWorks(resources_);
     unit->getLocation()->addConstruction(newBuilding);
     newBuilding->setFaction(unit->getFaction());
	   unit->getFaction()->addConstruction(newBuilding);
//     cout << "New "<< print() << " was built in "<< unit->getLocation()->print()<<endl;
  }
  return newBuilding;
}

/*
 * Generate title assosiated with construction
 */

void ConstructionRule::finishConstruction(LocationEntity * location)
{
      if(generateTitle_)
       {
         location->addTitle(new TitleElement(generateTitle_,location,0));
       }
}



int ConstructionRule::getResourceRequirement(ConstructionWorksVariety * resource)
{
  ConstructionWorksIterator  iter;
  for (iter= resources_.begin(); iter != resources_.end();++iter)
  {
    if( (*iter)->getWorkType() == resource)
      return ((*iter)->getWorkAmount()).roundUp();
  }
  return 0;
}



int ConstructionRule::getProductionBonusValue(SkillRule * skill,int level)
{
  int bonus = 0;
  int currentBonus;
  for( vector <BonusElement *>::iterator iter = useBonuses_.begin();
       iter != useBonuses_.end(); ++iter)
       {
          if(skill->isDescendFrom((*iter)->getSkill(),1))
          {
            currentBonus = (*iter)->getBonusPoints();
            if   (bonus < currentBonus)
                  bonus = currentBonus;
          }
       }   
return bonus;

}



int ConstructionRule::getBonus(SkillRule * skill)
{
  for(BonusIterator iter = skillBonuses_.begin(); iter != skillBonuses_.end(); iter++)
    {
        if ((*iter)->getSkill() == skill)
          return (*iter)->getBonusPoints();
    }
  return 0;
}



bool ConstructionRule::mayMove()
{
  return mobile_;
}



void    ConstructionRule::extractKnowledge (Entity * recipient, int parameter)
{
  if(skill_)
  {
    if(recipient->addSkillKnowledge(skill_->getSkill(),skill_->getLevel()))
      skill_->getSkill()->extractKnowledge(recipient,skill_->getLevel());
  }


  for(vector <BonusElement *>::iterator iter = skillBonuses_.begin();
                                 iter != skillBonuses_.end(); ++iter)
    {
      if((*iter)->getSkill())
      {
        if(recipient->addSkillKnowledge((*iter)->getSkill(),1))
          (*iter)->getSkill()->extractKnowledge(recipient, 1);

      }
  }

  for(vector <BonusElement *>::iterator iter = useBonuses_.begin();
                                 iter != useBonuses_.end(); ++iter)
    {
      if((*iter)->getSkill())
      {
        if(recipient->addSkillKnowledge((*iter)->getSkill(),1))
          (*iter)->getSkill()->extractKnowledge(recipient, 1);

      }
  }


  if(buildCondition_)
    buildCondition_->extractKnowledge(recipient);
  if(staffCondition_)
    staffCondition_->extractKnowledge(recipient);

   if(generateTitle_)
    {
         recipient->addKnowledge(generateTitle_);
         generateTitle_->extractKnowledge(recipient);
    }

}



void ConstructionRule::printDescription(ReportPrinter & out)
{
    out << print()<< ": "<< getDescription()<<".";
    if(!resources_.empty())
    {
      out << " Requires ";
      for (ConstructionWorksIterator iter = resources_.begin();
          iter != resources_.end();++iter )
          {
            if(iter != resources_.begin())
              out << ", ";
            out<< *(*iter);
          }
    }
    out<< ". ";
    if(generateTitle_)
      {
        out << "Generates "<< generateTitle_->print()
                                    << " title. ";
      }
   
   if(!stats_.empty())
   {
     out << "Adds "<< stats_ << " to units inside.";
   }
}
