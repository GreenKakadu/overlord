/***************************************************************************
                          EntitiesCollection.cpp
  Template Classes for storing collection of Overlord data objects.
It includes access methods and methods for performing some generic
operations on all objects stored in the collection.

                             -------------------
    begin                : Mon Jun 10 12:24:42 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef ENTITIESCOLLECTIONEXPLICIT_H
#define ENTITIESCOLLECTIONEXPLICIT_H
#include "EntitiesCollection.h"
#endif
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <algorithm>
#include "LineParser.h"

template <class T>
EntitiesCollection<T>::EntitiesCollection (DataStorageHandler * handler,
                                          long int dimensions) : BasicEntitiesCollection(handler, dimensions)
{
}


template <class T> T* EntitiesCollection <T>::operator [] (long int index)
{
	if ( /*(index >= 0) &&*/ (index <= dimensions_))
         return dynamic_cast< T*> (data_[index]);
  else
    cout << "Error: Array index (" << index << ") is out of array dimensions (" <<dimensions_<<")"<<endl;
		status = IO_ERROR;
    return 0 ;
}


template <class T> T* EntitiesCollection <T>::operator [] (const string &tag)
{
  long int index = getIndex(tag);
  if (status == OK)
         return dynamic_cast< T*> (data_[index]);
  else
		     return 0;
}


template <class T>
STATUS   EntitiesCollection <T>::addNew  ( T * const newEntity)
{
  int i;
  long int randomIndex;
  for (i=0; i <1000; i++)
    {
      randomIndex = rand() % dimensions_;
      if(data_[randomIndex] == 0)
		  {
	     if (binary_search(RIP_.begin(),RIP_.end(),randomIndex) )
            continue;  // no reuse of indexes of entities which are now dead

      newEntity->setTag(tagFromIndex(randomIndex));
		  newEntity->setName(newEntity->getKeyword() );

      data_[randomIndex] =  dynamic_cast< T*> (newEntity);
			  emptyPlaces_ --;
	 	  if(dimensions_ / emptyPlaces_ > 10 ) // Collection is almost full
	    	{
	      	redimention((long int)(dimensions_ * 1.1) ); // reallocate additional 10% of storage
	    	}
	  	return OK;
		  }
    }
  cout << "Failed to find vacant id for" << newEntity->getName() << endl;
	return IO_ERROR;
}

