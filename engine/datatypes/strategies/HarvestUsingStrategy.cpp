/***************************************************************************
                          HarvestUsingStrategy.cpp 
                             -------------------
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "HarvestUsingStrategy.h"
#include "ItemElement.h"
#include "ItemRule.h"
#include "LocationEntity.h"
#include "UnitEntity.h"
#include "ResourceCompetitiveRequest.h"
#include "RulesCollection.h"
#include "ToolUseElement.h"
#include "BinaryPattern.h"
extern Reporter * notAvailableReporter;


extern RulesCollection    <ItemRule>     items;

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
//      harvest_ =  ItemElement::readElement(parser);
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



Rational HarvestUsingStrategy::getDailyProduction()
{
  return  Rational(harvest_ , days_); 
}



bool HarvestUsingStrategy::use(UnitEntity * unit, Order * OrderId)
{
  Rational request = getDailyProduction() * unit->getFiguresNumber();
  if( request.isEmpty())
  {
//    unit->addReport(new BinaryPattern(notAvailableReporter,resourceConsumed_ ,unit->getLocation()));
    return false;
  }
  vector <ToolUseElement *>::iterator iter;
  for(iter = tools_.begin(); iter != tools_.end();iter++)
  {
    request = request + request * (*iter)->getBonus() * unit->hasEquiped( (*iter)->getItemType())/100;
    }
  unit->getLocation()->addDailyConflictRequest(new ResourceCompetitiveRequest(unit,OrderId,getHarvestedResource(), request));
  return true;
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



USING_RESULT HarvestUsingStrategy::mayUse(UnitEntity * unit, SkillRule * skill)
{
  Rational request = getDailyProduction();
  if( !harvest_)
  {
//    unit->addReport(new BinaryPattern(notAvailableReporter,resourceConsumed_ ,unit->getLocation()));
    return NO_RESOURCES;
  }
  return  USING_OK;
}

void HarvestUsingStrategy::reportUse(USING_RESULT result, UnitEntity * unit, Order * OrderId)
{
    unit->addReport(new BinaryPattern(notAvailableReporter,resourceConsumed_ ,unit->getLocation()));

}
