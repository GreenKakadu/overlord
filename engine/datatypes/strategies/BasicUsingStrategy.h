/***************************************************************************
                          BasicUsingStrategy.h 
                             -------------------
    begin                : Wed Feb 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef USE_H
#define USE_H

#include "OverlordTypes.h"
#include "Strategy.h"

class UnitEntity;
class SkillElement;
class SkillRule;
class Order;
class Entity;

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
       virtual bool use(UnitEntity * unit, Order * OrderId);
       virtual void reportUse(USING_RESULT result, UnitEntity * unit, Order * OrderId);
       virtual USING_RESULT mayUse(UnitEntity * unit, SkillRule * skill);
       virtual void addUsingExperience(UnitEntity * unit, SkillElement & skill);// similar to addLearningExperience
       virtual void addRecursiveUsingExperience(UnitEntity * unit, SkillElement & skill);
       virtual int  calculateUsingExperience(UnitEntity * unit, SkillRule * skill);
       virtual  void extractKnowledge (Entity * recipient, int parameter = 0);

protected:
  static int expBase_;
};

#endif
