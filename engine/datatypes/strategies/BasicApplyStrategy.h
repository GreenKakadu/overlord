/***************************************************************************
                          BasicApplyStrategy.h
                             -------------------
    begin                : Tue Feb 28 2006
    copyright            : (C) 2006 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef APPLY_H
#define APPLY_H

#include "OverlordTypes.h"
#include "Strategy.h"

class TokenEntity;
class SkillElement;
class SkillRule;
class Entity;
class UnitEntity;

/**Use of skill in combat
  *@author Alex Dribin
  */

class BasicApplyStrategy : public Strategy  {
public:
      BasicApplyStrategy ( const string & keyword, GameData * parent): Strategy(keyword, parent){}
      BasicApplyStrategy ( const BasicApplyStrategy * prototype );
		 ~BasicApplyStrategy(){};
      virtual STATUS     initialize      ( Parser *parser);
      GameData * createInstanceOfSelf();


	protected:
      vector <ItemElement *> resources_;
};
extern BasicApplyStrategy         sampleApply;

#endif
