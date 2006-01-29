/***************************************************************************
                          EnchantmentRule.cpp 
                             -------------------
    begin                : Sun Jan 18 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "EnchantmentRule.h"
#include "EntitiesCollection.h"
#include "EffectEntity.h"
#include "PrototypeManager.h"

extern EntitiesCollection <EffectEntity>   effects;
EnchantmentRule    sampleEnchantment    ("FX_EFFECT",&sampleGameData);
RulesCollection <EnchantmentRule>    enchantments(new DataStorageHandler("enchantments.rules"));

EnchantmentRule::EnchantmentRule ( const EnchantmentRule * prototype ) : Rule(prototype)
{
  effectKeyword_ = "FX_EFFECT";
  targetType_ =  0;
}


GameData * EnchantmentRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<EnchantmentRule> (this);
}


STATUS
EnchantmentRule::initialize        ( Parser *parser )
{
  GameData::initialize(parser);
     stats_.initialize(parser);
  targetType_ =  prototypeManager->findInRegistry("UNIT");

  if (parser->matchKeyword("PLURAL"))
    {
      pluralName_ = parser->getText();
      return OK;
    }

  if (parser->matchKeyword("TARGET"))
    {
      string targetName = parser->getText();
      targetType_ =  prototypeManager->findInRegistry(targetName);
			if( targetType_== 0)
				{
					cout << "Unknown target type " <<  targetName << " for enchantment " << print()<< endl;
				}

      return OK;
    }


  if (parser->matchKeyword("CAPACITY"))
    {
      if(parser->matchInteger())
				{
					int index =  parser->getInteger();
					capacity_[index]  = parser->getInteger();
				}
			else
					{
						string modeTag = parser->getWord();
						if(movementModes.isValidTag(modeTag))
							{
								capacity_[modeTag]  = parser->getInteger();
							}
					}
      return OK;
    }

		movementBonuses_.initialize(parser);
		skillBonuses_.initialize(parser);
      return OK;

}



void EnchantmentRule::printDescription(ReportPrinter & out)
{
    out << print()<< ": "<< getDescription()<<".";
}


// Not used
USING_RESULT EnchantmentRule::carryOut(Entity * entity)
{
  return     UNUSABLE;

}


// Not used
EffectEntity * EnchantmentRule::createEffect(Entity * target)
{
  EffectEntity  * newEffect   = dynamic_cast<EffectEntity *> (createByKeyword(effectKeyword_));
  if(newEffect)
  {
    if(effects.addNew(newEffect) != OK)
      {
        cout << "Failed to add new effect \n";
        return 0;
      }
     newEffect->setName(name_);
     newEffect->setTarget(target);
     cout << "New "<< print() << " appeared at "<< target<<endl;
  }
  return newEffect;
}




int EnchantmentRule::getProductionBonusValue(SkillRule * skill)
{
  return skillBonuses_.getProductionBonus(skill);
}



int EnchantmentRule::getLearningBonus(SkillRule * skill)
{
  return skillBonuses_.getLearningBonus(skill);
}



int EnchantmentRule::getStudyBonus(SkillRule * skill)
{
  return skillBonuses_.getStudyBonus(skill);
}
