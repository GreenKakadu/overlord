/***************************************************************************
                          TravelElement.h 
                             -------------------
    begin                : Fri Apr 4 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef TRAVEL_ELEMENT_H
#define TRAVEL_ELEMENT_H


/**Contains movement information forstack traveling from one location to another
This element is not based on Element template
  *@author Alex Dribin
  */
#include <iostream>
class LocationEntity;
class MovementVariety;
class Parser;
using namespace std;

class TravelElement {
public: 
	TravelElement(MovementVariety *    movingMode,
                LocationEntity * origin,
                LocationEntity * destination,
                int time, int totalTravelTime);
	virtual ~TravelElement();
  static TravelElement *    readElement      ( Parser *parser);
  virtual void save(ostream & out);
  virtual void print(ostream & out);
   void retreat();
   void advance();
   bool isCrossingBorder();
   bool isArrived();
  inline MovementVariety * getMode() const {return movingMode_;}
         int               getModeIndex() const;
  inline LocationEntity *  getDestination() const {return destination_;}
  inline int               getRemainingTime() const {return time_;}
  inline int               getTravelTime() const {return totalTravelTime_;}
  inline void              resetTravelTime(int time) { totalTravelTime_= time;}
  inline void              resetRemainingTime(int time) { totalTravelTime_= time;}
 protected:
  MovementVariety *    movingMode_;
  LocationEntity * origin_;
  LocationEntity * destination_;
  int time_;
  int totalTravelTime_;
};

#endif
