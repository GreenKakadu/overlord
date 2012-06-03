/***************************************************************************
                          ObservationCondition.cpp  -  description
                             -------------------
    begin                : Sun Jan 26 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "ObservationCondition.h"
#include "UnitEntity.h"

ObservationCondition  sampleObservationCondition ("OBSERVATION_CONDITION", &sampleBasicCondition);
ObservationCondition::ObservationCondition(const ObservationCondition * prototype)
  : BasicCondition(prototype)
{
  observationRequired_ =0;
}


GameData * ObservationCondition::createInstanceOfSelf()
{
   return CREATE_INSTANCE<ObservationCondition> (this);
}

//#include "LocationEntity.h"


bool ObservationCondition::isSatisfied(TokenEntity * entity, Entity * target)
{
//  cout << unit->print()<< "at "<<unit->getLocation()->print() << "observing with "<< unit->getObservation() << " vs " <<observationRequired_<<endl;
   UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
   if(unit==0) // Observation may be later defined also for buildings
    return false;
   UnitEntity * targetUnit = dynamic_cast<UnitEntity *>(target);
   if(targetUnit ==0) // May be some stealth Entities would be defined later
   return true;



  if(unit->getObservation() >= targetUnit->getStealth() + observationRequired_ )
    return true;
  else
    return false;  
}



STATUS ObservationCondition::initialize ( Parser *parser )
{

  observationRequired_ = parser->getInteger();
  return OK;
}



void ObservationCondition::save(ostream &out)
{
    out <<observationRequired_<<endl;
}

/** Create observation condition*/
ObservationCondition * ObservationCondition::createObservationCondition(int observationLevel)
{
 ObservationCondition * criteria = new ObservationCondition(&sampleObservationCondition);
 criteria->setObservationCondition(observationLevel);
 return criteria;
}


