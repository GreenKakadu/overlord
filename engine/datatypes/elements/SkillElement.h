/***************************************************************************
                          SkillElement.h  -  description
                             -------------------
    begin                : Fri Jan 24 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef SKILL_ELEMENT_H
#define SKILL_ELEMENT_H
#include <iostream>
#include "Element.h"
#include "SkillRule.h"
#include "BasicLearningStrategy.h"
#include "RulesCollection.h"
using namespace std;
class SkillLevelElement;
extern RulesCollection <SkillRule>     skills;

typedef  Element2<SkillRule,int> BasicSkillElement;
class SkillElement : public BasicSkillElement {
public:
	SkillElement(SkillRule * rule, int expPoints) : BasicSkillElement (rule,expPoints){}
 ~SkillElement(){}
  void save(ostream & out) {out << rule_->getTag() << " " <<  parameter1_  << endl;}
  void print(ostream & out)
    {rule_->printLevel(getLevel(),out); out  << " (" <<  parameter1_ /BasicLearningStrategy::getPointsPerDay() <<")";}
  void printNextLevel(ostream & out)
    {rule_->printLevel(getLevel() +1 ,out);}
  string printName()
  { 
    char buffer[12];
    longtostr(parameter1_/100, buffer);
    if(parameter1_ > 1)
      return string( buffer) + " days of "+ rule_->printName();
    else  
      return string( buffer) + " day of "+ rule_->printName();
  } 
   inline SkillRule *   getSkill()      const     {return rule_;}
   inline int          getExpPoints()    const     {return parameter1_;}
   inline void         setExpPoints(int value)     { parameter1_ = value;}
   inline int          getLevel()    const     {return rule_->getLevel(parameter1_);}
   inline  EntityStatistics * getStats()  {return rule_->getStats(getLevel());}
   inline int getCapacity(int modeIndex){return rule_->getCapacity(modeIndex,getLevel());}
   void  getAllDerivatives(vector <SkillLevelElement *> & derivatives)
   {
     int level;
     for(level = 0; level <= getLevel(); level++)
      rule_->getDerivatives(derivatives, level);
     }
//   inline void         setSkill(SkillRule * rule) { rule_ = rule;}
  static SkillElement  * readElement (Parser * parser)
      {
        SkillRule * skill = skills[parser->getWord()];
        int expPoints = parser->getInteger();
        if( skill == 0  )
          return 0;
        else
        return new SkillElement(skill, expPoints);
      }
	protected:

};
typedef vector <SkillElement>::iterator SkillIterator;
#endif
