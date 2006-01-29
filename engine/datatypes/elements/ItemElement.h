/***************************************************************************
                          ItemElement.h
									Representation of   items
                             -------------------
    begin                : Wed Jan 1 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef ITEM_ELEMENT_H
#define ITEM_ELEMENT_H
#include <iostream>
#include <string>
#include "Element.h"
#include "ItemRule.h"


using namespace std;
typedef Element2<ItemRule, int > BasicItemElement;

class ItemElement : public  Element2< class ItemRule,  int > {
public: 
	ItemElement(ItemRule * rule, int num)  : BasicItemElement (rule,num){}
	ItemElement(Parser * parser) : BasicItemElement (0,0) {rule_ = items[parser->getWord()] ;parameter1_ = parser->getInteger();}
	 ~ItemElement(){}

   inline int getWeight() const {return rule_->getWeight() * parameter1_;}
   inline ItemRule *   getItemType()      const     {return rule_;}
   inline int          getItemNumber()    const     {return parameter1_;}
   inline void         setItemNumber(int value)     { parameter1_ = value;}
   inline void         setItemType(ItemRule * rule) { rule_ = rule;}
   inline bool isValidElement() const {return (rule_ != 0 && parameter1_ != 0);}
/*
 * saves data into output stream  as tags and numbers
 * Examples: "SWRD 5" 
 */
//   void save(ostream & out)
//    {out << rule_->getTag() << " " <<  parameter1_   << endl;}
    
/*
 * returns string with textual representation for reports and debuggers
 * Examples: "5 swords" or "1 horse"
 */
string print() const
{
    if (rule_ == 0) return "";
     if( parameter1_ > 1)
      return longtostr(parameter1_) + " " + rule_->getPluralName()  + rule_->printTag();
    else
      return longtostr(parameter1_) + " " + rule_->print();
}

//should provide non-const print too (or redefine all prints as const)
string print()
{
    if (rule_ == 0) return "";
     if( parameter1_ > 1)
      return longtostr(parameter1_) + " " + rule_->getPluralName()  + rule_->printTag();
    else
      return longtostr(parameter1_) + " " + rule_->print();
}

/*
 * checks if it is possible to read Inventory Element from the input parser
 */
static ItemElement  * readElement (Parser * parser)
{
  ItemRule * item = items[parser->getWord()];
  int number = parser->getInteger();
  if( (item == 0) || (number == 0))
          return 0;  
  else
        return new ItemElement(item, number);
}
static ItemElement readItemElement(Parser * parser)
{
  ItemRule * item = items[parser->getWord()];
  int number = parser->getInteger();
  if( (item == 0) || (number == 0))
          return 0;  
  else
        return  ItemElement(item, number);
}
	
	protected:

};
typedef vector <ItemElement>::iterator ItemElementIterator;
//#ifndef ITEM_ELEMENT_EXPLICIT_H
//#include "ItemElement.cpp"
//#endif

#endif
