/***************************************************************************
                          SkillUseElement.h
                             -------------------
    begin                : Sun Sep 7 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef SKILL_USE_ELEMENT_H
#define SKILL_USE_ELEMENT_H

/**
  *@author Alex Dribin
  */

#include <iostream>
#include <string>
#include "Element.h"
#include "SkillRule.h"
#include "RulesCollection.h"
extern RulesCollection <SkillRule>      skills;
using namespace std;
typedef Element3<SkillRule, Rational , int> BasicSkillUseElement;
class SkillUseElement : public  Element3< class SkillRule,  Rational , int> {
public:
	SkillUseElement(SkillRule * rule, Rational effectiveSkillDays, int duration)  : BasicSkillUseElement (rule,effectiveSkillDays,duration){}
	//SkillUseElement(Parser * parser) : BasicSkillUseElement (0,0,0)
  //{rule_ = skills[parser->getWord()] ;parameter1_ = parser->getInteger();}
	 ~SkillUseElement(){}

   inline SkillRule *  getSkillUsed()      const {return rule_;}
   inline Rational     getDaysUsed()       const {return parameter1_;}
   inline int          getDuration()      const {return parameter2_;}

   inline void  setSkillUsed(SkillRule * skill)  {rule_ = skill;}
   inline void  setDaysUsed(Rational effectiveSkillDays)       {parameter1_ = effectiveSkillDays;}
   inline void  setDuration(int duration)    {parameter2_ = duration;}

         
   void save(ostream & out)
    {out << rule_->getTag() << " " <<  parameter1_   << " " <<  parameter2_   << endl;}

   string printName()
   {
    if (rule_ == 0) return "";
//    char buffer[12];
//      longtostr(parameter1_,buffer);
//    if( parameter1_ > 1)
//      return string(buffer) + " days of using " + rule_->printName();
//    else
//      return string(buffer) + " day of using " + rule_->printName();
      return parameter1_.printName() + " days of using " + rule_->printName();
  }

   void report(ostream & out)
   {
    if (rule_ != 0) 
      out << parameter1_ <<" of "<< parameter2_<< " days of using " + rule_->printName();
  }

  static SkillUseElement  * readElement (Parser * parser)
      {
        SkillRule * skill = skills[parser->getWord()];
        Rational effectiveSkillDays = parser->getRational();
        int duration = parser->getInteger();
        if( skill == 0  )
          return 0;
        if( duration == 0  )
          return 0;
        else
        return new SkillUseElement(skill, effectiveSkillDays, duration);
      }
};
typedef vector <SkillUseElement *>::iterator SkillUseIterator;

#endif
