/***************************************************************************
                          EnchantmentAttribute.cpp 
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

EnchantmentAttribute::EnchantmentAttribute(){
dataCollection_.clear();
}
EnchantmentAttribute::~EnchantmentAttribute(){
}



STATUS
EnchantmentAttribute::initialize        ( Parser *parser )
{


  if (parser->matchKeyword ("FX_EFFECT") )
    {
      EnchantmentRule * enchantment = enchantments[parser->getWord()];
      int number = parser->getInteger();
      if( (enchantment == 0) || (number == 0))
          return OK;
      else
      {
         dataCollection_.push_back(EnchantmentElement(enchantment, number));
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



void EnchantmentAttribute::save(ostream &out)
{
  for (EnchantmentAttributesIterator  iter = dataCollection_.begin();
    iter != dataCollection_.end();  ++iter)
    {
      out << "FX_EFFECT " <<  ((*iter).getRule())->getTag() << " " << (*iter).getParameter1()<<endl;
    }
}



ostream&  EnchantmentAttribute::report(ostream &out)
{
  return out;
}



void EnchantmentAttribute::add(EnchantmentElement * data)
{
  for (EnchantmentAttributesIterator  iter = dataCollection_.begin();
    iter != dataCollection_.end();  ++iter)
    {
//      cout <<"----- Adding Enchantment "<< *(data->getRule()) << " " << data->getParameter1() << endl;
      if ( (*iter).getRule() == data->getRule() )
        {

           (*iter).setParameter1((*iter).getParameter1() + data->getParameter1()) ;
//            delete data;
            return;
        }
    }
    dataCollection_.push_back(*data);

}



void EnchantmentAttribute::remove(EnchantmentElement * data)
{
  for (EnchantmentAttributesIterator  iter = dataCollection_.begin();
    iter != dataCollection_.end();  ++iter)
    {
      if ( (*iter).getRule() == data->getRule() )
        {
          if( (*iter).getParameter1() > data->getParameter1())
          {
           (*iter).setParameter1((*iter).getParameter1() - data->getParameter1()) ;
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



EnchantmentElement * EnchantmentAttribute::has( EnchantmentElement * data)
{
  for (EnchantmentAttributesIterator  iter = dataCollection_.begin();
    iter != dataCollection_.end();  ++iter)
    {
      if ( (*iter).getRule() == data->getRule() )
      return (&(*iter));
    }
  return 0;
}



EnchantmentElement * EnchantmentAttribute::findAndDo(EnchantmentElement * data, EnchantmentElement * (*toDo) ())
{
  for (EnchantmentAttributesIterator  iter = dataCollection_.begin();
    iter != dataCollection_.end();  ++iter)
    {
      if ( (*iter).getRule() == data->getRule() )
      {
      return(*toDo) ();
      }
    }
  return 0;
}


// Get stats attribute as parameter and modifies it
void EnchantmentAttribute::addStats(EntityStatistics * stats, int figuresNumber = 1 )
{
  for (EnchantmentAttributesIterator  iter = dataCollection_.begin();
    iter != dataCollection_.end();  ++iter)
    {
      if( (*iter).getParameter1() <= figuresNumber)
      // Enchantment is not strong enough to cover all figures
      {
        stats->addPartialStats((*iter).getRule()->getStats(),(*iter).getParameter1(), figuresNumber);
      }
      else
        stats->addStats((*iter).getRule()->getStats());
    }
}



int EnchantmentAttribute::getCapacity(int modeIndex, int figuresNumber = 1)
{
  int capacity = 0;
  for (EnchantmentAttributesIterator  iter = dataCollection_.begin();
    iter != dataCollection_.end();  ++iter)
    {
      if( (*iter).getParameter1() <= figuresNumber)
      // Enchantment is not strong enough to cover all fugures
      {
        capacity +=  ((*iter).getRule()->getCapacity(modeIndex)) *  (*iter).getParameter1() ;
      }
      else
      {
        capacity +=  ((*iter).getRule()->getCapacity(modeIndex)) *  figuresNumber ;
      }
    }
    return capacity;
}



int EnchantmentAttribute::getCapacity(MovementVariety * mode, int figuresNumber = 1)
{
  int capacity = 0;
  for (EnchantmentAttributesIterator  iter = dataCollection_.begin();
    iter != dataCollection_.end();  ++iter)
    {
      if( (*iter).getParameter1() <= figuresNumber)
      // Enchantment is not strong enough to cover all fugures
      {
        capacity +=  ((*iter).getRule()->getCapacity(mode)) *  (*iter).getParameter1() ;
      }
      else
      {
        capacity +=  ((*iter).getRule()->getCapacity(mode)) *  figuresNumber ;
      }
    }
    return capacity;
}



void EnchantmentAttribute::processExpiration(int figuresNumber)
{
  for (EnchantmentAttributesIterator  iter = dataCollection_.begin();
    iter != dataCollection_.end();)
    {
//      cout <<"----- Expiration of Enchantment "<< *((*iter)->getRule()) << " " << (*iter)->getParameter1() << endl;
      if( (*iter).getParameter1() <= figuresNumber) // Enchantment expired and deleted
      {
            //delete *iter ;
            dataCollection_.erase(iter);
      } 
      else
      {
         (*iter).setParameter1((*iter).getParameter1() - figuresNumber);
           ++iter;
      }
    }
}




int EnchantmentAttribute::getProductionBonus(SkillRule * skill)
{
	int bonus = 0;
	for (EnchantmentAttributesIterator  iter = dataCollection_.begin();
    iter != dataCollection_.end();++iter)
    {
			bonus += (*iter).getRule()->getProductionBonusValue(skill);
		}
	return bonus;
}


int EnchantmentAttribute::getStudyBonus(SkillRule * skill)
{
	int bonus = 0;
	for (EnchantmentAttributesIterator  iter = dataCollection_.begin();
    iter != dataCollection_.end();++iter)
    {
			bonus += (*iter).getRule()->getStudyBonus(skill);
		}
	return bonus;
}


int EnchantmentAttribute::getLearningBonus(SkillRule * skill)
{
	int bonus = 0;
	int current= 0;
	for (EnchantmentAttributesIterator  iter = dataCollection_.begin();
    iter != dataCollection_.end();++iter)
    {
			current = (*iter).getRule()->getLearningBonus(skill);
			if (current > bonus)
			bonus = current;
		}

	return bonus;
}



int EnchantmentAttribute::getMovementBonus(MovementVariety *    mode)
{
	int bonus = 0;
	int current= 0;
	for (EnchantmentAttributesIterator  iter = dataCollection_.begin();
    iter != dataCollection_.end();++iter)
    {
			current = (*iter).getRule()->getMovementBonus(mode);
		}

	return bonus;
}



void EnchantmentAttribute::removeAll()
{
 dataCollection_.clear();
}
