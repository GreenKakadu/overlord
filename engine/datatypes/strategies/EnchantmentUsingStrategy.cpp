/***************************************************************************
                          EnchantmentUsingStrategy.cpp
                             -------------------
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "EnchantmentUsingStrategy.h"
#include "EnchantmentElement.h"
#include "SkillUseElement.h"
#include "EnchantmentRule.h"
#include "ItemRule.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "RulesCollection.h"
#include "ToolUseElement.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
extern ReportPattern * notEnoughResourcesReporter;
extern ReportPattern * privateEnchanterReporter;
extern ReportPattern * privateEnchantmentReporter;
//extern ReportPattern * publicEnchantmentReporter;
extern GameData  targetTypeSelf;
EnchantmentUsingStrategy  sampleEnchantmentUsing  ("USING_ENCHANT",      &sampleUsing);

//extern RulesCollection    <EnchantmentRule>     enchantments;

EnchantmentUsingStrategy::EnchantmentUsingStrategy ( const EnchantmentUsingStrategy * prototype ): BasicProductionStrategy(prototype)
{
  productNumber_ = 1;
  productType_ =0;
}



GameData * EnchantmentUsingStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<EnchantmentUsingStrategy> (this);
}



STATUS
EnchantmentUsingStrategy::initialize        ( Parser *parser )
{
  if (parser->matchKeyword ("GRANTS") )
    {
      productType_ = enchantments[parser->getWord()];
      productionDays_ =  parser->getInteger();
      if( (productType_ == 0) ||  (productionDays_ == 0) )
      {
        cout << "Error while reading GRANTS \n";
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



USING_RESULT EnchantmentUsingStrategy::unitUse(UnitEntity * unit, SkillRule * skill, int & repetitionCounter)
{
   USING_RESULT result;
// Production modifiers:
 int effectiveProductionRate = getEffectiveProductionRate(unit,skill).getValue();

  SkillUseElement * dailyUse = new SkillUseElement(skill,effectiveProductionRate,productionDays_);

  // cycleCounter is number of cycles that may be finished on this day
    int cycleCounter  = unit->addCumullativeSkillUse(dailyUse,repetitionCounter);
  if(cycleCounter < repetitionCounter) // In the middle of the production cycle
  {
     return USING_IN_PROGRESS;
  }

// Enchantment production is different from Item production.
// Items are producted at each production cycle and enchantment
// produced all at once when  the number of cycles defined by
// repetition counter.

  else // The production cycle is finished but it should be repeated
       // as defined by repetition counter
  {
    int resourcesAvailable = checkResourcesAvailability(unit);

    if( resourcesAvailable < cycleCounter)
        cycleCounter = resourcesAvailable;
    int effectiveProduction = cycleCounter * productNumber_;
    if( (repetitionCounter != 0) && (effectiveProduction  >= repetitionCounter) ) // production will be finished now
      {
        effectiveProduction = repetitionCounter;
        cycleCounter = (effectiveProduction + productNumber_ -1)/ productNumber_;
        if(cycleCounter >1)
          consumeResources(unit,cycleCounter-1);
         result = USING_COMPLETED;
        repetitionCounter = 0;
      unit->getCurrentOrder()->setCompletionFlag(true);

   // Determine target
   TokenEntity * target;
   if(productType_->getTargetType() == &targetTypeSelf)
    target = unit;
   else
    target = dynamic_cast <TokenEntity *>(unit->getTarget());
   if(!target) // definition of EnchantmentRule is wrong
      {
       cout << "Wrong definition of EnchantmentRule. Target is not a token"<<endl;
       return WRONG_TARGET;
      }
    target->addEnchantment(new EnchantmentElement(productType_, effectiveProduction));  //
      if(!unit->isSilent())
        {
//QQQ
          unit->addReport(
          new TertiaryMessage(privateEnchanterReporter, unit, target,
          new EnchantmentElement(productType_,effectiveProduction))
                  );
        }
      if(!target->isSilent())
        {
//QQQ
          target->addReport(
          new TertiaryMessage(privateEnchantmentReporter, target, unit,
          new EnchantmentElement(productType_,effectiveProduction))
                  );
        }
//      target->getLocation()->addReport(
//      new BinaryMessage(publicEnchantmentReporter, unit, productType_
//        /*, 0, observation condition*/);

      }
    else
    {
      consumeResources(unit,cycleCounter-1);
//      if(dailyUse->getDaysUsed() > 0)
//        unit->addSkillUse(dailyUse);
//      repetitionCounter = repetitionCounter - effectiveProduction;
         result = USING_IN_PROGRESS;
    }

    return result;
  }
}



USING_RESULT EnchantmentUsingStrategy::unitMayUse(UnitEntity * unit, SkillRule * skill)
{
// check target type
    USING_RESULT result = checkTarget(unit,productType_->getTargetType());
    if( result != USING_OK)
      return  result;
    else
      return BasicProductionStrategy::unitMayUse(unit,skill);
}



void    EnchantmentUsingStrategy::extractKnowledge (Entity * recipient, int parameter)
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



void EnchantmentUsingStrategy::reportUse(USING_RESULT result, TokenEntity * tokenEntity)
{
  for(vector <ItemElement *>::iterator iter = resources_.begin(); iter != resources_.end(); ++iter)
    {
      if (tokenEntity->hasItem((*iter)->getItemType()) < (*iter)->getItemNumber())
        tokenEntity->addReport(new BinaryMessage(notEnoughResourcesReporter, (*iter)->getItemType(),productType_));
    }
}



void EnchantmentUsingStrategy::printSkillDescription(ostream & out)
{
  BasicProductionStrategy::printSkillDescription(out);

 if(!productType_)
    return;
 else

 out << " Use grants: " << productNumber_<<" ";
 if(productNumber_ > 1)
  out << productType_->getPluralName()<< " " << productType_->printTag();
 else
  out << productType_->print();

  out<<" in "<< productionDays_ <<" days.";
}



