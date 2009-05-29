/***************************************************************************
                          HarvestUsingStrategy.h 
                             -------------------
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#ifndef HARVEST_USING_H
#define HARVEST_USING_H

#include <vector>
#include "BasicUsingStrategy.h"
#include "RationalNumber.h"

/**Harvesting skill
  *@author Alex Dribin
  */
class ItemRule;
class RationalNumber;
class ToolUseElement;
class Order;
class UnitEntity;

class HarvestUsingStrategy : public BasicUsingStrategy  {
public: 
      HarvestUsingStrategy ( const string & keyword, GameData * parent): BasicUsingStrategy(keyword, parent){}
      HarvestUsingStrategy ( const HarvestUsingStrategy * prototype ); 
		 ~HarvestUsingStrategy(){};
      GameData * createInstanceOfSelf();
      STATUS initialize        ( Parser *parser );
      USING_RESULT unitUse(UnitEntity * unit, SkillRule *, int &useCounter,OrderLine * order);
      USING_RESULT unitMayUse(UnitEntity * unit, SkillRule * skill);
      void reportUse(USING_RESULT result, TokenEntity * tokenEntity);
      RationalNumber getDailyProduction(); // should return scaled int
      inline ItemRule * getHarvestedResource(){return resourceHarvested_;}
      inline ItemRule * getConsumedResource(){return resourceConsumed_;}
      void extractKnowledge (Entity * recipient, int parameter = 0);
      void printSkillDescription(ostream & out);
			BasicUsingStrategy * cloneSelf();
    protected:
     ItemRule * resourceHarvested_;
     ItemRule * resourceConsumed_;
      int  harvest_;     
      int days_;
      vector <ToolUseElement *> tools_;
};
extern HarvestUsingStrategy     sampleHarvestUsing;

#endif
