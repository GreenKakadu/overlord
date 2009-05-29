/***************************************************************************
                          ResourceElement.h
									Representation of Resources at location
                             -------------------
    begin                : Wed Jan 1 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef RESOURCE_ELEMENT_H
#define RESOURCE_ELEMENT_H
#include <iostream>
#include "Element.h"
#include "ItemRule.h"
#include "RationalNumber.h"
class Parser;
using namespace std;

typedef Element3<  ItemRule,  int ,  RationalNumber> BasicResourceElement;

class ResourceElement : public BasicResourceElement  {
public: 
	ResourceElement(ItemRule * rule, int num)  : BasicResourceElement (rule,num,num){}
	ResourceElement(ItemRule * rule, int num, int rest)  : BasicResourceElement (rule,num,rest){}
	~ResourceElement(){}
//  /** No descriptions */
//   void save(ostream & out)
//   {out << rule_->getTag() << " " <<  parameter1_  << endl;}
   void reportResourse(ostream & out)
   {
     out<< print() << " (" << parameter2_ <<" available).";
   }

   string print()
   {
    if (rule_ == 0) return "";
    if( parameter1_ > 1)
      return longtostr(parameter1_) + " " + rule_->getPluralName()  + rule_->printTag();
    else
      return longtostr(parameter1_) + " " + rule_->print();
  }

   inline ItemRule *   getResource()      const     {return rule_;}
   inline int          getResourceAmount()         {return parameter1_;}
   inline void         setResourceAmount(int value)     { parameter1_ = value;}
   inline void         setResource(ItemRule * rule) { rule_ = rule;}
   inline RationalNumber     getAvailableResource()    const  {return parameter2_;}
   inline void         setAvailableResource(const RationalNumber& value)  { parameter2_  = value;}
	
  /** Checks if it is possible to read Resource Element from the input parser */
  static ResourceElement read(Parser * parser)
    {
	    return ResourceElement ( items[parser->getWord()], parser->getInteger() );
    }

  static ResourceElement * readElement(Parser * parser)
    {
        ItemRule * item = items[parser->getWord()];
        int number = parser->getInteger();
        if( item == 0  )
          return 0;
        if (number == 0 )
          return 0;
        else
				{
        int rest = parser->getInteger();
				if (rest == 0)
          return new ResourceElement(item, number);
				else
          return new ResourceElement(item, number, rest);
				}
    }
  static ResourceElement * readStoredElement(Parser * parser)
    {
        ItemRule * item = items[parser->getWord()];
        int number = parser->getInteger();
        if( item == 0  )
          return 0;
        if (number == 0 )
          return 0;
        else
		{
          	return new ResourceElement(item, number);
		}
    }
	protected:
};
typedef vector <ResourceElement *>::iterator ResourceElementIterator;
#endif
