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

class PhysicalEntity;
class SkillElement;
class SkillRule;
class Order;
class Entity;
class UnitEntity;

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
       virtual USING_RESULT unitUse(UnitEntity * unit, SkillRule *, int &useCounter);
       virtual USING_RESULT unitMayUse(UnitEntity * unit, SkillRule * skill);
       virtual USING_RESULT use(PhysicalEntity * tokenEntity , SkillRule *, int &useCounter);
       virtual USING_RESULT mayUse(PhysicalEntity * tokenEntity , SkillRule * skill);
       virtual void reportUse(USING_RESULT result, PhysicalEntity * tokenEntity);
       virtual void addUsingExperience(PhysicalEntity * tokenEntity, SkillElement & skill);// similar to addLearningExperience
       virtual void addRecursiveUsingExperience(PhysicalEntity * tokenEntity, SkillElement & skill);
       virtual int  calculateUsingExperience(PhysicalEntity * tokenEntity, SkillRule * skill);
       virtual  void extractKnowledge (Entity * recipient, int parameter = 0);
       virtual int getUseDuration();
       virtual void printSkillDescription(ostream & out){}

protected:
  static int expBase_;
};
extern BasicUsingStrategy         sampleUsing;

#endif
