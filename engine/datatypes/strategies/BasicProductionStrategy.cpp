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
extern GameData  targetTypeSelf;

extern RulesCollection    <ItemRule>     items;

BasicProductionStrategy::BasicProductionStrategy ( const BasicProductionStrategy * prototype ): BasicUsingStrategy(prototype)
{
  productNumber_ = 1;
  productionDays_ =1;
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



void BasicProductionStrategy::reportUse(USING_RESULT result, TokenEntity * unit)
{
}



USING_RESULT BasicProductionStrategy::unitMayUse(UnitEntity * unit, SkillRule * skill)
{
/** We may be in the middle of production. Then we anyway may continue.*/
/** Otherwise we'll need resources */

 if (!unit->isCurrentlyUsingSkill(skill)) // beginning of production cycle
  {
    if(checkResourcesAvailability(unit) == 0)
      {
        return NO_RESOURCES;
      }
     else
     {
        consumeResources(unit,1);
        return  USING_OK;
     }
  }
    return  USING_OK;
}


int BasicProductionStrategy::checkResourcesAvailability(TokenEntity * unit)
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



bool BasicProductionStrategy::consumeResources(TokenEntity * unit, int numCycles)
{
  for(vector <ItemElement *>::iterator iter = resources_.begin(); iter != resources_.end(); ++iter)
    {
      if((*iter)->getItemNumber() * numCycles != unit->takeFromInventory ((*iter)->getItemType(),(*iter)->getItemNumber() * numCycles ))
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
       out<< (*iter)->print();
     }
  out<<".";
}



USING_RESULT  BasicProductionStrategy::checkTarget(UnitEntity * unit, GameData * targetType)
{
   AbstractData * target = unit->getTarget();
// check target type
  // if targetType = SELF any target will work
   if(targetType == &targetTypeSelf)
      return USING_OK;


   if(target==0)             // we may try to find default target
       return NO_TARGET;    //  such as containning construction

  NewEntityPlaceholder * placeholder = dynamic_cast<NewEntityPlaceholder *>(target);
  if(placeholder != 0)  // this is  placeholder.
    {
      GameData* realEntity = placeholder->getRealEntity();
      if(realEntity) // We can get real entity id from placeholder
        {
            if(realEntity->isDescendantFrom(targetType))
              return USING_OK;
            else
              return WRONG_TARGET;
        }
      else   // placeholder is still unresolved. Skill can't be used
              return TARGET_NOT_EXIST;
    }

  //  or some GameData (Entity or rule)  check
  GameData * dataTarget = dynamic_cast<GameData *>(target);
  assert(dataTarget);
  if(dataTarget->isDescendantFrom(targetType))
    return USING_OK;
  else
    return WRONG_TARGET;

}
