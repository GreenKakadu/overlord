/***************************************************************************
                          TitleRule.cpp
                             -------------------
    begin                : Fri Nov  9 19:24:42 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include <algorithm>
#include "IntegerData.h"
#include "TitleRule.h"
#include "SkillCondition.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "SkillLevelElement.h"
#include "BonusElement.h"
#include "QuintenaryMessage.h"
TitleRule     sampleTitle     ("TITLE",    &sampleGameData);
MerchantPrinceTitleRule     sampleMerchantPrinceTitleRule =     MerchantPrinceTitleRule("PRINCE", &sampleTitle);
OverlordTitleRule  sampleOverlordTitleRule =  OverlordTitleRule ("OVERLORD", &sampleTitle);
RulesCollection <TitleRule>     titles(new DataStorageHandler("titles.rules"));
extern ReportPattern * failedContestTitleReporter;
extern ReportPattern * successContestTitleReporter;

TitleRule::TitleRule(const TitleRule * prototype) : Rule(prototype)
{
    cost_    = 0;
    control_ = 0;
    type_    = 0;
    range_   = 0;
    condition_ = 0;
}



GameData* TitleRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<TitleRule> (this);
}



STATUS
TitleRule::initialize        ( Parser *parser)
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
  if (parser->matchKeyword ("SKILL") )
    {
      condition_ = new SkillCondition(sampleSkillCondition);
      condition_->initialize(parser);
      return OK;
    }
  if ( parser->matchKeyword ("COST") )
    {
      setCost( parser->getInteger() );
      return OK;
    }
  if ( parser->matchKeyword ("CONTROL") )
    {
      setControl( parser->getInteger() );
      return OK;
    }
  if ( parser->matchKeyword ("TYPE") )
    {
      setType( parser->getInteger() );
      return OK;
    }
  if ( parser->matchKeyword ("RANGE") )
    {
      setRange( parser->getInteger() );
      return OK;
    }

		skillBonuses_.initialize(parser);

  return OK;
}



void TitleRule::printDescription(ReportPrinter & out)
{
   out << print()<< ": "<< getDescription()<<". ";

   if(range_)   out << "Range "<< range_ <<" days of walking. ";
   if(cost_)    out << "Costs $"<< cost_ <<". ";

/*   if(learningLevelBonus_)
        out << "Allows owner to learn " << learningLevelBonus_->getLevel()
            <<" additional levels of all skills derived from "
            << learningLevelBonus_->getSkill() << " without a teacher. ";

    if(studyBonus_)
        out << "Allows owner to learn all skills derived from "
            << studyBonus_->getSkill() << " "<< studyBonus_->getBonusPoints()
            << "% faster.";*/
}


bool TitleRule::contest(UnitEntity * titleHolder, UnitEntity * contender,
                                                LocationEntity * location)
{
  SkillRule * skill = condition_->getSkill();
  int skillExp1 = titleHolder->getSkillPoints(skill) /100;
  int skillExp2 = contender->getSkillPoints(skill) /100;
  bool contestResult;


 if(titleHolder->getLocation() != location)
 {
   contestResult = (skillExp2 *100 / skillExp1 >= 90);
  }
 else
 {
   contestResult = (skillExp2 *100 / skillExp1 > 110);
  }

  if(contestResult)
  {
  contender->addReport(new QuintenaryMessage(successContestTitleReporter,contender, titleHolder,
        new IntegerData(skillExp2), skill, new IntegerData(skillExp1) ) );
  titleHolder->addReport(new QuintenaryMessage(failedContestTitleReporter, titleHolder,contender,
                      new IntegerData(skillExp1), skill, new IntegerData(skillExp2)  ) );
  }
  else
  {
  contender->addReport(new QuintenaryMessage(failedContestTitleReporter,contender, titleHolder,
        new IntegerData(skillExp2), skill, new IntegerData(skillExp1) ) );
  titleHolder->addReport(new QuintenaryMessage(successContestTitleReporter, titleHolder,contender,
                      new IntegerData(skillExp1), skill, new IntegerData(skillExp2)  ) );
  }

  return  contestResult;
}



AbstractData * TitleRule::getContestCriteria(UnitEntity * unit)
{
//QQQ
   if(condition_ == 0)
    return (new AbstractData());

  return  unit->getSkillElement(condition_->getSkill());
}



void TitleRule::activateClaimingEffects(UnitEntity * titleHolder,
                                        LocationEntity * location)
{
 if(range_)
  markTerritoryOwned(location,titleHolder,range_);
}



void TitleRule::desactivateClaimingEffects(UnitEntity * titleHolder,
                                        LocationEntity * location)
{
 if(range_)
  markTerritoryOwned(location,0,range_);
}



int TitleRule::markTerritoryOwned(LocationEntity * start, UnitEntity * titleHolder, int distance )
{
  int counter = 0;
  LocationEntity * current = start;
  vector <LocationEntity *> examinedLocations;
  int currentDistance  = 0;
  vector <LocationEntity *> openList;
  FactionEntity * owner;
  if(titleHolder)
    owner = titleHolder->getFaction();
  else
     owner = 0;

  current->markClosed();
  current->setDistance(0);
  examinedLocations.push_back(current);

  while(current->getDistance() <= distance)
  {
    current->setLegalOwner(owner,start);
    counter++;
    current->examineNeighboringLocations(currentDistance, openList,
                                examinedLocations);

//    std::nth_element(openList.begin(),openList.begin(),openList.end(),betterDistance);
    std::sort(openList.begin(),openList.end(),betterDistance);

   if(openList.empty())
   {
            break;
   }
   current = *(openList.begin());
   openList.erase(openList.begin());

   currentDistance  = current->getDistance();
   current->markClosed();
  } // end of while loop

 openList.clear();

 for(vector <LocationEntity *>::iterator iter = examinedLocations.begin();
                  iter != examinedLocations.end(); ++iter)
 {
   (*iter)->clearClosed();
   (*iter)->setDistance(0);
 }

 examinedLocations.clear();

 return counter;

}



void    TitleRule::extractKnowledge (Entity * recipient, int parameter)
{
  if(condition_)
    condition_->extractKnowledge(recipient);

	skillBonuses_.extractKnowledge(recipient,1);
}



int TitleRule::getProductionBonusValue(SkillRule * skill)
{
  return skillBonuses_.getProductionBonus(skill);
}



int TitleRule::getLearningBonus(SkillRule * skill)
{
  return skillBonuses_.getLearningBonus(skill);
}



int TitleRule::getStudyBonus(SkillRule * skill)
{
  return skillBonuses_.getStudyBonus(skill);
}
//============================================================
//============================================================
//============================================================
//============================================================
//============================================================
//============================================================
GameData* OverlordTitleRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<OverlordTitleRule> (this);
}



bool OverlordTitleRule::contest(UnitEntity * titleHolder, UnitEntity * contender,
                                                LocationEntity * location)
{
   // Battle
   // report contest
   return false;
}



void OverlordTitleRule::activateClaimingEffects(UnitEntity * titleHolder,
                                        LocationEntity * location)
{
 if(range_)
  markTerritoryOwned(location,titleHolder,range_);

}



void OverlordTitleRule::desactivateClaimingEffects(UnitEntity * titleHolder,
                                        LocationEntity * location)
{
 if(range_)
  markTerritoryOwned(location,titleHolder,range_);
 // set NPC1 hostile

}



GameData* MerchantPrinceTitleRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<MerchantPrinceTitleRule> (this);
}



void MerchantPrinceTitleRule::activateClaimingEffects(UnitEntity * titleHolder,
                                        LocationEntity * location)
{
  location->setMarketPrince(titleHolder);
}



void MerchantPrinceTitleRule::desactivateClaimingEffects(UnitEntity * titleHolder,
                                        LocationEntity * location)
{
  location->setMarketPrince(0);
}
