/***************************************************************************
                          CombatUsingStrategy.h
                             -------------------
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef COMBAT_USING_H
#define COMBAT_USING_H

#include "BasicUsingStrategy.h"
class TokenEntity;

/**
  *@author Alex Dribin
  */

class CombatUsingStrategy : public BasicUsingStrategy  {
public:
      CombatUsingStrategy ( const string & keyword, GameData * parent): BasicUsingStrategy(keyword, parent){}
      CombatUsingStrategy ( const CombatUsingStrategy * prototype ): BasicUsingStrategy(prototype){}
		 ~CombatUsingStrategy(){};
      GameData * createInstanceOfSelf();
      STATUS initialize        ( Parser *parser );
        virtual void save(ostream &out);
      USING_RESULT use(TokenEntity * tokenEntity, SkillRule * skill, int & useCounter,OrderLine * order);
//			virtual USING_RESULT unitMayUse(UnitEntity * unit, SkillRule * skill);
//      virtual void reportUse(USING_RESULT result, TokenEntity * unit);
//     virtual void extractKnowledge (Entity * recipient, int parameter = 0);
//      virtual bool checkResourcesAvailability(TokenEntity * unit);
//      virtual bool consumeResources(TokenEntity * unit, int numCycles);
//              void printSkillDescription(ostream & out);


	protected:
//      vector <ItemElement *> resources_;
};
extern CombatUsingStrategy      sampleCombatUsing;

#endif
