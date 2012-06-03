/***************************************************************************
                          BuildUsingStrategy.h 
                             -------------------
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#ifndef BUILD_USING_H
#define BUILD_USING_H

#include <vector>
#include "BasicProductionStrategy.h"

/**Building
  *@author Alex Dribin
  */
class ToolUseElement;
class ConstructionWorksVariety;
class ItemRule;
class UnitEntity;
class ConstructionEntity;

class BuildUsingStrategy : public BasicProductionStrategy  {
public: 
      BuildUsingStrategy ( const string & keyword, GameData * parent): BasicProductionStrategy(keyword, parent){}
      BuildUsingStrategy ( const BuildUsingStrategy * prototype );
		 ~BuildUsingStrategy(){};
      virtual void save(ostream &out);
      GameData * createInstanceOfSelf();
      STATUS initialize        ( Parser *parser );
//      USING_RESULT use(TokenEntity * tokenEntity, SkillRule *, int &useCounter);
//      USING_RESULT mayUse(TokenEntity * tokenEntity, SkillRule * skill);
      void reportUse(USING_RESULT result, TokenEntity * tokenEntity);      
      USING_RESULT unitUse(UnitEntity * unit, SkillRule *, int &useCounter,OrderLine * order);
      USING_RESULT unitMayUse(UnitEntity * unit, SkillRule * skill);
      USING_RESULT build(UnitEntity * unit, SkillRule * skill, ConstructionEntity * construction );
      void printSkillDescription(ostream & out);
      void extractKnowledge (Entity * recipient, int parameter);
			BasicUsingStrategy * cloneSelf();
	protected:
      ConstructionWorksVariety * constructionWorkProduced_;
};
extern BuildUsingStrategy       sampleBuildUsing;

#endif
