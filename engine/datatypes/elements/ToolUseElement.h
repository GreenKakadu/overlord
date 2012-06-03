/***************************************************************************
                          ToolUseElement.h
									Representation of   items
                             -------------------
    begin                : Wed Jan 1 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef TOOL_USE_ELEMENT_H
#define TOOL_USE_ELEMENT_H
#include <iostream>
#include "Element.h"
#include "ItemRule.h"

using namespace std;
typedef Element2<ItemRule, int > BasicToolUseElement;
class ToolUseElement : public BasicToolUseElement  {
public: 
	ToolUseElement(ItemRule * rule, int num)  : BasicToolUseElement (rule,num)
  {        if ( num == 0)
               parameter1_ = 33; // default
  }
	~ToolUseElement(){}
  /** No descriptions */
//   void save(ostream & out)
//   {out << rule_->getTag() << " " <<  parameter1_   << endl;}

  /** No descriptions */
   inline ItemRule *   getItemType()      const     {return rule_;}
   inline int          getBonus()    const     {return parameter1_;}
   inline void         setBonus(int value)     { parameter1_ = value;}
   inline void         setItemType(ItemRule * rule) { rule_ = rule;}
  static ToolUseElement  * readElement (Parser * parser)
      {
        ItemRule * item = gameFacade->items[parser->getWord()];
        int number = parser->getInteger();
        if( item == 0  )
          return 0;
        else
        return new ToolUseElement(item, number);
      }
	
	protected:

};

#endif
