/***************************************************************************
                          EnchantmentsAttribute.cpp 
                             -------------------
    begin                : Thu Mar 11 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "EnchantmentAttribute.h"
#include "EnchantmentElement.h"
#include "RulesCollection.h"
#include "EnchantmentRule.h"
#include "EntityStatistics.h"

extern RulesCollection <EnchantmentRule>    enchantments;

EnchantmentsAttribute::EnchantmentsAttribute(){
}
EnchantmentsAttribute::~EnchantmentsAttribute(){
}



STATUS
EnchantmentsAttribute::initialize        ( Parser *parser )
{


  if (parser->matchKeyword ("FX_EFFECT") )
    {
      EnchantmentRule * enchantment = enchantments[parser->getWord()];
      int number = parser->getInteger();
      if( (enchantment == 0) || (number == 0))
          return OK;
      else
      {
         dataCollection_.push_back(new EnchantmentElement(enchantment, number));
//      cout <<"----- Created Enchantment "<< enchantment << " " << number << endl;
       }
//      EnchantmentElement * enchantment = EnchantmentElement::readElement(parser);
//
//      if(enchantment)
//      {
//        dataCollection_.push_back(enchantment);
//       }
      return OK;
    }

      return OK;

 }



void EnchantmentsAttribute::save(ostream &out)
{
  for (EnchantmentAttributesIterator  iter = dataCollection_.begin();
    iter != dataCollection_.end();  ++iter)
    {
      out << "FX_EFFECT " <<  ((*iter)->getRule())->getTag() << " " << (*iter)->getParameter1()<<endl; 
    }
}



ostream&  EnchantmentsAttribute::report(ostream &out)
{
  return out;
}



void EnchantmentsAttribute::add(EnchantmentElement * data)
{
  for (EnchantmentAttributesIterator  iter = dataCollection_.begin();
    iter != dataCollection_.end();  ++iter)
    {
//      cout <<"----- Adding Enchantment "<< *(data->getRule()) << " " << data->getParameter1() << endl;
      if ( (*iter)->getRule() == data->getRule() )
        {

            (*iter)->setParameter1((*iter)->getParameter1() + data->getParameter1()) ;
//            delete data;
            return;
        }    
    }
    dataCollection_.push_back(data);

}



void EnchantmentsAttribute::remove(EnchantmentElement * data)
{
  for (EnchantmentAttributesIterator  iter = dataCollection_.begin();
    iter != dataCollection_.end();  ++iter)
    {
      if ( (*iter)->getRule() == data->getRule() )
        {
          if( (*iter)->getParameter1() > data->getParameter1())
          {
            (*iter)->setParameter1((*iter)->getParameter1() - data->getParameter1()) ;
//            delete data;
            return;
          }
          else
          {
            dataCollection_.erase(iter);
            return;
          }
        }

    }


}



EnchantmentElement * EnchantmentsAttribute::has( EnchantmentElement * data)
{
  for (EnchantmentAttributesIterator  iter = dataCollection_.begin();
    iter != dataCollection_.end();  ++iter)
    {
      if ( (*iter)->getRule() == data->getRule() )
      return (*iter);
    }
  return 0;
}



EnchantmentElement * EnchantmentsAttribute::findAndDo(EnchantmentElement * data, EnchantmentElement * (*toDo) ())
{
  for (EnchantmentAttributesIterator  iter = dataCollection_.begin();
    iter != dataCollection_.end();  ++iter)
    {
      if ( (*iter)->getRule() == data->getRule() )
      {
      return(*toDo) ();
      }
    }
  return 0;
}



void EnchantmentsAttribute::addStats(EntityStatistics * stats, int figuresNumber = 1 )
{
  for (EnchantmentAttributesIterator  iter = dataCollection_.begin();
    iter != dataCollection_.end();  ++iter)
    {
      if( (*iter)->getParameter1() <= figuresNumber)
      // Enchantment is not strong enough to cover all fugures
      {
        stats->addPartialStats((*iter)->getRule()->getStats(),(*iter)->getParameter1(), figuresNumber);
      }
      else
        stats->addStats((*iter)->getRule()->getStats());
    }
}



int EnchantmentsAttribute::getCapacity(int modeIndex, int figuresNumber = 1)
{
  int capacity = 0;
  for (EnchantmentAttributesIterator  iter = dataCollection_.begin();
    iter != dataCollection_.end();  ++iter)
    {
      if( (*iter)->getParameter1() <= figuresNumber)
      // Enchantment is not strong enough to cover all fugures
      {
        capacity +=  ((*iter)->getRule()->getCapacity(modeIndex)) *  (*iter)->getParameter1() ;
      }
      else
      {
        capacity +=  ((*iter)->getRule()->getCapacity(modeIndex)) *  figuresNumber ;
      } 
    }
    return capacity;
}



void EnchantmentsAttribute::processExpiration(int figuresNumber = 1)
{
  for (EnchantmentAttributesIterator  iter = dataCollection_.begin();
    iter != dataCollection_.end();)
    {
//      cout <<"----- Expiration of Enchantment "<< *((*iter)->getRule()) << " " << (*iter)->getParameter1() << endl;
      if( (*iter)->getParameter1() <= figuresNumber) // Enchantment expired and deleted
      {
            //delete *iter ;
            dataCollection_.erase(iter);
      } 
      else
      {
         (*iter)->setParameter1((*iter)->getParameter1() - figuresNumber);
           ++iter;
      }
    }
}
