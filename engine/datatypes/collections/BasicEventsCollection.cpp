/***************************************************************************
                          BasicEventsCollection.cpp
              Basic Collection of Polymorphic Persistent objects
                             -------------------
    begin                : Mon May 16 12:24:42 IST 2011
    copyright            : (C) 2011 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include <time.h>
#include "BasicEventsCollection.h"
#include "Event.h"
#include "DataStorageHandler.h"
extern bool ciStringCompare(const string& s1,const string& s2);
BasicEventsCollection::~BasicEventsCollection()
{
  // delete handler_; // <- check
}

Event * BasicEventsCollection::findEvent(string tag, bool errorReportEnabled)
{
    map<string, Event *>::iterator iter = data_.find(tag);
    if(iter != data_.end())
    {
       return (iter->second);
    }
    if(errorReportEnabled)
    {
    	   cerr << "Error: Tag [" << tag << "] ("<<getStorageName() <<") was not found in "
    	           <<" events collection"<<endl;
    }
    	return 0;
}

//GameData* BasicEventsCollection::findByTag (const string &tag, bool errorReportEnabled)
//{
//	EventsIterator iter;
// for (iter = begin(); iter != end(); iter++)
//   {
//      if (!ciStringCompare((*iter) ->getTag() , tag))
//       return  (*iter);
//   }
//
//  if(errorReportEnabled)
//
//   cerr << "Error: Tag [" << tag << "] ("<<getStorageName()
//           <<":"<<getStorageLocator() <<") was not found in "
//           << "["<<getCollectionKeyword()<<"]"<<" collection"<<endl;
//   return 0 ;
//}
//GameData* BasicEventsCollection::findByIndex (const  long int index, bool errorReportEnabled)
//{
//	if ( index <= size())
//         return data_[index];
//  if(errorReportEnabled)
// cerr << "Error: BasicVarieties array index (" << index << ") is out of array dimensions!\n";
// return 0 ;
//}

void   BasicEventsCollection::addEvent  ( Event * newEvent, bool)
{
//  data_.push_back ( dynamic_cast< Event*> (newEvent) );
	data_[newEvent->getTag()] = newEvent;
//	   cerr << "Debug: Tag [" << newEvent->getTag() << "] ("<<getStorageName() <<") was added to events collection"<<endl;
}




//bool BasicEventsCollection::isValidTag (const string &tag) //const
//{
//	EventsIterator iter;
// for (iter = begin(); iter != end(); iter++)
//   {
//      if ((*iter) ->getTag() == tag)
//       return true;
//
//   }
// return false;
//}



//long int  BasicEventsCollection::getIndex (const string &tag)
//{
//	EventsIterator iter;
// long int i=0;
//
// for (iter = begin(); iter != end(); iter++)
//   {
//      if ((*iter) ->getTag() == tag)
//       return i;
//      i++;
//   }
// return i-1;
//
//}


