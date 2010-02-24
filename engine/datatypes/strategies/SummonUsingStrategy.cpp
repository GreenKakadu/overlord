/***************************************************************************
                          SummonUsingStrategy.cpp
                             -------------------
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "SummonUsingStrategy.h"
#include "RaceRule.h"
#include "LeaderRaceRule.h"
#include "CraftUsingStrategy.h"
#include "ItemElement.h"
#include "RaceElement.h"
#include "SkillUseElement.h"
#include "ItemRule.h"
#include "UnitEntity.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
//#include "RulesCollection.h"
#include "ToolUseElement.h"
#include "BinaryMessage.h"
#include "reporting.h"
SummonUsingStrategy       sampleSummonUsing       ("USING_SUMMON",       &sampleUsing);
extern ReportPattern * recruitMaxUnitSizeReporter;
extern ReportPattern * recruitMixedRaceReporter;
extern ReportPattern * recruitForeignUnitReporter;
extern ReportPattern * startUseReporter;

//extern ReportPattern * newSummonedReporter;
//extern ReportPattern * summonerReporter;
extern ReportPattern * notEnoughResourcesReporter;
extern ReportPattern * summoningReporter;

GameData * SummonUsingStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<SummonUsingStrategy> (this);
}



STATUS
SummonUsingStrategy::initialize        ( Parser *parser )
{

	if (parser->matchKeyword ("SUMMONS") )
    {
      summonedRace_ = races[parser->getWord()];
      productionDays_ =  parser->getInteger();
      if( (summonedRace_ == 0) ||  (productionDays_ == 0) )
      {
        cout << "Error while reading SUMMONS \n";
        return IO_ERROR;
      }
      return OK;
    }
  if (parser->matchKeyword ("CONSUME") )
    {
			if(parser->matchElement())
			  resources_.push_back(new ItemElement(parser));
      return OK;
    }

	if (parser->matchKeyword ("USE_MANA") )
    {
      mana_ =  parser->getInteger();
      return OK;
    }

  if (parser->matchKeyword ("MULTIPLE") )
    {
      productNumber_ =  parser->getInteger();
      if(productNumber_ == 0)
        productNumber_ = 1;
      return OK;
    }


  if (parser->matchKeyword ("TOOL") )
  {
      ToolUseElement * tool = ToolUseElement::readElement (parser);
      if( tool)
        tools_.push_back(tool);
      return OK;
    }
      return OK;
}







void   SummonUsingStrategy::extractKnowledge (Entity * recipient, int parameter)
{

    if(recipient->addKnowledge(summonedRace_))
      summonedRace_->extractKnowledge(recipient);


  for(vector <ItemElement *>::iterator iter = resources_.begin(); iter != resources_.end(); ++iter)
    {
      if(recipient->addKnowledge((*iter)->getItemType()))
        (*iter)->getItemType()->extractKnowledge(recipient);
    }

  for(vector <ToolUseElement *>::iterator iter = tools_.begin(); iter != tools_.end(); ++iter)
    {
      if((*iter)->getItemType())
      {
        if(recipient->addKnowledge((*iter)->getItemType()))
          (*iter)->getItemType()->extractKnowledge(recipient);
       }
     }
}



void SummonUsingStrategy::reportUse(USING_RESULT result, TokenEntity * tokenEntity)
{
  for(vector <ItemElement *>::iterator iter = resources_.begin(); iter != resources_.end(); ++iter)
    {
      if (tokenEntity->hasItem((*iter)->getItemType()) < (*iter)->getItemNumber())
        tokenEntity->addReport(new BinaryMessage(notEnoughResourcesReporter, (*iter)->getItemType(),summonedRace_));
    }
}



void SummonUsingStrategy::printSkillDescription(ostream & out)
{
  BasicProductionStrategy::printSkillDescription(out);

 out << " Use summons: " << productNumber_<<" ";
 if(productNumber_ > 1)
  out << summonedRace_->getPluralName()<< " " << summonedRace_->printTag();
 else
  out << summonedRace_->print();

  out<<" in "<< productionDays_ <<" days.";
}


//------------------------------------------------------------------
// use default target if it is possible.
// overvise create new target and save it as default
//------------------------------------------------------------------
UnitEntity *  SummonUsingStrategy::useDefaultTarget(UnitEntity * unit)
{
	UnitEntity * newUnit = dynamic_cast<UnitEntity *>(unit->getDefaultTarget());
//use default target if it is valid
	if(newUnit)
	{
	  if(isValidSummoningTarget(unit, newUnit))
	  	return newUnit;
	}
//create new default target
  newUnit =	unit->createUnit(unit->getFaction(),summonedRace_,0,unit->getLocation());
  newUnit->explicitInitialization();
	unit->setDefaultTarget(newUnit);
	return newUnit;
}



//------------------------------------------------------------------
// Use current target if it is valid
// (current target may be placeholder)
// overwise use default target
UnitEntity *  SummonUsingStrategy::findSummoningTarget(UnitEntity * unit)
{
   AbstractData * target = unit->getTarget();
	 if(target==0)
			return useDefaultTarget(unit);





	UnitEntity * newUnit = 0;

  NewEntityPlaceholder * placeholder = dynamic_cast<NewEntityPlaceholder *>(target);
  if(placeholder != 0)  // this is  placeholder.
    {
			GameData* realEntity = placeholder->getRealEntity();
      if(realEntity) // We can get real entity id from placeholder
        {            // Check that it is unit
          newUnit = dynamic_cast<UnitEntity *>(realEntity);
					if(newUnit ==0) // wrong placeholder. use default target
							return useDefaultTarget(unit);


        }
      else   // placeholder is still unresolved. Resolve it.
			{
    		TokenEntity * temp = placeholder->getNewEntity();
		 		if(temp)
		 		{
       		newUnit = dynamic_cast<UnitEntity *>(temp);
		 		}

 				if(newUnit == 0) // Something wrong with placeholder
    				newUnit   = new UnitEntity(unit);

      	if(units.addNew(newUnit) != OK)
      		{
        		cout << "Failed to add new unit \n";
        		return 0;
      		}
      	placeholder->setRealEntity(newUnit);
       	unit->getFaction()->addUnit(newUnit);
       	unit->getLocation()->addUnit(newUnit);
       	newUnit->setRace(summonedRace_,0);


/*	     	if (unit->isTraced())
        cout  <<"== TRACING " << "New unit summoned: "<<newUnit->print() <<" \n";
 */       return newUnit;
			}
    }
	else //  or some already existing Unit. check it
	{
  	newUnit = dynamic_cast<UnitEntity *>(target);
		if(newUnit ==0)
							return useDefaultTarget(unit);


	}

	if( isValidSummoningTarget(unit,newUnit))
		return newUnit;
	else
		return useDefaultTarget(unit);

}




USING_RESULT SummonUsingStrategy::unitUse(UnitEntity * unit, SkillRule * skill,
			 int & useRestrictionCounter,OrderLine * order)
{

    int effectiveProduction = 0;
    bool newCycle = false;
		USING_RESULT result =
			produce(unit, skill, useRestrictionCounter, effectiveProduction,order,newCycle);
		if (effectiveProduction == 0)
		{
			return result;
		}

   UnitEntity * newUnit = findSummoningTarget(unit);
//===== Now add new figures
   newUnit->addNewFigures(effectiveProduction);
   newUnit->recalculateStats();
	 if(!unit->isSilent())
        {
//QQQ
          unit->addReport(
          new TertiaryMessage(summoningReporter, unit,
          new RaceElement(summonedRace_,effectiveProduction), new StringData(newUnit->getTag()))
                  );
          if(newCycle)
          {
            unit->addReport(new BinaryMessage(startUseReporter,unit,skill));
          }
      unit->getLocation()->addReport(
      new TertiaryMessage(summoningReporter, unit,
      new RaceElement(summonedRace_,effectiveProduction), new StringData(newUnit->getTag()) /*newUnit*/)
        /*, 0, observation condition*/);
    }
    return result;
}


bool  SummonUsingStrategy::isValidSummoningTarget(UnitEntity * unit, UnitEntity * newUnit)
{
	 if(unit->getFaction() != newUnit ->getFaction())
   {
      unit->addReport(new UnaryMessage(recruitForeignUnitReporter,newUnit));
		  return false;
    }

	 if(summonedRace_ != newUnit ->getRace())
   {
      unit->addReport(new UnaryMessage(recruitMixedRaceReporter,newUnit));
		  return false;
    }

   if(summonedRace_->isDescendantFrom(&sampleLeaderRaceRule))
   {
      unit->addReport(new BinaryMessage(recruitMaxUnitSizeReporter,newUnit,
                      new RaceElement(summonedRace_, 1)));
		  return false;
    }
 return true;
}



BasicUsingStrategy * SummonUsingStrategy::cloneSelf()
{
 SummonUsingStrategy * copyOfSelf = new SummonUsingStrategy(keyword_,parent_);
 *copyOfSelf = *this;
 return copyOfSelf;
}
