/***************************************************************************
                          EnchantmentUsingStrategy.cpp
                             -------------------
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
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
     BasicProductionStrategy::initialize(parser);

	if (parser->matchKeyword ("GRANTS") )
    {
      productType_ = gameFacade->enchantments[parser->getWord()];
      productionDays_ =  parser->getInteger();
      if( (productType_ == 0) ||  (productionDays_ == 0) )
      {
        cout << "Error while reading GRANTS \n";
        return IO_ERROR;
      }
      return OK;
    }

      return OK;
}

void EnchantmentUsingStrategy::save(ostream &out)
{
    BasicProductionStrategy::save(out);
    if(productType_) out<<"GRANTS"<<" "<<productType_->getTag()
            <<" "<<productionDays_ << endl;
}


USING_RESULT EnchantmentUsingStrategy::unitUse(UnitEntity * unit, SkillRule * skill, 
					int & repetitionCounter,OrderLine * order)
{
   if(unit->isTraced())
   {
   //EnchantmentElement * enchant =  EnchantmentElement::createElement(productType_, effectiveProduction);
        cout << "======unitUse======= repetitionCounter=" <<repetitionCounter<<endl;
   }
   USING_RESULT result;
// Production modifiers:
 int effectiveProduction =0;
 bool newCycle =false;
 result = produce(unit, skill,repetitionCounter, effectiveProduction, order, newCycle);

     if (effectiveProduction == 0)// Nothing was produced
    {
        return result;
    }
// print
 // add enchants


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
   EnchantmentElement * enchant =  new EnchantmentElement(productType_, effectiveProduction);
   if(unit->isTraced())
   {
   //EnchantmentElement * enchant =  EnchantmentElement::createElement(productType_, effectiveProduction);
        cout << "=============" << enchant->print()<<endl;
   }
   // Operator new is not re-defined for EnchantmentElement,
   // so actually object of type Element2<EnchantRule,int> is created;
   // it's ok but it can't be printed.
   EnchantmentElement  enchant2 = EnchantmentElement(productType_, effectiveProduction);
    target->addEnchantment(enchant);  //

      if(!unit->isSilent())
        {
//QQQ
          unit->addReport(
          new TertiaryMessage(privateEnchanterReporter, unit, target,
         new StringData(enchant2.print()))
                        );
        }
      if(!target->isSilent())
        {
//QQQ
          target->addReport(
          new TertiaryMessage(privateEnchantmentReporter, target, unit,
          new StringData(enchant2.print()))
                            );
        }
//      target->getLocation()->addReport(
//      new BinaryMessage(publicEnchantmentReporter, unit, productType_
//        /*, 0, observation condition*/);

    return result;

}

USING_RESULT EnchantmentUsingStrategy::produce(UnitEntity * unit, SkillRule * skill, int & repetitionCounter, int & effectiveProduction, OrderLine * order, bool & newCycle)
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
    int manaAvailable = checkManaAvailability(unit);

    if( resourcesAvailable < cycleCounter)
        cycleCounter = resourcesAvailable;
    if( manaAvailable < cycleCounter)
        cycleCounter = manaAvailable;

    effectiveProduction = cycleCounter * productNumber_;
    if( (repetitionCounter != 0) && (effectiveProduction  >= repetitionCounter) ) // production will be finished now
      {
        effectiveProduction = repetitionCounter;
        cycleCounter = (effectiveProduction + productNumber_ -1)/ productNumber_;
        if(cycleCounter >1)
				{
          consumeResources(unit,cycleCounter-1);
          consumeMana(unit,cycleCounter-1);
				}
         result = USING_COMPLETED;
        repetitionCounter = 0;
        order->setCompletionFlag(true);
      }
    else // Old cycle was finished and new started
    {
                 if (unit->isTraced())
      {
          cout <<"== TRACING " <<unit->print()<<" Starting new cycle "
            << " cycleCounter= " << cycleCounter<<endl;
      }
      consumeResources(unit,cycleCounter-1);
      consumeMana(unit,cycleCounter-1);
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



BasicUsingStrategy * EnchantmentUsingStrategy::cloneSelf()
{
 EnchantmentUsingStrategy * copyOfSelf = new EnchantmentUsingStrategy(keyword_,parent_);
 *copyOfSelf = *this;
 return copyOfSelf;
}
