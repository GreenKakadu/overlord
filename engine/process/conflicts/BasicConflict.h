/***************************************************************************
                          BasicConflict.h 
                             -------------------
    begin                : Fri Apr 18 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef BASIC_CONFLICT_H
#define BASIC_CONFLICT_H
/**
  *@author Alex Dribin
  */
#include <vector>

class BasicCompetitiveRequest;
class UnitEntity;
class LocationEntity;
using namespace std;

class BasicConflict {
public: 
	BasicConflict(LocationEntity * location);
	virtual ~BasicConflict();
  virtual void insertRequest(BasicCompetitiveRequest *request);
  virtual void addRequest(BasicCompetitiveRequest *request);
  virtual void resolve(vector  <BasicCompetitiveRequest *> & requests);
//  virtual void processRequest(UnitEntity * unit);
          void process();
//  inline virtual int getTotalAvailableValue() const {return 0;}
protected:
    vector  <BasicCompetitiveRequest *> requests_;
    LocationEntity * location_;
};

#endif
