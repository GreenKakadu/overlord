/***************************************************************************
                          ConstructionRule.cpp 
                             -------------------
    begin                : Sun Aug 31 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
 

#include "GameFacade.h"
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
#include "BuildUsingStrategy.h"
#include "PrototypeManager.h"
#include "BinaryMessage.h"
#include "StringData.h"


ConstructionRule   sampleConstructionRule("CONSTRUCTION", &sampleGameData);
//RulesCollection <ConstructionRule>      constructions(new DataStorageHandler("constructions.rules"),&sampleConstructionRule);
extern ReportPattern * newBuidingStartedReporter;
extern ReportPattern * buidingFinishedReporter;

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
 buildingSkill_ =0;
 isBattle_ = false;
 buildingParadigm_  = 0;
 economyBonus_ =0;
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
  if (parser->matchKeyword("BUILDING_SKILL"))
	  {
            buildingSkill_ = SkillElement::readElement(parser);
	    return OK;
	  }
  if (parser->matchKeyword("TITLE"))
	  {
      generateTitle_ = gameFacade->titles[parser->getWord()];
	    return OK;
	  }
  if (parser->matchKeyword("BATTLE"))
	  {
      isBattle_ = true;;
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

      staffCondition_ = BasicCondition::readCondition(parser);
      if(staffCondition_)
      {
        staffCondition_->initialize(parser) ;
        }
      return OK;
    }

    if (parser->matchKeyword("STAFF_CAPACITY"))
    {
        if (parser->matchInteger())
        {
            int index = parser->getInteger();
            capacity_[index] = parser->getInteger();
            if (capacity_[index])
                mobile_ = true;
        } else
        {
            string modeTag = parser->getWord();
            if (gameFacade->movementModes.isValidTag(modeTag))
            {
                capacity_[modeTag] = parser->getInteger();
                if (capacity_[modeTag])
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

    if (parser->matchKeyword("ECONOMY"))
	{
            economyBonus_= parser->getInteger();
            return OK;
	}


  if (parser->matchKeyword("WEIGHT"))
	{
	  weight_ = parser->getInteger();
	  return OK;
	}
  if (parser->matchKeyword ("BUILDING_PARADIGM") )
        {
          string keyword = parser->getWord();
          GameData * temp =  prototypeManager->findInRegistry(keyword);
          if(temp == 0)
          {
            cerr << "Unknown building paradigm " << keyword
                    << " for construstion " << print()<< endl;
          }
          else
          {
            buildingParadigm_ = dynamic_cast<BuildUsingStrategy *>
                    (temp ->createInstanceOfSelf ());
          }
          return OK;
        }
          
          if (parser->matchKeyword("BUILD_CONDITION"))
    {

      buildCondition_ = dynamic_cast<BasicCondition *>
              (createByKeyword(parser->getWord()));
      if(buildCondition_)
      {
        buildCondition_->initialize(parser) ;
        //buildCondition_->setSubject(this);
        }
      return OK;
    }
 	if (parser->matchKeyword("BONUS"))
	{
	  bonuses_.initialize(parser);
	  return OK;
	}
        if(buildingParadigm_)
        {   
          buildingParadigm_->initialize(parser);
        }
    stats_.initialize(parser);
    skillBonuses_.initialize(parser);
    movementBonuses_.initialize(parser);
  Rule::initialize(parser);

      return OK;
}

void ConstructionRule::save(ostream &out)
{
  Rule::save(out);
  for(vector<ConstructionWorksElement *>::iterator iter = resources_.begin();
          iter != resources_.end(); ++iter)
  {
     out<<"RESOURCE"<<" ";
     (*iter)->save(out);
  }
  if(skill_)
  {
      out<<"SKILL"<<" ";
      skill_->save(out);
  }
  if(buildingSkill_)
  {
      out<<"BUILDING_SKILL"<<" ";
       buildingSkill_->save(out);
  }
  if(generateTitle_)out<<"TITLE"<<" "<<generateTitle_->getTag() <<endl;
  if(isBattle_)out<<"BATTLE"<<" "<<endl;
  if(maxStaff_)out<<"STAFF_MEN"<<" "<<maxStaff_ <<endl;
  if(staffCondition_)
  {
      out<<"STAFF_CONDITION"<<" "<<staffCondition_->getKeyword() <<endl;
      staffCondition_->save(out);
  }
 for(int i =0; i <gameFacade->movementModes.size(); ++i)
  {
    if(capacity_[i]) out << "STAFF_CAPACITY "<<(gameFacade->movementModes[i])->getTag()
            <<" " << capacity_[i] <<endl;
  }
  if(landUse_)out<<"LAND"<<" "<<landUse_ <<endl;
  if(economyBonus_)out<<"ECONOMY"<<" "<< economyBonus_<<endl;
  if(weight_)out<<"WEIGHT"<<" "<< weight_<<endl;
  // There is also parameter PRICE in rulefiles which is not used.
  if(buildingParadigm_)
  {
      out<<"BUILDING_PARADIGM"<<" "<<buildingParadigm_->getKeyword() <<endl;
      buildingParadigm_->save(out);
  }
  if(buildCondition_)
  {
      out<<"BUILD_CONDITION"<<" "<< buildCondition_->getKeyword()<<endl;
      buildCondition_->save(out);
  }
    bonuses_.save(out,"BONUS ",0);
    stats_.save(out,"",0);
    skillBonuses_.save(out);
    movementBonuses_.save(out);
}



ConstructionEntity * ConstructionRule::startConstruction(UnitEntity * unit)
{
  ConstructionEntity  * newBuilding   = dynamic_cast<ConstructionEntity *>
          (createByKeyword(entityKeyword_));
  if(newBuilding)
  {
    if(gameFacade->buildingsAndShips.addNew(newBuilding) != OK)
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
     newBuilding->explicitInitialization();
		 // NEWTARGET
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



int ConstructionRule::getProductionBonusValue(SkillRule * skill)
{
  return skillBonuses_.getProductionBonus(skill);
}



int ConstructionRule::getLearningBonus(SkillRule * skill)
{
  return skillBonuses_.getLearningBonus(skill);
}



int ConstructionRule::getStudyBonus(SkillRule * skill)
{
  return skillBonuses_.getStudyBonus(skill);
}



bool ConstructionRule::mayMove()
{
  return mobile_;
}



void    ConstructionRule::extractKnowledge (Entity * recipient, int parameter)
{
  Rule::extractKnowledge(recipient);
  if(skill_)
  {
    if(recipient->addSkillKnowledge(skill_->getSkill(),skill_->getLevel()))
      skill_->getSkill()->extractKnowledge(recipient,skill_->getLevel());
  }

  if(buildingSkill_)
  {
    if(recipient->addSkillKnowledge(buildingSkill_->getSkill(),buildingSkill_->getLevel()))
      buildingSkill_->getSkill()->extractKnowledge(recipient,buildingSkill_->getLevel());
  }

	skillBonuses_.extractKnowledge(recipient, 1);


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
    if(buildingSkill_)
    {
      out << " Requires ";
      out << buildingSkill_->print() << " to build.";
    }
    if(generateTitle_)
      {
        out << "Generates "<< generateTitle_->print()
                                    << " title. ";
      }
   if(isBattle_)
   {
     out << "May fight in battles.";
   }

   if(!stats_.empty())
   {
     out << "Adds "<< stats_ << " to units inside.";
   }

	 skillBonuses_.report(out);
}



vector <AbstractData *> ConstructionRule::aPrint()
{
  vector <AbstractData *> out;
  out.push_back(new StringData(getDescription()));

  if(!resources_.empty())
  {
        out.push_back(new StringData(" Requires "));
    for (ConstructionWorksIterator iter = resources_.begin();
        iter != resources_.end();++iter )
        {
          if(iter != resources_.begin())
        {
            out.push_back(new StringData(", "));
          }
          out.push_back((*iter));
        }
    out.push_back(new StringData(". "));
  }

  if(buildingSkill_)
  {
      out.push_back(new StringData(" Requires "));
      vector <AbstractData *> v = SkillLevelElement(buildingSkill_->getSkill() ,buildingSkill_->getLevel()).aPrintLevel();
      for(vector<AbstractData *>::iterator iter= v.begin(); iter != v.end(); iter++)
        {
                out.push_back(*iter);
        }
       out.push_back(new StringData(" to build."));
  }
  if(generateTitle_)
    {
        out.push_back(new StringData("Generates "));
        out.push_back(new StringData(generateTitle_->getName()));
        out.push_back(new StringData(" title."));
    }

  out.push_back(new StringData(""));
 if(isBattle_)
 {
     out.push_back(new StringData("May fight in battles."));
 }

 if(!stats_.empty())
 {
     bool isFirst = true;
     out.push_back(new StringData(" Adds "));
     vector<AbstractArray> statPrint = stats_.aPrint();
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
     out.push_back(new StringData(" to units inside. "));
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




USING_RESULT ConstructionRule::startNewConstruction(UnitEntity * unit,
        ConstructionEntity *buildingOrShip, AbstractData * target)
{

// Check unit has skill to start construction
  SkillElement * skillRequired = 0;
  skillRequired = getBuildingSkill(); 
  if(skillRequired)
  {
    if(!unit->hasSkill(skillRequired))
    {
      // Print report: cn't build skill required.
      return CANNOT_USE; //?
    }
  }
  ConstructionEntity * newBuilding;
  if(buildingOrShip ==0)
  {
  newBuilding = startConstruction(unit);
  }
  else
  {
    newBuilding = buildingOrShip;
  }
  if(!newBuilding)
    return UNUSABLE;
  if(newBuilding->isCompleted())// If no resourses required
  {
    newBuilding->buildingCompleted();
    unit->addReport( new BinaryMessage
        (buidingFinishedReporter, this, new StringData(newBuilding->printTag())));
    unit->getLocation()->addReport( new BinaryMessage
        (buidingFinishedReporter, this, new StringData(newBuilding->printTag())));
  }

  else
  {
    unit->addReport( new BinaryMessage
        (newBuidingStartedReporter, this, new StringData(newBuilding->printTag())));
    unit->getLocation()->addReport( new BinaryMessage
        (newBuidingStartedReporter, this, new StringData(newBuilding->printTag())));
  }

    // If we used placeholder  now it's time to set it to real construction
  if(target)
  {
    NewEntityPlaceholder * placeholder = dynamic_cast<NewEntityPlaceholder *>(target);
    if(placeholder)
    {
      Entity * newEntity = placeholder->getRealEntity();
      if (!newEntity)
      {
        placeholder->setRealEntity(newBuilding);
              //unit->setTarget(newBuilding);
      }
      else
      {
                  //report placeholder duplication ?
      }
    }
  }
  return USING_COMPLETED;
}



USING_RESULT ConstructionRule::buildExistingConstruction(UnitEntity * unit,
        SkillRule * skill, int &useCounter,OrderLine * order)
{  
  USING_RESULT result = buildingParadigm_->unitUse(unit, skill, useCounter,order);
  return result;  
}
