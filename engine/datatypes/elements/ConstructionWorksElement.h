/***************************************************************************
                          ConstructionWorksElement.h
                             -------------------
    begin                : Wed Sep 3 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
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
#include "RationalNumber.h"
#include "GameFacade.h"

using namespace std;


/**
  *@author Alex Dribin
  */
typedef  Element2<ConstructionWorksVariety,RationalNumber> BasicConstructionWorksElement;

class ConstructionWorksElement : public BasicConstructionWorksElement  {
public: 
	ConstructionWorksElement(ConstructionWorksVariety * works, RationalNumber amount): BasicConstructionWorksElement (works,amount){}
	ConstructionWorksElement(ConstructionWorksVariety * works, int amount): BasicConstructionWorksElement (works,amount){}
	~ConstructionWorksElement(){}
   inline ConstructionWorksVariety *   getWorkType()      const     {return rule_;}
   inline RationalNumber          getWorkAmount()    const     {return parameter1_;}
//   inline void         setWorkAmount(int value)     { parameter1_ = value;}
   inline void         setWorkAmount(RationalNumber value)     { parameter1_ = value;}
   inline bool isValidElement() const {return (( rule_ != 0 ) && (parameter1_ != 0));}

//   void save(ostream & out)
//    {out << rule_->getTag() << " " <<  parameter1_   << endl;}
   string print()
    {
      if(parameter1_ > 1)
        return parameter1_.print() + " of " + rule_->getPluralName();
      else
        return parameter1_.print() + " of " + rule_->getName();
    }

  static ConstructionWorksElement  * readElement (Parser * parser)
      {
        ConstructionWorksVariety * works = gameFacade->construction_works[parser->getWord()];
        RationalNumber amount = parser->getRationalNumber();
        if(( works == 0 ) || (parser->status != OK) )
          return 0;
        else
        return new ConstructionWorksElement(works, amount);
      }
	protected:
};

#endif
