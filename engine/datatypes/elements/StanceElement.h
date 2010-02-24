/***************************************************************************
                          StanceElement.h  -  description
                             -------------------
    begin                : Fri Jan 10 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#ifndef STANCE_ELEMENT_H
#define STANCE_ELEMENT_H

#include   <vector>
#include "StringData.h"
#include "Element.h"
#include "Entity.h"
#include "StanceVariety.h"

typedef  Element2<StanceVariety,Entity *> BasicStanceElement;
class StanceElement:public  Element2<StanceVariety,Entity *>
{
  public:     
    StanceElement(StanceVariety * stance, Entity * entity)  : BasicStanceElement (stance,entity){}
    //StanceVariety * getRule(){return stance_;}
    //void setStance(StanceVariety *stance){ stance_ = stance;}
   // Entity * getParameter1(){return entity_;}
    string print()
    {
      return string("Attitude to ") + getParameter1()->print() + " is " + getRule()->getName() + ".";
    }
    vector <AbstractData *> aPrint()
    {
      vector <AbstractData *> v; 
      v.push_back(new StringData("Attitude to ")); 
      v.push_back(getParameter1()); 
      v.push_back(new StringData(" is ")); 
      v.push_back(getRule()); 
      return v;
    }    
   //Entity * entity_;
   //StanceVariety * stance_;
};
typedef vector<StanceElement>::iterator StanceIterator;
#endif
