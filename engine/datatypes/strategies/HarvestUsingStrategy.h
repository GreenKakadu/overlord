/***************************************************************************
                          HarvestUsingStrategy.h 
                             -------------------
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef HARVEST_USING_H
#define HARVEST_USING_H

#include <vector>
#include "BasicUsingStrategy.h"
#include "Rational.h"

/**Harvesting skill
  *@author Alex Dribin
  */
//class ItemElement;
class ItemRule;
class Rational;
class ToolUseElement;
class Order;

class HarvestUsingStrategy : public BasicUsingStrategy  {
public: 
      HarvestUsingStrategy ( const string & keyword, GameData * parent): BasicUsingStrategy(keyword, parent){}
      HarvestUsingStrategy ( const HarvestUsingStrategy * prototype ): BasicUsingStrategy(prototype){}
		 ~HarvestUsingStrategy(){};
      GameData * createInstanceOfSelf();
      STATUS initialize        ( Parser *parser );
      USING_RESULT mayUse(UnitEntity * unit, SkillRule * skill);
      bool use(UnitEntity * unit, Order * OrderId);
      void reportUse(USING_RESULT result, UnitEntity * unit, Order * OrderId);
      Rational getDailyProduction(); // should return scaled int
      inline ItemRule * getHarvestedResource(){return resourceHarvested_;}
      inline ItemRule * getConsumedResource(){return resourceConsumed_;}
      void extractKnowledge (Entity * recipient, int parameter = 0);
    protected:
     ItemRule * resourceHarvested_;
     ItemRule * resourceConsumed_;
      int  harvest_;     
//    ItemElement * harvest_;
      int days_;
      vector <ToolUseElement *> tools_;
};

#endif
