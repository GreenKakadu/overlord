/***************************************************************************
                          BonusElement.h 
                             -------------------
    begin                : Fri Feb 14 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef BONUS_ELEMENT_H
#define BONUS_ELEMENT_H
#include "Element.h"
//#include "SkillElement.h"
#include "SkillRule.h"

using namespace std;


/**
  *@author Alex Dribin
  */
typedef  Element2<SkillRule,int> BasicBonusElement;

class BonusElement : public BasicBonusElement{
public: 
	BonusElement(SkillRule * rule, int bonusPoints) : BasicBonusElement (rule,bonusPoints){}
 ~BonusElement(){}
   inline SkillRule *   getSkill()      const     {return rule_;}
   inline int          getBonusPoints()    const     {return parameter1_;}
   inline void         setBonusPoints(int value)     { parameter1_ = value;}
   inline bool isValidElement() const {return (( rule_ != 0 ) && (parameter1_ != 0));}
//   inline void         setSkill(SkillRule * rule) { rule_ = rule;}
//   void save(ostream & out)
//    {out << rule_->getTag() << " " <<  parameter1_   << endl;}

    string print()
    {
      if( parameter1_ >= 100)
        return string (1,'+') + longtostr(parameter1_ - 100)  + "% to " + rule_->getName();
      else  
        return string (1,'-') + longtostr(100 - parameter1_)  + "% to " + rule_->getName();
    }

  static BonusElement  * readElement (Parser * parser)
      {
        SkillRule * skill = skills[parser->getWord()];
        int bonusPoints = parser->getInteger();
        if(( skill == 0 ) || (bonusPoints == 0) )
          return 0;
        else
        return new BonusElement(skill, bonusPoints);
      }
	protected:

};
typedef  vector <BonusElement *>::iterator BonusIterator;
typedef  vector <BonusElement>::iterator BonusIterator2;

#endif
