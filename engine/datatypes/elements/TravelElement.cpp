/***************************************************************************
                          TravelElement.cpp 
                             -------------------
    begin                : Fri Apr 4 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "TravelElement.h"
#include "misc/Parser.h"
#include "entities/LocationEntity.h"
#include "varieties/MovementVariety.h"
#include "collections/EntitiesCollection.h"
#include "collections/VarietiesCollection.h"
extern EntitiesCollection <LocationEntity> locations;
extern VarietiesCollection <MovementVariety> movementModes;

TravelElement::TravelElement(MovementVariety *    movingMode,
                  LocationEntity * origin,
                  LocationEntity * destination,
                  int time, int totalTravelTime)
{
   movingMode_ = movingMode;
   origin_  =    origin;
   destination_ = destination;
   time_ =   time;
   totalTravelTime_ = totalTravelTime;
}



TravelElement::~TravelElement(){
}



TravelElement *    TravelElement::readElement( Parser *parser)
{
  MovementVariety *    movingMode = movementModes[parser->getWord()];
  if(movingMode == 0)
      return 0;
  LocationEntity * origin = locations[parser->getWord()];
 if(origin == 0)
      return 0;

  LocationEntity * destination = locations[parser->getWord()];
 if(destination == 0)
      return 0;
  int time = parser->getInteger();
  if(time == 0)
      return 0;
  int totalTravelTime = parser->getInteger();
  if(totalTravelTime == 0)
      return 0;
  return (new TravelElement(movingMode,origin,destination,
  time, totalTravelTime));
}



void TravelElement::save(ostream & out)
{
  out << "MOVING "<<movingMode_->getTag() << " " << origin_->getTag();
  out << " "<< destination_->getTag() << " " <<time_ << " ";
  out << totalTravelTime_ << ".\n";
  
}



void TravelElement::reportTravel(ostream & out)
{
  out << movingMode_->getName() << " " << origin_->print(); 
  out << " to "<< destination_->print() << " " <<time_ << " days from ";
  out << totalTravelTime_ << ".\n";
}



void TravelElement::retreat()
{
  LocationEntity * temp = origin_;
  origin_ = destination_;
  destination_ = temp;
  time_ = totalTravelTime_ - time_;
}


void TravelElement::advance()
{
 time_ --;
}



bool TravelElement::isCrossingBorder()
{
 return( time_ == totalTravelTime_/2);
}



bool TravelElement::isArrived()
{
 if (time_)
  return false;
 else
  return true;
  
}



int  TravelElement::getModeIndex() const
{
    int i;
    for( i = 0; i< movementModes.size();i++)
    {
       if(movementModes[i] == movingMode_)
        return i;
      }
      return -1;
}
