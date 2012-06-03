/***************************************************************************
                          FactionPermissionElement.h
        Permission on some sort of activity for factions 

                             -------------------
    begin                : Wed Jan 1 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef FACTION_PERMISSION_ELEMENT_H
#define FACTION_PERMISSION_ELEMENT_H
#include <iostream>
#include <string>
#include "Element.h"
#include "Rule.h"
#include "RuleIndex.h"
#include "FactionEntity.h"

using namespace std;
typedef Element3<Rule, FactionEntity * , int> BasicFactionPermissionElement;
class FactionPermissionElement : public  BasicFactionPermissionElement {
public: 
	FactionPermissionElement(Rule * rule, FactionEntity * faction, int permission)
          : BasicFactionPermissionElement (rule,faction,permission){}
	 ~FactionPermissionElement(){}
   inline Rule *          getRule()      const      {return rule_;}
   inline FactionEntity * getFaction()    const     {return parameter1_;}
   inline bool         getPermission()    const     {if(parameter2_) return true; else return false;}
   inline bool         isValidElement() const {return (rule_ )&&(parameter1_); }
	
string print()
{
    if ( !( (rule_ )&&(parameter1_) ) ) return "";
    if(parameter2_)
      return  string("Permitted ") + rule_->print()  + " for " + parameter1_->print();
    else   
      return  string("Forbidden ") + rule_->print()  + " for " + parameter1_->print();
}



void save(ostream & out)
{
  out << rule_->getTag() << " " <<  parameter1_->getTag() << parameter2_ << endl;
}



/*
 * Reads (if it is possible) Permission Element from the input parser
 */
static FactionPermissionElement  * readElement (Parser * parser)
{
  Rule * rule = gameFacade->ruleIndex.findRule(parser->getWord());
  if( rule == 0  )
          return 0;
  FactionEntity * faction  = gameFacade->factions[parser->getWord()];
  if( faction == 0  )
          return 0;
  int permission = parser->getInteger();       

  return new FactionPermissionElement(rule, faction,permission);
}
	protected:

};


#endif
