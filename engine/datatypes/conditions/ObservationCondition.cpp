/***************************************************************************
                          ObservationCondition.cpp  -  description
                             -------------------
    begin                : Sun Jan 26 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "ObservationCondition.h"
#include "UnitEntity.h"
extern ObservationCondition  sampleObservationCondition;
GameData * ObservationCondition::createInstanceOfSelf()
{
   return CREATE_INSTANCE<ObservationCondition> (this);
}

//#include "LocationEntity.h"


bool ObservationCondition::isSatisfied(UnitEntity * unit)
{
//  cout << unit->printName()<< "at "<<unit->getLocation()->printName() << "observing with "<< unit->getObservation() << " vs " <<observationRequired_<<endl;
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


