/***************************************************************************
                          BasicExit.h
                             -------------------
    begin                : Sun Jan 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef BASIC_EXIT_H
#define BASIC_EXIT_H
#include <iostream>
#include <vector>
#include "BasicData.h"
#include "MovementElement.h"

using namespace std;
class LocationEntity;
class DirectionVariety;
//class MovementRule;
class UnitEntity;
class MovementVariety;
class Parser;

class BasicExit :public BasicData{
public: 
	BasicExit(){};
	BasicExit(LocationEntity * origin, DirectionVariety * dir, LocationEntity * dest);
	virtual ~BasicExit(){};
  /** No descriptions */
//  virtual void read(Parser * parser);
  virtual void save(ostream & out);
  virtual void print();
  virtual void report(ostream & out);
  /** No descriptions */
  virtual inline LocationEntity * getDestination() const {return destination_;}
  virtual inline DirectionVariety * getDirection() const {return dir_;}
  /** No descriptions */
  virtual int getTravelTime(MovementVariety * mode);
   /** No descriptions */
    protected:
    DirectionVariety * dir_;
		LocationEntity * destination_;
		LocationEntity * origin_;
   private:
};
typedef	vector <BasicExit *>::iterator DirectionIterator;
#endif
