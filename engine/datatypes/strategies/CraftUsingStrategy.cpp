/***************************************************************************
                          CraftUsingStrategy.cpp 
                             -------------------
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "CraftUsingStrategy.h"
#include "ItemElement.h"
#include "SkillUseElement.h"
#include "ItemRule.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "RulesCollection.h"
#include "ToolUseElement.h"
#include "BinaryPattern.h"
extern Reporter * notEnoughResourcesReporter;
extern Reporter * productionReporter;

extern RulesCollection    <ItemRule>     items;

CraftUsingStrategy::CraftUsingStrategy ( const CraftUsingStrategy * prototype ): BasicProductionStrategy(prototype)
{
  productNumber_ = 1;
  productType_ =0;
}



GameData * CraftUsingStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<CraftUsingStrategy> (this);
}



STATUS
CraftUsingStrategy::initialize        ( Parser *parser )
{
  if (parser->matchKeyword ("PRODUCES") )
    {
      productType_ = items[parser->getWord()];
      productionDays_ =  parser->getInteger();
      if( (productType_ == 0) ||  (productionDays_ == 0) )
      {
        cout << "Error while reading PRODUCES \n";
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



USING_RESULT CraftUsingStrategy::unitUse(UnitEntity * unit, SkillRule * skill, int & useRestrictionCounter)
{
   USING_RESULT result;
// Production modifiers:

// Tools accelerate production
  vector <ToolUseElement *>::iterator iter;
  int effectiveProductionRate = unit->getFiguresNumber();
  for(iter = tools_.begin(); iter != tools_.end();iter++)
  {
    effectiveProductionRate =  effectiveProductionRate + (*iter)->getBonus() * unit->hasEquiped( (*iter)->getItemType())/100;
  }


  SkillUseElement * dailyUse = new SkillUseElement(skill,effectiveProductionRate,productionDays_);

  int cycleCounter  = unit->addSkillUse(dailyUse);
  if(cycleCounter == 0) // In the middle of the production cycle
  {
     return USING_IN_PROGRESS;
  }

  else // The old  production cycle is finished. Do we want to start new?
  {
    int resourcesAvailable = checkResourcesAvailability(unit);
    
    if( resourcesAvailable < cycleCounter)
        cycleCounter = resourcesAvailable;
    int effectiveProduction = cycleCounter * productNumber_;
    if( (useRestrictionCounter != 0) && (effectiveProduction  >= useRestrictionCounter) ) // limited number of new cycles
      {  
        effectiveProduction = useRestrictionCounter;
        cycleCounter = (effectiveProduction + productNumber_ -1)/ productNumber_;
        if(cycleCounter >1)
          consumeResources(unit,cycleCounter-1);
         result = USING_COMPLETED; 
        useRestrictionCounter = 0;
      unit->getCurrentOrder()->setCompletionFlag(true);
      }
        
    else 
    {
      consumeResources(unit,cycleCounter-1);
      if(dailyUse->getDaysUsed() > 0)
        unit->addSkillUse(dailyUse);
      useRestrictionCounter = useRestrictionCounter - effectiveProduction;
         result = USING_IN_PROGRESS;
    }

    unit->addToInventory(productType_, effectiveProduction);
      if(!unit->isSilent())
        {
//QQQ
          unit->addReport(
          new BinaryPattern(productionReporter, unit,
          new ItemElement(productType_,effectiveProduction))
                  );
      unit->getLocation()->addReport(
      new BinaryPattern(productionReporter, unit,
      new ItemElement(productType_,effectiveProduction))
        /*, 0, observation condition*/);
    }
    return result;
  }
}



//USING_RESULT CraftUsingStrategy::unitMayUse(UnitEntity * unit, SkillRule * skill)
//{
///** We may be in the middle of production. Then we anyway may continue.*/
///** Otherwise we'll need resources */
//
// if (!unit->isCurrentlyUsingSkill(skill)) // beginning of production cycle
//  {
//    if(checkResourcesAvailability(unit) == 0)
//      {
//        return NO_RESOURCES;
//      }
//     else
//     {
//        consumeResources(unit,1);
//        return  USING_OK;
//     }
//  }
//    return  USING_OK;
//
//
//
//
////  vector <ToolUseElement *>::iterator iter;
////  RationalNumber dailyProduction(productNumber_ * unit->getFiguresNumber(), productionDays_);
////  for(iter = tools_.begin(); iter != tools_.end();iter++)
////  {
////    dailyProduction = dailyProduction + dailyProduction * (*iter)->getBonus() * unit->hasEquiped( (*iter)->getItemType())/100;
////  }
//
////  RationalNumber  currentAmount = unit->getItemAmount(productType_);
//// Check for new production cycle + check for production counter
////  if( currentAmount.isInteger() )
////  {
////      if(resourceNumber_ != unit->takeFromInventory(resourceType_, resourceNumber_))
////      {
////        return NO_RESOURCES;
////       }
////   }
////  else if( (currentAmount + dailyProduction).getValue() > currentAmount.getValue() )
////    {
////      if(resourceNumber_ != unit->takeFromInventory(resourceType_, resourceNumber_))
////      {
////        // no resources but old production in progress
////        dailyProduction =  currentAmount + 1 - currentAmount.getValue();
////        unit->addReport(new BinaryPattern(notEnoughResourcesReporter,resourceType_ ,productType_));
////        }
////    }
////    return  USING_OK;
//}



void    CraftUsingStrategy::extractKnowledge (Entity * recipient, int parameter)
{
  if(productType_)
  {
    if(recipient->addKnowledge(productType_))
      productType_->extractKnowledge(recipient);
  }

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



void CraftUsingStrategy::reportUse(USING_RESULT result, TokenEntity * tokenEntity)
{
  for(vector <ItemElement *>::iterator iter = resources_.begin(); iter != resources_.end(); ++iter)
    {
      if (tokenEntity->hasItem((*iter)->getItemType()) < (*iter)->getItemNumber())
        tokenEntity->addReport(new BinaryPattern(notEnoughResourcesReporter, (*iter)->getItemType(),productType_));
    }
}

void CraftUsingStrategy::printSkillDescription(ostream & out)
{
  BasicProductionStrategy::printSkillDescription(out);

 if(!productType_)
    return;
 else

 out << " Use produces: " << productNumber_<<" ";
 if(productNumber_ > 1)
  out << productType_->getPluralName()<< " " << productType_->printTag();
 else
  out << productType_->print();

  out<<" in "<< productionDays_ <<" days.";
}
