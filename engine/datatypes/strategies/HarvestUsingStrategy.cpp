/***************************************************************************
                          HarvestUsingStrategy.cpp 
                             -------------------
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "HarvestUsingStrategy.h"
#include "ItemRule.h"
#include "LocationEntity.h"
#include "UnitEntity.h"
#include "ResourceCompetitiveRequest.h"
#include "RulesCollection.h"
#include "ToolUseElement.h"
#include "BinaryPattern.h"
#include "ConstructionEntity.h"

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



USING_RESULT HarvestUsingStrategy::unitUse(UnitEntity * unit, SkillRule * skill, int & useCounter)
{
  Rational request = getDailyProduction() * unit->getFiguresNumber();
  if( request.isEmpty())
  {
//    unit->addReport(new BinaryPattern(notAvailableReporter,resourceConsumed_ ,unit->getLocation()));
    return CANNOT_USE;
  }
  vector <ToolUseElement *>::iterator iter;
  for(iter = tools_.begin(); iter != tools_.end();iter++)
  {
    request = request + request * (*iter)->getBonus() * unit->hasEquiped( (*iter)->getItemType())/100;
  }

  ConstructionEntity * containingConstruction = unit->getContainingConstruction();
  if(containingConstruction)
  {    
    containingConstruction->setProductionBonus(getHarvestedResource(), skill,unit->getSkillLevel(skill));
  }
      
  unit->getLocation()->addDailyConflictRequest(new ResourceCompetitiveRequest(unit,unit->getCurrentOrder(),getHarvestedResource(), request));
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
  Rational request = getDailyProduction();
  if( !harvest_)
  {
//    unit->addReport(new BinaryPattern(notAvailableReporter,resourceConsumed_ ,unit->getLocation()));
    return NO_RESOURCES;
  }
  return  USING_OK;
}



void HarvestUsingStrategy::reportUse(USING_RESULT result, PhysicalEntity * tokenEntity)
{
    tokenEntity->addReport(new BinaryPattern(notAvailableReporter,resourceConsumed_ ,tokenEntity->getLocation()));

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
  out << resourceHarvested_->printName();

  out<<" in "<< days_ <<" days.";
}
