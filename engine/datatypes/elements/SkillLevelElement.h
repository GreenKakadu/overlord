/***************************************************************************
                          SkillLevelElement.h  -  description
                             -------------------
    begin                : Fri Feb 28 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef SKILL_LEVEL_ELEMENT_H
#define SKILL_LEVEL_ELEMENT_H
#include <iostream>
#include "Element.h"
#include "IntegerData.h"
#include "StringData.h"
#include "SkillRule.h"

using namespace std;


typedef  Element2<SkillRule,int> BasicSkillLevelElement;
class SkillLevelElement : public BasicSkillLevelElement {
public:
	SkillLevelElement(SkillRule * rule, int level) : BasicSkillLevelElement (rule,level){}
 ~SkillLevelElement(){}
  inline bool operator ==  (SkillLevelElement  rule2)  {return ( (rule_ == rule2.getRule()) && (parameter1_ == rule2.getLevel()) );}
  inline bool operator !=  (SkillLevelElement  rule2)  {return ( (rule_ != rule2.getRule())||(parameter1_ != rule2.getLevel()) );}
  inline bool operator >   (SkillLevelElement  rule2)  {return ( (rule_->getTag() > rule2.getRule()->getTag())|| (parameter1_ >rule2.getLevel()) );}
  inline bool operator <   (SkillLevelElement  rule2)  {return ( (rule_->getTag() < rule2.getRule()->getTag())|| (parameter1_ <rule2.getLevel()) );}
   inline SkillRule *   getSkill()      const     {return rule_;}
   inline int          getLevel()    const     {return parameter1_;}
   inline  void         setLevel(int level)         {parameter1_ = level;}
   inline  EntityStatistics * getStats()  {return rule_->getStats(parameter1_);}
	protected:



  public:
  string print() { return rule_->printLevel(parameter1_); }
//void save(ostream & out) {out << rule_->getTag() << " " <<  parameter1_  << endl;}

//void print(ostream & out)
//    {rule_->printLevel(parameter1_,out);}

void printNextLevel(ostream & out)
    {rule_->printLevel(parameter1_ +1 ,out);}

//friend  ostream &operator << ( ostream &out, SkillLevelElement element)
//{
//  element.save(out);
//	return out;
//}

vector <AbstractData *> aPrintLevel()
{
    vector <AbstractData *> out;
    if(parameter1_ >0)
    {
    out.push_back(new IntegerData(parameter1_));
    }
    switch (parameter1_)
    {
      case 0:
       out.push_back(new StringData("novice "));
       break;
      case 1:
       out.push_back(new StringData("st "));
       break;
      case 2:
       out.push_back(new StringData("nd "));
       break;
      case 3:
       out.push_back(new StringData("rd "));
       break;
      default:
       out.push_back(new StringData("th "));
      }
    out.push_back(rule_);
    return out;
}
inline bool skillEqual ( SkillLevelElement * skill1, SkillLevelElement * skill2)
{
  return( skill1->getSkill() == skill2->getSkill() && skill1->getLevel() == skill2->getLevel());
}



inline bool skillLessThan ( SkillLevelElement * skill1, SkillLevelElement * skill2)
{
  if(skill1->getSkill() == skill2->getSkill())
      return skill1->getLevel() <= skill2->getLevel();
  if(skill1->getSkill()->getTag() < skill2->getSkill()->getTag())
    return true;
  else
    return false;
}



void  getAllDerivatives(vector <SkillLevelElement *> & derivatives)
{
     int level;
     for(level = 0; level <= parameter1_; level++)
      rule_->getDerivatives(derivatives, level);
}
//   inline void         setSkill(SkillRule * rule) { rule_ = rule;}



static SkillLevelElement  * readElement (Parser * parser)
{
        int level;
        string skillTag = parser->matchWord();
        if (skillTag.empty())
            return 0;
        SkillRule * skill = gameFacade->skills[skillTag];
        if( skill == 0  )
          return 0;
        else
          parser->getWord();
          
        if(parser->matchInteger())
          level = parser->getInteger();
          else
            level = 1;
        return new SkillLevelElement(skill, level);
}

void saveLevelElement (ostream & out)
{
    out<<"SKILL_ELEMENT "<<getSkill()->getTag()<<" "<<getLevel()<<endl;
    getSkill()->saveLevel(out, getLevel(),true);
    out<<endl;
}

};
typedef vector <SkillLevelElement *>::iterator SkillLevelIterator;
#endif
