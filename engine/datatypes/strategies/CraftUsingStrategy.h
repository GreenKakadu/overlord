/***************************************************************************
                          CraftUsingStrategy.h 
                             -------------------
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
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
      CraftUsingStrategy ( const string & keyword, GameData * parent): BasicProductionStrategy(keyword, parent){}
      CraftUsingStrategy ( const CraftUsingStrategy * prototype );
		 ~CraftUsingStrategy(){};
      GameData * createInstanceOfSelf();
      STATUS initialize        ( Parser *parser );
      USING_RESULT unitUse(UnitEntity * unit, SkillRule *, int &useCounter);
      USING_RESULT unitMayUse(UnitEntity * unit, SkillRule * skill);
      void reportUse(USING_RESULT result, PhysicalEntity * tokenEntity);
      void extractKnowledge (Entity * recipient, int parameter = 0);
      void printSkillDescription(ostream & out);
private:
      ItemRule * productType_;
      int productionDays_;
};
extern CraftUsingStrategy       sampleCraftUsing;

#endif
