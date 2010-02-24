/***************************************************************************
                          CraftUsingStrategy.h 
                             -------------------
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef CRAFT_USING_H
#define CRAFT_USING_H

#include <vector>
#include "BasicProductionStrategy.h"
class ToolUseElement;
class ItemElement;

/**Producting skills
  *@author Alex Dribin
  */
class ItemRule;
class Entity;
class UnitEntity;

class CraftUsingStrategy : public BasicProductionStrategy  {
public: 
      CraftUsingStrategy ( const string & keyword, GameData * parent): BasicProductionStrategy(keyword, parent){cout <<"===> "<<keyword<<" created"<<endl;}
      CraftUsingStrategy ( const CraftUsingStrategy * prototype );
		 ~CraftUsingStrategy(){};
      GameData * createInstanceOfSelf();
      STATUS initialize        ( Parser *parser );
      USING_RESULT unitUse(UnitEntity * unit, SkillRule *, int &useCounter,OrderLine * order);
//      USING_RESULT unitMayUse(UnitEntity * unit, SkillRule * skill);
			void reportUse(USING_RESULT result, TokenEntity * tokenEntity);
      void extractKnowledge (Entity * recipient, int parameter = 0);
      void printSkillDescription(ostream & out);
			BasicUsingStrategy * cloneSelf();
protected:
      ItemRule * productType_;
private:
};
extern CraftUsingStrategy       sampleCraftUsing;

#endif
