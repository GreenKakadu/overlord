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
      virtual void reportUse(USING_RESULT result, PhysicalEntity * unit);
      virtual void extractKnowledge (Entity * recipient, int parameter = 0);
      virtual int checkResourcesAvailability(PhysicalEntity * unit);
      virtual bool consumeResources(PhysicalEntity * unit, int numCycles);
              void printSkillDescription(ostream & out);
	protected:
      vector <ItemElement *> resources_;
      int productNumber_ ;
      vector <ToolUseElement *> tools_;
};

#endif
