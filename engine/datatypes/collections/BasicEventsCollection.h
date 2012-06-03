/***************************************************************************
 *
 *                        BasicEventsCollection.h
 *
 *  Created on: May 16, 2011
 *      Author: alex
 *
    copyright            : (C) 2011 by Alex Dribin
 *  email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#ifndef BASIC_EVENTS_COLLECTION_H_
#define BASIC_EVENTS_COLLECTION_H_
#include <string>
#include <map>
#include "OverlordTypes.h"
#include "BasicCollection.h"
#include "Event.h"
using namespace std;
class  Variety;
class  DataStorageHandler;
typedef     vector <Event *>::iterator EventsIterator;
class BasicEventsCollection: public BasicCollection {
public:
	BasicEventsCollection(DataStorageHandler * handler = 0, GameData *sample=0):BasicCollection(handler,sample){}
	virtual ~BasicEventsCollection();
	Event * findEvent(string tag, bool errorReportEnabled =true);
 //  GameData* findByTag        (const string &tag, bool errorReportEnabled = true);
 //   GameData* findByIndex      (const long int index, bool errorReportEnabled = true);
  void          addEvent          (Event *  newEntity, bool isReportDuplication=false)   ;
//  bool          isValidTag   (const string &tag)  ;
//  long int       getIndex     (const string &tag)  ;

inline long int        size() const    {return data_.size();}
//inline EventsIterator begin()         {return data_.begin();}
//inline EventsIterator end()           {return data_.end();}
protected:
 map < string, Event *> data_;
private:
};

#endif /* BASICEVENTSCOLLECTION_H_ */



