/***************************************************************************
                          InventoryElement.h
									Representation of Equippable  items
                             -------------------
    begin                : Wed Jan 1 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef INVENTORY_ELEMENT_H
#define INVENTORY_ELEMENT_H
#include <iostream>
#include "Element.h"
#include "ItemRule.h"
#include "RulesCollection.h"
#include "Rational.h"
class Parser;
extern RulesCollection <ItemRule>      items;
using namespace std;
typedef Element3<  ItemRule,  Rational ,  int> BasicInventoryElement;

class InventoryElement : public BasicInventoryElement  {
public: 
	InventoryElement(ItemRule * rule, int num, int equip=0)  : BasicInventoryElement (rule,num,equip)
			{	if (equip > num)		parameter2_ = num;}
	InventoryElement(ItemRule * rule, Rational num, int equip=0)  : BasicInventoryElement (rule,num,equip)
			{	if (equip > num)		parameter2_ = num.getValue();}
	~InventoryElement(){}

   inline ItemRule *   getItemType()      const {return rule_;}
   inline int          getItemNumber()         {return parameter1_.getValue();}
   inline void         setItemNumber(int value)     { parameter1_ = value;}
   inline Rational     getRationalItemNumber()         {return parameter1_;}
   inline void         setRationalItemNumber(const Rational& value)     { parameter1_ = value;}
   inline void         setItemType(ItemRule * rule) { rule_ = rule;}
   inline int          getEquipedNumber()    const  {return parameter2_;}
   inline void         setEquipedNumber(int value)  { parameter2_  = value;}

   inline bool isValidElement() const {return ((rule_ != 0) && (parameter1_ != 0));}
   inline int getWeight()  {return rule_->getWeight() * parameter1_.getValue();}
   inline  EntityStatistics * getStats()  {return rule_->getStats();}
   inline int getCapacity(int modeIndex)
    {
          return rule_->getCapacity(modeIndex) * (parameter1_.getValue() -
            parameter2_) + rule_->getEquipCapacity(modeIndex) * parameter2_;
    }
   void save(ostream & out)
   {
      out << rule_->getTag() << " " <<  parameter1_ << " " << parameter2_  << endl;
   }

   void print(ostream & out)
   {
     out<< printName();// << " "; 
     if (parameter2_) out << " (" << parameter2_ <<" equiped)";
   }

   string printName()
   {
    if (rule_ == 0) return "";
    if( parameter1_.getValue() > 1)
      return parameter1_.printName() + " " + rule_->getPluralName()  + rule_->printTag();
    else
      return parameter1_.printName()  + " " + rule_->printName();
   }

	
/*
 * Checks if it is possible to read Inventory Element from the input parser
 */
 
  static InventoryElement  * readElement (Parser * parser)
      {
        ItemRule * item = items[parser->getWord()];
        int number = parser->getInteger();
        int equiped = parser->getInteger();
        if( (item == 0) || (number == 0))
          return 0;
        else
        return new InventoryElement(item, number,equiped);
      }

  protected:
  InventoryElement read(Parser * parser)
{
	return InventoryElement (items[parser->getWord()],
														parser->getRational(),
														parser->getInteger()
														);
}
};
typedef vector <InventoryElement *>::iterator InventoryElementIterator;
//#ifndef INVENTORY_ELEMENT_EXPLICIT_H
//#include "InventoryElement.cpp"
//#endif



#endif
