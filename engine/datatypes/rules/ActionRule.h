/***************************************************************************
                          ActionRule.h
                             -------------------
    begin                : Sun Jan 18 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef ACTION_RULE_H
#define ACTION_RULE_H

#include "Rule.h"
class Entity;

/**Action types
  *@author Alex Dribin
  */

class ActionRule : public Rule  {
public: 
      ActionRule ( const string & keyword, GameData * parent) : Rule(keyword, parent){}
      ActionRule ( const ActionRule * prototype );
      ~ActionRule (){}
      virtual STATUS     initialize      ( Parser *parser);
      GameData * createInstanceOfSelf();
      void printDescription(ReportPrinter & out);
      virtual ACTION_RESULT carryOut(Entity * entity);
      inline GameData *  getTargetType() {return targetType_;}
	protected:
       GameData * targetType_; // pointer to prototype
};
#include "RulesCollection.h"
extern RulesCollection <ActionRule>    fx_actions  ;
#endif
