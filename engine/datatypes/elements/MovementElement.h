/***************************************************************************
                          MovementElement.h
									Representation of Equippable  items
                             -------------------
    begin                : Wed Jan 1 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef MOVEMENT_ELEMENT_H
#define MOVEMENT_ELEMENT_H
#include <iostream>
#include "Element.h"
#include "SkillLevelElement.h"
#include "MovementVariety.h"
#include "VarietiesCollection.h"
class Parser;
using namespace std;
class MovementVariety;
extern VarietiesCollection <MovementVariety> movementModes;
                    
typedef Element3<  MovementVariety,  int ,  SkillLevelElement *> BasicMovementElement;

class MovementElement : public BasicMovementElement  {
public: 
	MovementElement(MovementVariety * mode, int time, SkillLevelElement * condition=0)
                     : BasicMovementElement (mode,time,condition){}
	~MovementElement(){/*if(parameter2_) delete parameter2_;*/}
  /** No descriptions */
   void save(ostream & out)
   {out << rule_->getTag() << " " <<  parameter1_ ;
      if(parameter2_) out << "  " << *parameter2_ ;
      out << endl;}
   void print(ostream & out)
   {out << " " <<  parameter1_ << "days of "<< rule_->getTag() ;
      if(parameter2_) out << " using " << *parameter2_ ;
      out << endl;}
friend  ostream &operator << ( ostream &out, MovementElement element)
{
  out << element.rule_->getTag() << " " <<  element.parameter1_ << "  " << *(element.parameter2_)  << endl;
	return out;
}

  /** No descriptions */
   inline MovementVariety *   getMovementMode()      const     {return rule_;}
   inline int          getTravelTime()    const     {return parameter1_;}
   inline void         setTravelTime(int time)     { parameter1_ = time;}
   inline void         setMovementMode(MovementVariety * mode) { rule_ = mode;}
   inline SkillLevelElement *          getCondition()    const  {return parameter2_;}
	 
  /** Checks if it is possible to read Movement Element from the input parser */
  static MovementElement read(Parser * parser)
{  
  string modeTag = parser->matchWord();
  MovementVariety * movementMode = 0;
  if(modeTag.empty())
    return MovementElement (0,0,0);

  movementMode = movementModes[modeTag]; 
  if(movementMode == 0)
    return MovementElement (0,0,0);
  else
    parser->getWord();
  int time = parser->getInteger();
   SkillLevelElement * skillCondition = SkillLevelElement::readElement(parser);
          
  return MovementElement (  movementMode, time, skillCondition);
}
	protected:

};
//#ifndef MOVEMENT_ELEMENT_EXPLICIT_H
//#include "MovementElement.cpp"
//#endif


#endif
