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
#include "BasicUsingStrategy.h"
class ToolUseElement;

/**Producting skills
  *@author Alex Dribin
  */
class ItemRule;
class Entity;

class CraftUsingStrategy : public BasicUsingStrategy  {
public: 
      CraftUsingStrategy ( const string & keyword, GameData * parent): BasicUsingStrategy(keyword, parent){}
      CraftUsingStrategy ( const CraftUsingStrategy * prototype );
		 ~CraftUsingStrategy(){};
      GameData * createInstanceOfSelf();
      STATUS initialize        ( Parser *parser );
      bool use(UnitEntity * unit, Order * OrderId);
      void reportUse(USING_RESULT result, UnitEntity * unit, Order * OrderId);
      USING_RESULT mayUse(UnitEntity * unit, SkillRule * skill);
      void extractKnowledge (Entity * recipient, int parameter = 0);
private:      
      ItemRule * productType_;
      int productionDays_;
      ItemRule * resourceType_ ;
      int resourceNumber_ ;
      int productNumber_ ;
      vector <ToolUseElement *> tools_;
};

#endif
