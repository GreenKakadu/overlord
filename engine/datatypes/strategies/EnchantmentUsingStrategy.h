/***************************************************************************
                          EnchantmentUsingStrategy.h 
                             -------------------
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef ENCHANTMENT_USING_H
#define ENCHANTMENT_USING_H

#include <vector>
#include "BasicProductionStrategy.h"
class ToolUseElement;
class EnchantmentElement;

/** Enchantment skill grants Enchantment (produces special effect) 
  * i.e. EnchantmentElement, consisting from  EnchantmentRule * productType_
  * and counter of enchantment duration
  *@author Alex Dribin
  */
class EnchantmentRule;
class Entity;
class UnitEntity;

class EnchantmentUsingStrategy : public BasicProductionStrategy  {
public: 
      EnchantmentUsingStrategy ( const string & keyword, GameData * parent): BasicProductionStrategy(keyword, parent){}
      EnchantmentUsingStrategy ( const EnchantmentUsingStrategy * prototype );
		 ~EnchantmentUsingStrategy(){};
      GameData * createInstanceOfSelf();
      STATUS initialize        ( Parser *parser );
      USING_RESULT unitUse(UnitEntity * unit, SkillRule *, int &useCounter,OrderLine * order);
      USING_RESULT unitMayUse(UnitEntity * unit, SkillRule * skill);
      void reportUse(USING_RESULT result, TokenEntity * tokenEntity);
      void extractKnowledge (Entity * recipient, int parameter = 0);
      void printSkillDescription(ostream & out);
			BasicUsingStrategy * cloneSelf();
private:
      EnchantmentRule * productType_;
};
extern EnchantmentUsingStrategy       sampleEnchantmentUsing;

#endif
