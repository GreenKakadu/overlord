/***************************************************************************
                          ConstructionWorksElement.h
                             -------------------
    begin                : Wed Sep 3 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#ifndef CONSTRUCTION_WORKS_ELEMENT_H
#define CONSTRUCTION_WORKS_ELEMENT_H
#include "Element.h"
#include "ConstructionWorksVariety.h"
#include "VarietiesCollection.h"
#include "Rational.h"

using namespace std;
extern VarietiesCollection <ConstructionWorksVariety>     construction_works;


/**
  *@author Alex Dribin
  */
typedef  Element2<ConstructionWorksVariety,Rational> BasicConstructionWorksElement;

class ConstructionWorksElement : public BasicConstructionWorksElement  {
public: 
	ConstructionWorksElement(ConstructionWorksVariety * works, Rational amount): BasicConstructionWorksElement (works,amount){}
	ConstructionWorksElement(ConstructionWorksVariety * works, int amount): BasicConstructionWorksElement (works,amount){}
	~ConstructionWorksElement(){}
   inline ConstructionWorksVariety *   getWorkType()      const     {return rule_;}
   inline Rational          getWorkAmount()    const     {return parameter1_;}
//   inline void         setWorkAmount(int value)     { parameter1_ = value;}
   inline void         setWorkAmount(Rational value)     { parameter1_ = value;}
   inline bool isValidElement() const {return (( rule_ != 0 ) && (parameter1_ != 0));}

   void save(ostream & out)
    {out << rule_->getTag() << " " <<  parameter1_   << endl;}
   void print(ostream & out)
    {
      out << parameter1_<<" of ";
      if(parameter1_ > 1)
        out<< rule_->getPluralName();
      else
        out<<rule_->getName();
    }

  static ConstructionWorksElement  * readElement (Parser * parser)
      {
        ConstructionWorksVariety * works = construction_works[parser->getWord()];
        Rational amount = parser->getRational();
        if(( works == 0 ) || (parser->status != OK) )
          return 0;
        else
        return new ConstructionWorksElement(works, amount);
      }
	protected:
};

#endif
