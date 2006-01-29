/***************************************************************************
                          BasicProductionStrategy.h
                             -------------------
    begin                : Thu Sep 18 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef BASIC_PRODUCTION_STRATEGY_H
#define BASIC_PRODUCTION_STRATEGY_H

#include <vector>
#include "BasicUsingStrategy.h"
#include "RationalNumber.h"
/**
  *@author Alex Dribin
  */
class ToolUseElement;
class ItemElement;

/**Producting skills
  *@author Alex Dribin
  */


class BasicProductionStrategy : public BasicUsingStrategy  {
public: 
      BasicProductionStrategy ( const string & keyword, GameData * parent): BasicUsingStrategy(keyword, parent){}
      BasicProductionStrategy ( const BasicProductionStrategy * prototype );
		  virtual ~BasicProductionStrategy(){};
      virtual GameData * createInstanceOfSelf();
      virtual STATUS initialize        ( Parser *parser );
      virtual USING_RESULT unitMayUse(UnitEntity * unit, SkillRule * skill);
      virtual void reportUse(USING_RESULT result, TokenEntity * unit);
      virtual void extractKnowledge (Entity * recipient, int parameter = 0);
      virtual int checkResourcesAvailability(TokenEntity * unit);
      virtual bool consumeResources(TokenEntity * unit, int numCycles);
              void printSkillDescription(ostream & out);
      USING_RESULT  checkTarget(UnitEntity * unit, GameData * targetType);
			RationalNumber getEffectiveProductionRate(UnitEntity * unit, SkillRule * skill);
	protected:
      vector <ItemElement *> resources_;
      int productNumber_ ;
      vector <ToolUseElement *> tools_;
      int productionDays_;
};

#endif
