/***************************************************************************
                          BasicProductionStrategy.cpp 
                             -------------------
    begin                : Thu Sep 18 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "BasicProductionStrategy.h"
#include "ItemElement.h"
#include "SkillUseElement.h"
#include "ItemRule.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "RulesCollection.h"
#include "ToolUseElement.h"
#include "BinaryPattern.h"
extern Reporter * notEnoughResourcesReporter;
extern Reporter * productionReporter;

extern RulesCollection    <ItemRule>     items;

BasicProductionStrategy::BasicProductionStrategy ( const BasicProductionStrategy * prototype ): BasicUsingStrategy(prototype)
{
  productNumber_ = 1;
}



GameData * BasicProductionStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<BasicProductionStrategy> (this);
}



STATUS
BasicProductionStrategy::initialize        ( Parser *parser )
{
      return OK;
}





void    BasicProductionStrategy::extractKnowledge (Entity * recipient, int parameter)
{
}



void BasicProductionStrategy::reportUse(USING_RESULT result, PhysicalEntity * unit)
{
}



int BasicProductionStrategy::checkResourcesAvailability(PhysicalEntity * unit)
{
  int currentProductionCycles;
  int maxProductionCycles = 9999;  // very big number
  for(vector <ItemElement *>::iterator iter = resources_.begin(); iter != resources_.end(); ++iter)
    {
      currentProductionCycles = unit->hasItem((*iter)->getItemType()) /(*iter)->getItemNumber();
      if(currentProductionCycles == 0)
        return 0;
      if(iter == resources_.begin())
         maxProductionCycles = currentProductionCycles;
      else
      {
        if ( currentProductionCycles < maxProductionCycles)
           maxProductionCycles = currentProductionCycles;
      }
    }
   return  maxProductionCycles;
}



bool BasicProductionStrategy::consumeResources(PhysicalEntity * unit, int numCycles)
{
  for(vector <ItemElement *>::iterator iter = resources_.begin(); iter != resources_.end(); ++iter)
    {
      if(!unit->takeFromInventoryExactly ((*iter)->getItemType(),(*iter)->getItemNumber() * numCycles ))
          return false;
    }
      return true;
}



void BasicProductionStrategy::printSkillDescription(ostream & out)
{
 if(resources_.empty())
    return;
 else
 out << " Use requires: " ;
 for(vector <ItemElement *>::iterator iter = resources_.begin();
     iter != resources_.end(); ++iter )
     {
       if(iter != resources_.begin())
        out<<", ";
       out<< (*iter)->printName();
     }
  out<<".";
}