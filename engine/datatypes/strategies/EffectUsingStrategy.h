/***************************************************************************
                          EffectUsingStrategy.h 
                             -------------------
    begin                : Sun Jan 18 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef EFFECT_USING_H
#define EFFECT_USING_H

#include "BasicProductionStrategy.h"

/** Use of skill creates Special Effects
  *@author Alex Dribin
  */
class Entity;
class UnitEntity;
class EnchantmentRule;

class EffectUsingStrategy : public BasicProductionStrategy  {
public: 
      EffectUsingStrategy ( const string & keyword, GameData * parent): BasicProductionStrategy(keyword, parent){}
      EffectUsingStrategy ( const EffectUsingStrategy * prototype ); 
		 ~EffectUsingStrategy(){};
      GameData * createInstanceOfSelf();
      STATUS initialize        ( Parser *parser );
      USING_RESULT unitUse(UnitEntity * tokenEntity, SkillRule *, int &useCounter);
private:
      EnchantmentRule * enchantment_;
};
extern EffectUsingStrategy      sampleEffectUsing;

#endif
