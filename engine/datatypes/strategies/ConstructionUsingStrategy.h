/***************************************************************************
                          ConstructionUsingStrategy.h
                             -------------------
    begin                : Wed Sep 3 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#ifndef CONSTRUCTION_USING_STRATEGY_H
#define CONSTRUCTION_USING_STRATEGY_H

#include "BasicUsingStrategy.h"
class ConstructionRule;
class ItemRule;
class TokenEntity;
class UnitEntity;

/**
  *@author Alex Dribin
  Construction skills initiate building of new construction.
  
  */

class ConstructionUsingStrategy : public BasicUsingStrategy  {
public: 
      ConstructionUsingStrategy ( const string & keyword, GameData * parent): BasicUsingStrategy(keyword, parent){}
      ConstructionUsingStrategy ( const ConstructionUsingStrategy * prototype );
		 ~ConstructionUsingStrategy(){};
      GameData * createInstanceOfSelf();
      STATUS initialize        ( Parser *parser );
      virtual void save(ostream &out);
      USING_RESULT unitUse(UnitEntity * unit, SkillRule * skill, int & useCounter,OrderLine * order);
      USING_RESULT unitMayUse(UnitEntity * unit, SkillRule * skill);
      void printSkillDescription(ostream & out);
      void extractKnowledge (Entity * recipient, int parameter);
			BasicUsingStrategy * cloneSelf();
	protected:
      ConstructionRule * construction_;
      int productionDays_;
      int multiplicationFactor_; // not used
      ItemRule * resourceType_ ;
      int resourceNumber_ ;
      ConstructionRule * constructionToUpgrade_;
};
extern ConstructionUsingStrategy       sampleConstructionUsing;

#endif
