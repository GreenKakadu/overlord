/***************************************************************************
                          ApplyActionStrategy.h
                             -------------------
    begin                : Thu Mar 03 2006
    copyright            : (C) 2006 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#ifndef APPLY_ACTION_H
#define APPLY_ACTION_H

#include <vector>
#include "BasicApplyStrategy.h"
#include "RationalNumber.h"

/**Combat Enchantment
  *@author Alex Dribin
  */
class ItemRule;
class RationalNumber;
class ToolUseElement;
class Order;
class UnitEntity;

class ApplyActionStrategy : public BasicApplyStrategy  {
public: 
      ApplyActionStrategy ( const string & keyword, GameData * parent): BasicApplyStrategy(keyword, parent){}
      ApplyActionStrategy ( const ApplyActionStrategy * prototype );
		 ~ApplyActionStrategy(){};
      GameData * createInstanceOfSelf();
      STATUS initialize        ( Parser *parser );
      USING_RESULT unitUse(UnitEntity * unit, SkillRule *, int &useCounter);
      USING_RESULT unitMayUse(UnitEntity * unit, SkillRule * skill);
      void reportUse(USING_RESULT result, TokenEntity * tokenEntity);
      RationalNumber getDailyProduction(); // should return scaled int
      inline ItemRule * getHarvestedResource(){return resourceHarvested_;}
      inline ItemRule * getConsumedResource(){return resourceConsumed_;}
      void extractKnowledge (Entity * recipient, int parameter = 0);
      void printSkillDescription(ostream & out);
    protected:
     ItemRule * resourceHarvested_;
     ItemRule * resourceConsumed_;
      int  harvest_;     
      int days_;
      vector <ToolUseElement *> tools_;
};
extern ApplyActionStrategy     sampleApplyAction;

#endif
