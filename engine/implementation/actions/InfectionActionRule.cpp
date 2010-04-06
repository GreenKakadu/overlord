/* 
 * File:   InfectionActionRule.cpp
 * Author: alex
 * 
 * Created on January 18, 2010, 12:51 PM
 */

#include "InfectionActionRule.h"
#include "UnitEntity.h"
#include "TertiaryMessage.h"
#include "LocationEntity.h"
#include "PlagueEffect.h"
#include "implementation/effects/PlagueEffect.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "RaceElement.h"

extern ReportPattern * plagueDeathReporter;
extern ReportPattern * plagueDeathReporter2;



InfectionActionRule     sampleInfectionActionRule =      InfectionActionRule("INFECTION_ACTION", &sampleAction);

 InfectionActionRule:: InfectionActionRule(const  InfectionActionRule * prototype )
                    : ActionRule(prototype)
{
    name_ = "Infection Action";
    effectType_ = 0;
}


GameData *  InfectionActionRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE< InfectionActionRule> (this);
}

STATUS
InfectionActionRule::initialize        ( Parser *parser )
{
    ActionRule::initialize(parser );

    if (parser->matchKeyword("EFFECT"))
    {
        effectType_ = dynamic_cast<PlagueEffectRule *>(effectRules[parser->getText()]);
        return OK;
    }
    return OK;
}



ACTION_RESULT  InfectionActionRule::carryOut(Entity * entity,  AbstractData * parameter,  int value)
{
    if(entity->isTraced())
    {
    cout <<"-+-+-+-> "<< entity->print() << " infection processing: value= "<< value<<" -> " <<effectType_->getIllnessDuration()<< " | "<< effectType_->getIncubationPeriod()<<endl;
    }

  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
  LocationEntity * location = unit->getLocation();
  if(location==0)
  {
   cout <<"-+-+-+-> "<< entity->print() << " has no location"<<endl;
   return ACTION_SUCCESS;
  }
  //assert(location);
  int counter;
  // Get duration of infection enchant
  if(value < effectType_->getMaxLength() - effectType_->getIncubationPeriod() )
  {
      counter = 0;
      for(int n=0; n< unit->getFiguresNumber(); n++)
      {
//          int roll= Roll_1Dx(10000);
//          cout<<"Roll: "<<roll<<" vs. "<<((effectType_->getlethality()*100)/gameConfig.daysInMonth)<<endl;
//           if(roll < (effectType_->getlethality()*10)/gameConfig.daysInMonth)
           if(Roll_1Dx(10000)< (effectType_->getlethality()*100)/gameConfig.daysInMonth)
           {
               counter++;
           }
      }
      if(counter)
      {
        if(entity->isTraced())
        {
           cout <<"-+-+-+-> in "<< unit->print()<<" "<<counter <<" figures died from plague"<<endl;
        }
      if(unit->getFiguresNumber()>counter)
       {
        unit->sufferDamage(counter);      
        unit->addReport(new BinaryMessage(plagueDeathReporter2,new RaceElement(unit->getRace(),counter),unit));
       }
       else
       {
          unit->disband();
         unit->addReport(new UnaryMessage(plagueDeathReporter,unit));
         // report
       }
       location->addResource(items["dead"],counter);
       //add dead body to location
      }

   }
    return     ACTION_SUCCESS;
}




