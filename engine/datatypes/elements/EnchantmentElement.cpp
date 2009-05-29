/***************************************************************************
                          EnchantmentElement.cpp 
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
#include "EnchantmentElement.h"
#include "EnchantmentRule.h"


EnchantmentElement  * EnchantmentElement::readElement (Parser * parser)
{
  EnchantmentRule * enchantment = enchantments[parser->getWord()];
  int number = parser->getInteger();
  if( (enchantment == 0) || (number == 0))
          return 0;
  else
        return new EnchantmentElement(enchantment, number);
}



//EnchantmentElement EnchantmentElement::readItemElement(Parser * parser)
//{
//  EnchantmentRule * enchantment = enchantments[parser->getWord()];
//  int number = parser->getInteger();
//  if( (enchantment == 0) || (number == 0))
//          return 0;
//  else
//        return  EnchantmentElement(enchantment, number);
//}


