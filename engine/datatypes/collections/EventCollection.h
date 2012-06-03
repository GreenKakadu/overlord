 /***************************************************************************
                          EventsCollection.h
  Template Classes for storing collection of Overlord data objects.
	It includes specific search and access methods.
                             -------------------
    begin                : Mon May 16 12:24:42 IST 2011
    copyright            : (C) 2011 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef EVENTS_COLLECTION_H
#define EVENTS_COLLECTION_H
#include <string>
#include "BasicEventsCollection.h"
using namespace std;
class  DataStorageHandler;
class  GameData;

template <class T> class EventsCollection : public BasicEventsCollection {
  public:
	EventsCollection(DataStorageHandler * handler = 0, GameData *sample=0) : BasicEventsCollection(handler,sample){}
    ~EventsCollection(){}
          T*            operator []  (long int        index);
          T*            operator []  (const string &tag);
//      void  initialize            ();


  protected:
  private:

};

#ifndef EVENTSCOLLECTIONEXPLICIT_H
#include "EventsCollection.cpp"
#endif


#endif

