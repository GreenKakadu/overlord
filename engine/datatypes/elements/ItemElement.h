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
#include "RulesCollection.h"
extern RulesCollection <ItemRule>      items;
using namespace std;
//class ItemRule;
typedef Element2<ItemRule, int > BasicItemElement;
class ItemElement : public  Element2< class ItemRule,  int > {
public: 
	ItemElement(ItemRule * rule, int num)  : BasicItemElement (rule,num){}
	ItemElement(Parser * parser) : BasicItemElement (0,0) {rule_ = items[parser->getWord()] ;parameter1_ = parser->getInteger();}
	 ~ItemElement(){}
  /** No descriptions */
   void save(ostream & out)
    {out << rule_->getTag() << " " <<  parameter1_   << endl;}
    
   string printName()
   {
    if (rule_ == 0) return "";
    char buffer[12];
      longtostr(parameter1_,buffer);
//		  sprintf(buffer,"%d",parameter1_);// May use hand-made convertor itoa
    if( parameter1_ > 1)
      return string(buffer) + " " + rule_->getPluralName()  + rule_->printTag();
    else
      return string(buffer) + " " + rule_->printName();
  }

  /** No descriptions */
  inline int getWeight() const {return rule_->getWeight() * parameter1_;}
   inline ItemRule *   getItemType()      const     {return rule_;}
   inline int          getItemNumber()    const     {return parameter1_;}
   inline void         setItemNumber(int value)     { parameter1_ = value;}
   inline void         setItemType(ItemRule * rule) { rule_ = rule;}
  /** Checks if it is possible to read Inventory Element from the input parser */
  // friend ItemElement readItemElement(Parser * parser);
  static ItemElement  * readElement (Parser * parser)
      {
        ItemRule * item = items[parser->getWord()];
        int number = parser->getInteger();
        if( item == 0  )
          return 0;  // may be number also should be non-zero?
        else
        return new ItemElement(item, number);
      }
//  static ItemElement readItemElement(Parser * parser);
	
	protected:

};
typedef vector <ItemElement *>::iterator ItemElementIterator;
//#ifndef ITEM_ELEMENT_EXPLICIT_H
//#include "ItemElement.cpp"
//#endif

#endif
