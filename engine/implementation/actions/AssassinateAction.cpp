/* 
 * File:   AssassinateAction.cpp
 * Author: alex
 * 
 * Created on February 15, 2010, 9:14 PM
 */

#include "AssassinateAction.h"
#include "TokenEntity.h"
#include "LocationEntity.h"
#include "FactionEntity.h"
#include "UnitEntity.h"
#include "ConstructionEntity.h"
#include "SimpleMessage.h"
#include "BinaryMessage.h"
#include "UnaryMessage.h"

extern int roll_1Dx(int);
extern ReportPattern * assassineEscapededReport2;
extern ReportPattern * assassineEscapededReport;
extern ReportPattern * assassineCapturedReport3;
extern ReportPattern * assassineCapturedReport2;
extern ReportPattern * assassineCapturedReport;
extern ReportPattern * assassinationSuccessReport;
extern ReportPattern * assassinationSuccessReport2;
extern ReportPattern * assassinationSuccessReport3;
extern ReportPattern * assassinationSuccessReport4;
extern ReportPattern * assassinNoTargetReport;

AssassinateAction     sampleAssassinateAction =     AssassinateAction("ASSASSINATE_ACTION", &sampleAction);

AssassinateAction::AssassinateAction(const AssassinateAction * prototype )
                    : ActionRule(prototype)
{
    name_ = "Assassinate Action";
}


GameData * AssassinateAction::createInstanceOfSelf()
{
  return CREATE_INSTANCE<AssassinateAction> (this);
}




ACTION_RESULT AssassinateAction::carryOut(Entity * entity, AbstractData * parameter,   int value)
{
  TokenEntity * assassin = dynamic_cast<TokenEntity *>(entity);
  assert(assassin);
  TokenEntity * target= dynamic_cast<TokenEntity *>(assassin->getTarget());
  if(target==0)
  {
      return ACTION_FAILURE;
  }

  cout << "Assassination Attempt: " <<assassin << " killing " <<target <<endl;

  if(assassin->getGlobalLocation() == 0 || assassin->getGlobalLocation() != target->getGlobalLocation())
  {
     assassin->addReport(new UnaryMessage(assassinNoTargetReport, target));
    return ACTION_FAILURE;
  }
  LocationEntity * location =assassin->getGlobalLocation();
  UnitIterator iter;
  // Calculate observation score OS for all allied units and buildings
  // AS = sum((obse+1)*num figures)
  int observationScore = 0;
  int bestObservation=0; // Victim not counted in because it may be killed
  //int bestButVictimObservation = 0;
  for(iter = location->unitsPresent().begin(); iter != location->unitsPresent().end(); ++iter)
  {
      if ((*iter)==assassin)
          continue;
     if((*iter)->getFaction()->getStance(target)== alliedStance)
     {
        observationScore += (*iter)->getObservationRating();
      if ((*iter)==target)// Victim not counted in because it may be killed
          continue;
        if((*iter)->getObservation() > bestObservation)
        {
            bestObservation = (*iter)->getObservation();
        }
     }
  }
  // Constructions may contribute to observationScore

  FactionEntity * faction = 0;
  for(ConstructionIterator iter = location->constructionsPresent().begin(); iter != location->constructionsPresent().end(); ++iter)
  {
      faction=(*iter)->getFaction();
      if(faction ==0)
          continue;
     if(faction->getStance(target)== alliedStance)
     {
        observationScore += (*iter)->getObservationRating();
     }
  }
// Calculate Assasine score AS (Stealth, Assa skill level)
  int assassineScore = assassin->getStealthRating();
  bool isAssassineDetectable =false;

  if(assassin->getStealth() < bestObservation)
  {
     isAssassineDetectable = true;
  }

  // chance = AS/(AS + OS)
  int chance = roll_1Dx(observationScore + assassineScore);
  if(chance < assassineScore)
  {
      //Success
      if(isAssassineDetectable)
      {
        location->addReport(new UnaryMessage(assassinationSuccessReport,target));
        target->addReport(new SimpleMessage(assassinationSuccessReport4));
      }
      else
       {
         target->addReport(new UnaryMessage(assassinationSuccessReport,assassin));
         location->addReport(new BinaryMessage(assassinationSuccessReport2,
                target,assassin));
//        location->addReport(new UnaryMessage(assassinationSuccessReport2,
//                target,assassin),0,
//                ObservationCondition::createObservationCondition(assassin->getStealth()));
      }

      target->disband();
      // But assassin still may be captured but only if may be seen
    if(isAssassineDetectable)
      {
          int chance = roll_1Dx(observationScore + assassineScore);
          if(chance < assassineScore)
          {
            location->addReport(new UnaryMessage(assassineEscapededReport,assassin));
         }
          else
          {
            location->addReport(new UnaryMessage(assassineCapturedReport,assassin));
            assassin->addReport(new SimpleMessage(assassineCapturedReport3));
            assassin->disband();
         }
    }
  }
  else //Failure
  {
       if(target->getObservation() > bestObservation)// Target survived
        {
            bestObservation = target->getObservation();
        }
   //chance to escape (only if unseen) AS/(AS + OS)
    if(assassin->getStealth() > bestObservation)
      {
          int chance = roll_1Dx(observationScore + assassineScore);
          if(chance < assassineScore)
          {
            location->addReport(new SimpleMessage(assassineEscapededReport2));
          }
          else
          {
            assassin->addReport(new SimpleMessage(assassineCapturedReport3));
            location->addReport(new SimpleMessage(assassineCapturedReport2));
            assassin->disband();
         }

      }
  }
    return     ACTION_SUCCESS;
}


