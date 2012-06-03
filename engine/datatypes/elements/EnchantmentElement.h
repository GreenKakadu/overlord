/***************************************************************************
                          EnchantmentElement.h
                             -------------------
    begin                : Tue Jan 27 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#ifndef ENCHANTMENT_ELEMENT_H
#define ENCHANTMENT_ELEMENT_H
#include "Element.h"
#include "EnchantmentRule.h"
#include "IntegerData.h"
#include "StringData.h"

using namespace std;
/* Operator new defined in Element2 and produces Element2 object and not derivative
 * use constructors to create EnchantmentElement
*/
/**
  *@author Alex Dribin
  */
typedef  Element2<EnchantmentRule,int> BasicEnchantmentElement;

class EnchantmentElement : public BasicEnchantmentElement  {
public:
	EnchantmentElement(EnchantmentRule * effect,int days):BasicEnchantmentElement(effect,days){}
	~EnchantmentElement(){}
static EnchantmentElement  * readElement (Parser * parser);
static EnchantmentElement  * createElement (EnchantmentRule * effect,int days);
   //static void * operator new(size_t size);
   //static void * operator new(size_t, void * element);  // Placement form of new
   //static void   operator delete(void * deadObject, size_t size);
//static EnchantmentElement readItemElement(Parser * parser);
/*
 * returns string with textual representation for reports and debuggers
 * Examples: "5 days of flying"
 */

string print() const
{
    if (rule_ == 0) return "";
     if( parameter1_ > 1)
      //return longtostr(parameter1_) + " " + rule_->getPluralName()  + rule_->printTag();
       return longtostr(parameter1_) + " " + rule_->getPluralName();
   else
      return longtostr(parameter1_) + " " + rule_->getName();
    //return longtostr(parameter1_) + " " + rule_->print();
}

//should provide non-const print too (or redefine all prints as const)
string print()
{
  if (rule_ == 0) return "";
     if( parameter1_ > 1)
      return longtostr(parameter1_) + " " + rule_->getPluralName()  /*+ rule_->printTag()*/;
    else
      return longtostr(parameter1_) + " " + rule_->getName() /*+ rule_->print()*/;

}
// Printing as string array for QT
vector <AbstractData *> aPrint()
{
  vector <AbstractData *> v;
  v.push_back(new IntegerData(getParameter1()));
  v.push_back(new StringData(" "));
  v.push_back(getRule());
  return v;
}
vector <AbstractData *> aPrintEnchant()
{

    vector <AbstractData *> v;
    v.push_back(new IntegerData(getParameter1()));
    if(parameter1_ > 1)
    {
        v.push_back(new StringData(" days of "));
    }
    else
    {
        v.push_back(new StringData(" day of "));
    }
    v.push_back(getRule());
    //v.push_back(this);
    return v;
}

};
// void * EnchantmentElement::operator new (size_t size)
//{
//	if(size != sizeof(EnchantmentElement ) )
//		return ::operator new(size);
//
//	EnchantmentElement  * p =  EnchantmentElement::headOfFreeList;
//	if(p)
//			EnchantmentElement::headOfFreeList = p->next;
//	else
//		{
//			EnchantmentElement  * newBlock = static_cast<EnchantmentElement  *>
//							(:: operator new ( BLOCK_SIZE * sizeof (EnchantmentElement ) ) );
//			for (int i = 1; i < BLOCK_SIZE - 1 ; ++i)
//				{
//					newBlock[i].next = &newBlock[i+1];
//				}
//			newBlock[BLOCK_SIZE - 1 ].next = 0;
//			p = newBlock;
//			EnchantmentElement ::headOfFreeList = &newBlock[1];
//		}
//	return p;
//}
// void * EnchantmentElement::operator new(size_t, void * element)
//{
// return  element;
//}
// void  EnchantmentElement::operator delete (void * deadObject, size_t size)
//{
// if(deadObject == 0) return;
// if(size != sizeof(EnchantmentElement ) )
//		{
//			::operator delete(deadObject);
//			return;
//		}
//	EnchantmentElement  * p = static_cast<EnchantmentElement  *> (deadObject);
//	p->next = EnchantmentElement ::headOfFreeList;
//	EnchantmentElement ::headOfFreeList = p;
//}
#endif
