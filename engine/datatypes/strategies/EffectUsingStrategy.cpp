/***************************************************************************
                          EffectUsingStrategy.cpp 
                             -------------------
    begin                : Sun Jan 18 2004
    begin                : Thu Feb 20 2003
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "EffectUsingStrategy.h"
#include "SkillUseElement.h"
#include "ToolUseElement.h"
#include "ItemElement.h"
#include "EnchantmentRule.h"
#include "UnitEntity.h"
#include "RulesCollection.h"
extern RulesCollection    <EnchantmentRule>     enchantments;

EffectUsingStrategy::EffectUsingStrategy ( const EffectUsingStrategy * prototype )
        : BasicProductionStrategy(prototype)
{
  enchantment_ = 0;
}



GameData * EffectUsingStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<EffectUsingStrategy> (this);
}



STATUS
EffectUsingStrategy::initialize        ( Parser *parser )
{
  if (parser->matchKeyword ("PRODUCES") )
    {
      enchantment_ = enchantments[parser->getWord()];
      productionDays_ =  parser->getInteger();
      if( (enchantment_ == 0) ||  (productionDays_ == 0) )
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




/*
 *
 */



USING_RESULT EffectUsingStrategy::unitUse(UnitEntity * unit, SkillRule * skill, int &useCounter)
{
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


  else // The   production cycle is finished.
  {

 /* <---- Create effect here */
    return enchantment_->carryOut(unit);
  }


}

