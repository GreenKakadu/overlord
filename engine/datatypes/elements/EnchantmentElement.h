/***************************************************************************
                          EnchantmentElement.h
                             -------------------
    begin                : Tue Jan 27 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
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
#include "RulesCollection.h"
//#include "BasicEnchantmentElement.h"
using namespace std;
extern RulesCollection <EnchantmentRule>     enchantments;

/**
  *@author Alex Dribin
  */
typedef  Element2<EnchantmentRule,int> BasicEnchantmentElement;

class EnchantmentElement : public BasicEnchantmentElement  {
public:
	EnchantmentElement(EnchantmentRule * effect,int days):BasicEnchantmentElement(effect,days){}
	~EnchantmentElement(){}
static EnchantmentElement  * readElement (Parser * parser);
//static EnchantmentElement readItemElement(Parser * parser);
};

#endif
