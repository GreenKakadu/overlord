/***************************************************************************
                          RuleIndex.cpp
                             -------------------
    begin                : Tue Aug 5 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include <iostream>
#include "GameData.h"
#include "Rule.h"
#include "RuleIndex.h"
#include "BasicRulesCollection.h"
extern bool ciStringCompare(const string& s1,const string& s2);

RuleIndex::RuleIndex()
{
}
RuleIndex::~RuleIndex(){
}



void RuleIndex::addRules ( BasicRulesCollection  * collection )
{
		if(collection->getStatus() == OK)
			{
 				rules_.push_back(collection);
			}
}



BasicRulesCollection  * RuleIndex::findRuleCollection(const string& keyword)
{
  RulesCollectionIterator ruleIter;
  for( ruleIter = rules_.begin(); ruleIter != rules_.end(); ++ruleIter)
	   {
//        cout << " findRuleCollection:: "<<keyword << " in "<< (*ruleIter)->getCollectionKeyword()<<endl;
        if(!ciStringCompare(keyword, (*ruleIter)->getCollectionKeyword()))
        {
          return (*ruleIter);
        }
     }
   return 0;
}



Rule * RuleIndex::findRule( const string& tag)
{
  RulesCollectionIterator ruleIter;
  GameData * current;
  for( ruleIter = rules_.begin(); ruleIter != rules_.end(); ++ruleIter)
	   {
       current =  (*ruleIter)->findByTag(tag,false);
       if(current)
        {
            return dynamic_cast<Rule *>(current);
        }
     }
   return 0;
}
