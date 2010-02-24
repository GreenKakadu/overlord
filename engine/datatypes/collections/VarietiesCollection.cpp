/***************************************************************************
                          VarietiesCollection.h
  Template Classes for storing collection of Overlord data objects.
It includes access methods and methods for performing some generic
operations on all objects stored in the collection.

                             -------------------
    begin                : Mon Jun 10 12:24:42 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "Variety.h"
#ifndef VARIETIESCOLLECTIONEXPLICIT_H
#define VARIETIESCOLLECTIONEXPLICIT_H
#include "VarietiesCollection.h"
#endif
#include <time.h>
//#include "Global.h"
#include "GameData.h"
extern bool ciStringCompare(const string& s1,const string& s2);
template <class T> T* VarietiesCollection <T>::operator [] (long int index)
{
	if (index <= size())
         return dynamic_cast< T*>(data_[index]);
//	 return dynamic_cast< T*>(*(begin() + index) );
 cerr << "Error: Array index (" << index << ") is out of array dimensions!\n";
 return 0 ;
}

template <class T> T* VarietiesCollection <T>::operator [] (const string &tag)
{
VarietiesIterator iter;
// vector<GameData *>::const_iterator iter;
 for (iter = begin(); iter != end(); iter++)
   {
      if (!ciStringCompare((*iter) ->getTag(),tag))
       return dynamic_cast< T*> (*iter);
//       return  (*iter);
   }

 cerr << "Error: Tag (" << tag  << ") was not found in "<<collectionKeyword_<<"-s collection\n";
    return 0 ;
}


