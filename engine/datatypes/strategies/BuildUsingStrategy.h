/***************************************************************************
                          BuildUsingStrategy.h 
                             -------------------
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
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

class BuildUsingStrategy : public BasicProductionStrategy  {
public: 
      BuildUsingStrategy ( const string & keyword, GameData * parent): BasicProductionStrategy(keyword, parent){}
      BuildUsingStrategy ( const BuildUsingStrategy * prototype );
		 ~BuildUsingStrategy(){};
      GameData * createInstanceOfSelf();
      STATUS initialize        ( Parser *parser );
//      USING_RESULT use(TokenEntity * tokenEntity, SkillRule *, int &useCounter);
//      USING_RESULT mayUse(TokenEntity * tokenEntity, SkillRule * skill);
      USING_RESULT unitUse(UnitEntity * unit, SkillRule *, int &useCounter);
      USING_RESULT unitMayUse(UnitEntity * unit, SkillRule * skill);
      void printSkillDescription(ostream & out);
      void extractKnowledge (Entity * recipient, int parameter);
	protected:
      ConstructionWorksVariety * constructionWorkProduced_;
};
extern BuildUsingStrategy       sampleBuildUsing;

#endif
