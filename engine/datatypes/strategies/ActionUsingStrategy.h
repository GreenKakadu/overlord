/***************************************************************************
                          ActionUsingStrategy.h 
                             -------------------
    begin                : Sun Jan 18 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef ACTION_USING_H
#define ACTION_USING_H

#include "BasicProductionStrategy.h"

/** Use of skill invokes Special Action
  *@author Alex Dribin
  */
class Entity;
class UnitEntity;
class ActionRule;

class ActionUsingStrategy : public BasicProductionStrategy  {
public: 
      ActionUsingStrategy ( const string & keyword, GameData * parent): BasicProductionStrategy(keyword, parent){}
      ActionUsingStrategy ( const ActionUsingStrategy * prototype );
		 ~ActionUsingStrategy(){};
      GameData * createInstanceOfSelf();
      virtual void save(ostream &out);
      STATUS initialize        ( Parser *parser );
      USING_RESULT unitUse(UnitEntity * tokenEntity, SkillRule *, int &useCounter,OrderLine * order);
      USING_RESULT unitMayUse(UnitEntity * unit, SkillRule * skill,OrderLine * order);
      void reportUse(USING_RESULT result, TokenEntity * tokenEntity);
      void extractKnowledge (Entity * recipient, int parameter = 0);
      void printSkillDescription(ostream & out);
			BasicUsingStrategy * cloneSelf();
private:
      ActionRule * productType_;
      AbstractData * specificProduct_;
};
extern ActionUsingStrategy      sampleActionUsing;

#endif
