/***************************************************************************
                          ActionRule.h
                             -------------------
    begin                : Sun Jan 18 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
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
#include "RulesCollection.h"
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
      virtual void save(ostream &out);
      GameData * createInstanceOfSelf();
      void printDescription(ReportPrinter & out);
      vector <AbstractData *> aPrint();
      virtual ACTION_RESULT carryOut(Entity * entity, AbstractData * parameter = 0, int value =0);
      inline GameData *  getTargetType() {return targetType_;}
	protected:
       GameData * targetType_; // pointer to prototype
};
//extern RulesCollection <ActionRule>    fx_actions  ;
extern  ActionRule      sampleAction;

#endif
