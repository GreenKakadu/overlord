/***************************************************************************
                          PermissionElement.h
Permission on some sort of activity for factions with stances at least as good as stance
                             -------------------
    begin                : Wed Jan 1 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef PERMISSION_ELEMENT_H
#define PERMISSION_ELEMENT_H
#include <iostream>
#include <string>
#include "GameFacade.h"
#include "Element.h"
#include "Rule.h"
#include "StanceVariety.h"
#include "RuleIndex.h"

using namespace std;
typedef Element2<Rule, StanceVariety * > BasicPermissionElement;
class PermissionElement : public  BasicPermissionElement {
public: 
	PermissionElement(Rule * rule,StanceVariety * stance)   : BasicPermissionElement (rule,stance){}
	 ~PermissionElement(){}
   inline Rule *          getRule()      const     {return rule_;}
   inline StanceVariety * getStance()    const     {return parameter1_;}
   inline bool         isValidElement() const {return (rule_ )&&(parameter1_); }
	
string print()
{
    if ( !( (rule_ )&&(parameter1_) ) ) return "";
      return  rule_->print()  + " " + parameter1_->print(); 
}



void save(ostream & out)
{
  out << rule_->getTag() << " " <<  parameter1_->getTag()   << endl;
}



/*
 * Reads (if it is possible) Permission Element from the input parser
 */
static PermissionElement  * readElement (Parser * parser)
{
  Rule * rule = gameFacade->ruleIndex.findRule(parser->getWord());
  if( rule == 0  )
          return 0;
  StanceVariety * stance  = gameFacade->stances[parser->getWord()];
  if( stance == 0  )
          return 0;
  else
  return new PermissionElement(rule, stance);
}
	protected:

};

#endif
