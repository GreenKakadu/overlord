/***************************************************************************
                          BasicUsingStrategy.h 
                             -------------------
    begin                : Wed Feb 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef USE_H
#define USE_H

#include "OverlordTypes.h"
#include "Strategy.h"

class TokenEntity;
class SkillElement;
class SkillRule;
class Entity;
class UnitEntity;
class OrderLine;

/**Use of skill
  *@author Alex Dribin
  */

class BasicUsingStrategy : public Strategy  {
public: 
      BasicUsingStrategy ( const string & keyword, GameData * parent): Strategy(keyword, parent){}
      BasicUsingStrategy ( const BasicUsingStrategy * prototype );
		 ~BasicUsingStrategy(){};
      virtual STATUS     initialize      ( Parser *parser);
      GameData * createInstanceOfSelf();
      virtual USING_RESULT unitUse(UnitEntity * unit, SkillRule *, int &useCounter,OrderLine * order);
      virtual USING_RESULT unitMayUse(UnitEntity * unit, SkillRule * skill);
      virtual USING_RESULT use(TokenEntity * tokenEntity , SkillRule *, int &useCounter,OrderLine * order);
      virtual USING_RESULT mayUse(TokenEntity * tokenEntity, SkillRule * skill);
      virtual void reportUse(USING_RESULT result, TokenEntity * tokenEntity);
      virtual void addUsingExperience(TokenEntity * tokenEntity, SkillElement & skill);// similar to addLearningExperience
      virtual void addRecursiveUsingExperience(TokenEntity * tokenEntity, SkillRule * skill, int experience);
      virtual int  calculateUsingExperience(TokenEntity * tokenEntity, SkillRule * skill);
      virtual  void extractKnowledge (Entity * recipient, int parameter = 0);
      virtual int getUseDuration();
      virtual void printSkillDescription(ostream & out){}
      virtual inline int getProductionDays(){return 0;}
			virtual int calculateProductionBonus(TokenEntity * tokenEntity, SkillRule * skill);
			virtual BasicUsingStrategy * cloneSelf();
protected:
  static int expBase_;
};
extern BasicUsingStrategy         sampleUsing;

#endif
