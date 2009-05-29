/***************************************************************************
                          CombatEnchantmentRule.cpp
                             -------------------
    begin                : Sun Jan 18 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "CombatEnchantmentRule.h"
#include "EntitiesCollection.h"
#include "EffectEntity.h"
#include "PrototypeManager.h"
#include "BattleInstance.h"

extern EntitiesCollection <EffectEntity>   effects;
CombatEnchantmentRule    sampleEnchantment    ("COMBAT_EFFECT",&sampleGameData);
RulesCollection <CombatEnchantmentRule>    enchantments(new DataStorageHandler("enchantments.rules"));

CombatEnchantmentRule::CombatEnchantmentRule ( const CombatEnchantmentRule * prototype ) : Rule(prototype)
{
}


GameData * CombatEnchantmentRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<CombatEnchantmentRule> (this);
}


STATUS
CombatEnchantmentRule::initialize        ( Parser *parser )
{


     modifyingStats.initialize(parser);

      return OK;

}



void CombatEnchantmentRule::printDescription(ReportPrinter & out)
{
    out << print()<< ": "<< getDescription()<<".";
}


void CombatEnchantmentRule::addOnInstance(BattleInstance * target)
{
}



void CombatEnchantmentRule::removeFromInstance(BattleInstance * target)
{
}



