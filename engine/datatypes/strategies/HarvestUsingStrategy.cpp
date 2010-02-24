/***************************************************************************
                          HarvestUsingStrategy.cpp
                             -------------------
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "HarvestUsingStrategy.h"
#include "ItemRule.h"
#include "LocationEntity.h"
#include "FactionEntity.h"
#include "UnitEntity.h"
#include "ResourceCompetitiveRequest.h"
#include "RulesCollection.h"
#include "ToolUseElement.h"
#include "BinaryMessage.h"
#include "ConstructionEntity.h"
#include "GameConfig.h"


extern ReportPattern * notAvailableReporter;
HarvestUsingStrategy      sampleHarvestUsing      ("USING_HARVEST",      &sampleUsing);


ReportPattern *	 harvestingNotPermittedReporter  = new ReportPattern("harvestingNotPermittedReporter");



HarvestUsingStrategy::HarvestUsingStrategy ( const HarvestUsingStrategy * prototype )
        : BasicUsingStrategy(prototype)
{
     resourceHarvested_ = 0;
     resourceConsumed_ = 0;
     harvest_ = 0;
     days_ = 1;

}




GameData * HarvestUsingStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<HarvestUsingStrategy> (this);
}



STATUS
HarvestUsingStrategy::initialize        ( Parser *parser )
{
  if (parser->matchKeyword ("HARVEST") )
    {
      resourceConsumed_ = items[parser->getWord()];
      harvest_  = parser->getInteger();

      return OK;
    }
  if (parser->matchKeyword ("PRODUCES") )
    {
      resourceHarvested_ = items[parser->getWord()];
      days_ =  parser->getInteger();
      return OK;
    }
      return OK;

  if (parser->matchKeyword ("TOOL") )
  {
      ToolUseElement * tool = ToolUseElement::readElement (parser);
      if( tool)
        tools_.push_back(tool);
      return OK;
    }

}



RationalNumber HarvestUsingStrategy::getDailyProduction()
{
  return  RationalNumber(harvest_ , days_);
}



USING_RESULT HarvestUsingStrategy::unitUse(UnitEntity * unit, SkillRule * skill, 
					int & useCounter,OrderLine * order)
{
 // First check that there are at least some resource to harvest
  RationalNumber availableResourceAtLocation = unit->getLocation()->getAvailableResource(resourceConsumed_);
 if(availableResourceAtLocation == 0)
 {
    unit->addReport(new BinaryMessage(notAvailableReporter,resourceConsumed_ ,unit->getLocation()));
    return CANNOT_USE;
 }
	RationalNumber request = getDailyProduction() * unit->getFiguresNumber();
	// / gameConfig.daysInMonth;
  if( request.isEmpty())
  {
  }
	int bonus = calculateProductionBonus(unit,skill);


	vector <ToolUseElement *>::iterator iter;
  for(iter = tools_.begin(); iter != tools_.end();iter++)
  {
    bonus += (((*iter)->getBonus() * unit->hasEquiped( (*iter)->getItemType())) /
		unit->getFiguresNumber()) /100;
  }
   request = (request * (100 + bonus))/100;



  ConstructionEntity * containingConstruction = unit->getContainingConstruction();
  if(containingConstruction)
  {
    containingConstruction->setProductionBonus(getHarvestedResource(), skill,unit->getSkillLevel(skill));
  }

  unit->getLocation()->addDailyConflictRequest(new ResourceCompetitiveRequest(unit, 
					order, getHarvestedResource(), request));
  return USING_COMPLETED;
}



void    HarvestUsingStrategy::extractKnowledge (Entity * recipient, int parameter)
{
  if(resourceConsumed_)
  {
    if(recipient->addKnowledge(resourceConsumed_))
      resourceConsumed_->extractKnowledge(recipient);
  }

  if(resourceHarvested_)
  {
  if(recipient->addKnowledge(resourceHarvested_))
    resourceHarvested_->extractKnowledge(recipient);
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



USING_RESULT HarvestUsingStrategy::unitMayUse(UnitEntity * unit, SkillRule * skill)
{
  LocationEntity * location = unit->getLocation();
	if(location == 0)
	{
	 return USING_RESULT_END;
	}
  RationalNumber request = getDailyProduction();
  if( !harvest_)
  {
     if(!location->getResource(resourceConsumed_))
    unit->addReport(new BinaryMessage(notAvailableReporter,resourceConsumed_ ,unit->getLocation()));
    return NO_RESOURCES;
  }
  FactionEntity * owner = location->getRealOwner();

    if(owner)
    {
      if(!owner->stanceAtLeast(unit,
				location->getOwnershipPolicy().getHarvestPermission(resourceConsumed_)))
      {
        unit->addReport(new BinaryMessage (harvestingNotPermittedReporter,
				 resourceConsumed_, location),0,0);
        return USING_NOT_ALLOWED;
      }
    }
  return  USING_OK;
}



void HarvestUsingStrategy::reportUse(USING_RESULT result, TokenEntity * tokenEntity)
{
    tokenEntity->addReport(new BinaryMessage(notAvailableReporter,resourceConsumed_ ,tokenEntity->getLocation()));

}



void HarvestUsingStrategy::printSkillDescription(ostream & out)
{

 if(!resourceHarvested_)
    return;
 else

 out << " Use produces: " << harvest_<<" ";
 if(harvest_ > 1)
  out << resourceHarvested_->getPluralName()<< " " << resourceHarvested_->printTag();
 else
  out << resourceHarvested_->print();

  out<<" in "<< days_ <<" days.";
}


BasicUsingStrategy * HarvestUsingStrategy::cloneSelf()
{
 HarvestUsingStrategy * copyOfSelf = new HarvestUsingStrategy(keyword_,parent_);
 *copyOfSelf = *this;
 return copyOfSelf;
}
