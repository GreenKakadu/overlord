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
//class ItemRule;
typedef Element3<  ItemRule,  Rational ,  int> BasicInventoryElement;

class InventoryElement : public BasicInventoryElement  {
public: 
	InventoryElement(ItemRule * rule, int num, int equip=0)  : BasicInventoryElement (rule,num,equip)
			{	if (equip > num)		parameter2_ = num;}
	~InventoryElement(){}

  /** No descriptions */
   void save(ostream & out)
   {out << rule_->getTag() << " " <<  parameter1_ << " " << parameter2_  << endl;}

   void print(ostream & out)
   {
     out<< printName();// << " "; 
     if (parameter2_) out << " (" << parameter2_ <<" equiped)";
   }

   string printName()
   {
    if (rule_ == 0) return "";
//    char buffer[12];
//      longtostr(parameter1_,buffer);
////		  sprintf(buffer,"%d",parameter1_);// May use hand-made convertor itoa
    if( parameter1_.getValue() > 1)
      return parameter1_.printName() /*+ " "*/ + rule_->getPluralName()  + rule_->printTag();
    else
      return parameter1_.printName()  /*+ " " */+ rule_->printName();
  }

  /** No descriptions */
  inline int getWeight()  {return rule_->getWeight() * parameter1_.getValue();}
   inline ItemRule *   getItemType()      const {return rule_;}
   inline int          getItemNumber()         {return parameter1_.getValue();}
   inline void         setItemNumber(int value)     { parameter1_ = value;}
   inline Rational     getRationalItemNumber()         {return parameter1_;}
   inline void         setRationalItemNumber(const Rational& value)     { parameter1_ = value;}
   inline void         setItemType(ItemRule * rule) { rule_ = rule;}
   inline int          getEquipedNumber()    const  {return parameter2_;}
   inline void         setEquipedNumber(int value)  { parameter2_  = value;}
   inline  EntityStatistics * getStats()  {return rule_->getStats();}
  inline int getCapacity(int modeIndex)
  {return rule_->getCapacity(modeIndex) * (parameter1_.getValue() - parameter2_) + rule_->getEquipCapacity(modeIndex) * parameter2_;}
	
	protected:
  /** Checks if it is possible to read Inventory Element from the input parser */
  InventoryElement read(Parser * parser)
{
	return InventoryElement (items[parser->getWord()],
														parser->getInteger(),
														parser->getInteger()
														);
}

};
typedef vector <InventoryElement >::iterator InventoryElementIterator;
//#ifndef INVENTORY_ELEMENT_EXPLICIT_H
//#include "InventoryElement.cpp"
//#endif


//InventoryElement InventoryElement::read(Parser * parser)
//{
//	return InventoryElement (items[parser->getWord()],
//														parser->getInteger(),
//														parser->getInteger()
//														);
//}
#endif
