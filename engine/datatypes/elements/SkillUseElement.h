/***************************************************************************
                          SkillUseElement.h
                             -------------------
    begin                : Sun Sep 7 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
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
#include "IntegerData.h"
#include "StringData.h"

using namespace std;
typedef Element3<SkillRule, RationalNumber , int> BasicSkillUseElement;
class SkillUseElement : public  Element3< class SkillRule,  RationalNumber , int> {
public:
	SkillUseElement(SkillRule * rule, RationalNumber effectiveSkillDays, int duration)  : BasicSkillUseElement (rule,effectiveSkillDays,duration){}
	//SkillUseElement(Parser * parser) : BasicSkillUseElement (0,0,0)
  //{rule_ = skills[parser->getWord()] ;parameter1_ = parser->getInteger();}
	 ~SkillUseElement(){}

   inline SkillRule *  getSkillUsed()      const {return rule_;}
   inline RationalNumber     getDaysUsed()       const {return parameter1_;}
   inline int          getDuration()      const {return parameter2_;}

   inline void  setSkillUsed(SkillRule * skill)  {rule_ = skill;}
   inline void  setDaysUsed(RationalNumber effectiveSkillDays)       {parameter1_ = effectiveSkillDays;}
   inline void  setDuration(int duration)    {parameter2_ = duration;}

         
//   void save(ostream & out)
//    {out << rule_->getTag() << " " <<  parameter1_   << " " <<  parameter2_   << endl;}

   string print()
   {
    if (rule_ == 0) return "";
      return parameter1_.print() + " days of using " + rule_->print();
  }
 vector <AbstractData *> aPrintSkillUse()
 {
 vector <AbstractData *> v;// = SkillLevelElement(rule_,getLevel()).aPrintLevel();
 //v.push_back(new StringData(" ("));
 v.push_back(new RationalNumber(getParameter1()));
 v.push_back(new StringData(" days of using "));
  v.push_back(getSkillUsed());
  v.push_back(new StringData("."));
 return v;
 }

  static SkillUseElement  * readElement (Parser * parser)
      {
        SkillRule * skill = gameFacade->skills[parser->getWord()];
        RationalNumber effectiveSkillDays = parser->getRationalNumber();
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
