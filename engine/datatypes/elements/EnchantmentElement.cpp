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
#include <stdlib.h>
#include "GameFacade.h"
#include "EnchantmentElement.h"
#include "EnchantmentRule.h"


EnchantmentElement  * EnchantmentElement::readElement (Parser * parser)
{
  EnchantmentRule * enchantment = gameFacade->enchantments[parser->getWord()];
  int number = parser->getInteger();
  if( (enchantment == 0) || (number == 0))
          return 0;
  else
        return new EnchantmentElement(enchantment, number);
}

EnchantmentElement  * EnchantmentElement::createElement (EnchantmentRule * effect,int days)
{
  EnchantmentElement element = EnchantmentElement(effect,days);
  void * ptr = malloc(sizeof(element));
  EnchantmentElement * pElement = static_cast<EnchantmentElement *>(ptr);
  memcpy(&element,pElement,sizeof(element));
  return pElement;
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

