/***************************************************************************
                          EnchantmentRule.h
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

#ifndef ENCHANTMENT_RULE_H
#define ENCHANTMENT_RULE_H

#include "Rule.h"
#include "EntityStatistics.h"
#include "MovementMode.h"
/**Enchantment types
  *@author Alex Dribin
  */
class EffectEntity;

class EnchantmentRule : public Rule  {
public: 
      EnchantmentRule ( const string & keyword, GameData * parent) : Rule(keyword, parent){}
      EnchantmentRule ( const EnchantmentRule * prototype );// : Rule(prototype)	{_optimalPopulation =0;landWalk_ =0;}
      ~EnchantmentRule (){}
      virtual STATUS     initialize      ( Parser *parser);
      inline string getPluralName() const{return pluralName_;}
      GameData * createInstanceOfSelf();
      void printDescription(ReportPrinter & out);
      USING_RESULT carryOut(Entity * entity);
      EffectEntity * createEffect(Entity * target) ;
      inline  EntityStatistics * getStats()  {return &stats_;}
      inline int getCapacity(int modeIndex)  {return capacity_[modeIndex];}
      inline GameData *  getTargetType() {return targetType_;}

	protected:
	    string pluralName_;
		  MovementMode<int> capacity_;
      EntityStatistics stats_;
      string effectKeyword_;
      GameData * targetType_; // pointer to prototype
};
#include "RulesCollection.h"
extern RulesCollection <EnchantmentRule>   enchantments;

#endif
