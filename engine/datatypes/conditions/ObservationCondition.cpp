/***************************************************************************
                          ObservationCondition.cpp  -  description
                             -------------------
    begin                : Sun Jan 26 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "ObservationCondition.h"
#include "UnitEntity.h"
//ObservationCondition  sampleObservationCondition ("OBSERVATION_CONDITION", &sampleGameData);


GameData * ObservationCondition::createInstanceOfSelf()
{
   return CREATE_INSTANCE<ObservationCondition> (this);
}

//#include "LocationEntity.h"


bool ObservationCondition::isSatisfied(PhysicalEntity * entity)
{
//  cout << unit->printName()<< "at "<<unit->getLocation()->printName() << "observing with "<< unit->getObservation() << " vs " <<observationRequired_<<endl;
   UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
   if(unit==0)
    return false;
  if(unit->getObservation() >= observationRequired_)
    return true;
  else
    return false;  
//  return (unit->getObservation() >= observationRequired_);
}



STATUS ObservationCondition::initialize ( Parser *parser )
{
  observationRequired_ = parser->getInteger();
  return OK;
}

/** Create observation condition*/
ObservationCondition * ObservationCondition::createObservationCondition(int observationLevel)
{
 ObservationCondition * criteria = new ObservationCondition(&sampleObservationCondition);
 criteria->setObservationCondition(observationLevel);
 return criteria;
}


