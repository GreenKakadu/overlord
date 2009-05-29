/***************************************************************************
                          CombatEnchantmentRule.h
                             -------------------
    begin                : 25 Apr 2006
    copyright            : (C) 2006 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef COMBAT_ENCHANTMENT_RULE_H
#define COMBAT_ENCHANTMENT_RULE_H

#include "Rule.h"
#include "EntityStatistics.h"
#include "MovementMode.h"
#include "SkillBonusComboAttribute.h"
/**Enchantment types
  *@author Alex Dribin
  */
class EffectEntity;
class BattleInstance;

class CombatEnchantmentRule : public Rule  {
public: 
      CombatEnchantmentRule ( const string & keyword, GameData * parent) : Rule(keyword, parent){}
      CombatEnchantmentRule ( const CombatEnchantmentRule * prototype );
      ~CombatEnchantmentRule (){}
      virtual STATUS     initialize      ( Parser *parser);
      GameData * createInstanceOfSelf();
      void printDescription(ReportPrinter & out);

      inline  EntityStatistics * getStats()  {return &stats_;}
      inline GameData *  getTargetType() {return targetType_;}

//			void addOnInstance(BattleInstance * target);
//			void removeFromInstance(BattleInstance * target)

      EntityStatistics * getModifyingStats()  {return &modifyingStats;}



	protected:
      EntityStatistics modifyingStats;


};

extern RulesCollection <CombatEnchantmentRule>   enchantments;
extern  CombatEnchantmentRule      sampleEnchantment;

#endif
