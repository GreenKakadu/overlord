/***************************************************************************
                          RaceElement.h
									Representation of races
                             -------------------
    begin                : Wed Jan 1 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef RACE_ELEMENT_H
#define RACE_ELEMENT_H
#include <iostream>
#include <string>
#include "Element.h"
#include "RaceRule.h"
#include "RulesCollection.h"
extern RulesCollection <RaceRule>      races;
using namespace std;
typedef Element2<RaceRule, int > BasicRaceElement;
class RaceElement : public  BasicRaceElement {
public: 
	RaceElement(RaceRule * rule, int num)  : BasicRaceElement (rule,num){}
	RaceElement(Parser * parser) : BasicRaceElement (0,0) {rule_ = races[parser->getWord()] ;parameter1_ = parser->getInteger();}
	 ~RaceElement(){}
   inline int getWeight()               const {return rule_->getWeight() * parameter1_;}
   inline int getCapacity(int moveMode) const {return rule_->getCapacity(moveMode) * parameter1_;}
   inline RaceRule *   getRace()      const     {return rule_;}
   inline int          getFigures()    const     {return parameter1_;}
   inline void         setFigures(int value)     { parameter1_ = value;}
   inline void         setRace(RaceRule * rule) { rule_ = rule;}
   inline bool         isValidElement() const {return (rule_ )&&(parameter1_); }
//  static RaceElement readRaceElement(Parser * parser);
	
string RaceElement::printName()
{
    if (rule_ == 0) return "";
    char buffer[12];
      longtostr(parameter1_,buffer);
    if( parameter1_ > 1)
      return string(buffer)  + " " + rule_->getPluralName()  + rule_->printTag();
    else
      return string(buffer)  + " " + rule_->printName();
}



void RaceElement::save(ostream & out)
{
  out << rule_->getTag() << " " <<  parameter1_   << endl;
}



/*
 * Reads (if it is possible) Race Element from the input parser
 */
static RaceElement  * RaceElement::readElement (Parser * parser)
{
  RaceRule * item = races[parser->getWord()];
  int number = parser->getInteger();
  if( item == 0  )
          return 0;
  else
  return new RaceElement(item, number);
}
	protected:

};
//#ifndef ITEM_ELEMENT_EXPLICIT_H
//#include "RaceElement.cpp"
//#endif


#endif
