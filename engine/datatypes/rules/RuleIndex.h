/***************************************************************************
                          RuleIndex.h
                             -------------------
    begin                : Tue Aug 5 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#ifndef RULE_INDEX_H
#define RULE_INDEX_H
#include <string>
#include <vector>

#include "BasicRulesCollection.h"

/**Contains list of all rules
  *@author Alex Dribin
  */
//class BasicRulesCollection;
class Rule;

class RuleIndex {
public: 
	RuleIndex();
	~RuleIndex();
  void addRules( BasicRulesCollection  * collection);
  BasicRulesCollection  * findRuleCollection(const string& keyword);
  Rule * findRule( const string& tag);
  inline vector < BasicRulesCollection  *> & getAllRules() {return rules_;}
    protected:
  vector < BasicRulesCollection  *>  rules_;
};
typedef vector < BasicRulesCollection  *>::iterator RulesCollectionIterator;
extern RuleIndex ruleIndex;
#endif
