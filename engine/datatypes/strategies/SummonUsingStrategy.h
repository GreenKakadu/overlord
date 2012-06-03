/***************************************************************************
                          SummonUsingStrategy.h 
                             -------------------
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef SUMMON_USING_H
#define SUMMON_USING_H

#include "BasicProductionStrategy.h"

/**Summoning
  *@author Alex Dribin
  */
class RaceRule;

class SummonUsingStrategy : public BasicProductionStrategy  {
public: 
      SummonUsingStrategy ( const string & keyword, GameData * parent): BasicProductionStrategy(keyword, parent){}
      SummonUsingStrategy ( const SummonUsingStrategy * prototype ): BasicProductionStrategy(prototype){}
		 ~SummonUsingStrategy(){};
      GameData * createInstanceOfSelf();
     STATUS initialize        ( Parser *parser );
       virtual void save(ostream &out);
     USING_RESULT unitUse(UnitEntity * unit, SkillRule *, int &useCounter,OrderLine * order);
      bool isValidSummoningTarget(UnitEntity * unit, UnitEntity * newUnit);
//      USING_RESULT unitMayUse(UnitEntity * unit, SkillRule * skill);
      UnitEntity *  findSummoningTarget(UnitEntity * unit);
//      USING_RESULT  checkTarget(UnitEntity * unit, GameData * targetType);
      UnitEntity *  useDefaultTarget(UnitEntity * unit);
			void reportUse(USING_RESULT result, TokenEntity * tokenEntity);
      void extractKnowledge (Entity * recipient, int parameter = 0);
      void printSkillDescription(ostream & out);
			BasicUsingStrategy * cloneSelf();
private:
      RaceRule * summonedRace_;
};
extern SummonUsingStrategy      sampleSummonUsing;

#endif
