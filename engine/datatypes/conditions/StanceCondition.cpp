/* 
 * File:   StanceCondition.cpp
 * Author: alex
 * 
 * Created on April 11, 2011, 12:30 PM
 */

#include "StanceCondition.h"
#include "UnitEntity.h"
#include "StanceVariety.h"
#include "entities/TokenEntity.h"
#include "entities/FactionEntity.h"

StanceCondition  sampleStanceCondition ("STANCE_CONDITION", &sampleBasicCondition);
StanceCondition::StanceCondition(const StanceCondition * prototype)
  : BasicCondition(prototype)
{
  stanceRequired_ =0;
}


GameData * StanceCondition::createInstanceOfSelf()
{
   return CREATE_INSTANCE<StanceCondition> (this);
}

//#include "LocationEntity.h"


bool StanceCondition::isSatisfied(TokenEntity * entity, Entity * targetEntity)
{
   TokenEntity * target = dynamic_cast<TokenEntity *>(targetEntity);
   if(target ==0) // May be some stealth Entities would be defined later
   return true;

   if(!stanceRequired_)
   {
     return true;
   }

  if(entity->getFaction()->getStance(target->getFaction()) >= stanceRequired_  )
    return true;
  else
    return false;
}



STATUS StanceCondition::initialize ( Parser *parser )
{
  stanceRequired_ = gameFacade->stances[parser->getWord()];
  return OK;
}



void StanceCondition::save(ostream &out)
{
    if(stanceRequired_)
    {
      out <<"STANCE"<<" "<<stanceRequired_->getTag()<<" ";
    }
}

/** Create observation condition*/
StanceCondition * StanceCondition::createStanceCondition(StanceVariety * stance)
{
 StanceCondition * criteria = new StanceCondition(&sampleStanceCondition);
 criteria->stanceRequired_ = stance;
 return criteria;
}



