/***************************************************************************
                          RulesCollection.h
  Template Classes for storing collection of Overlord data objects.
It includes access methods and methods for performing some generic
operations on all objects stored in the collection.

                             -------------------
    begin                : Mon Jun 10 12:24:42 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef RULESCOLLECTIONEXPLICIT_H
#define RULESCOLLECTIONEXPLICIT_H
#include "RulesCollection.h"
#endif
#include <time.h>
#include "Rule.h"

template <class T> T* RulesCollection <T>::operator [] (long int index)
{
	if (index <= size())
         return dynamic_cast< T*>(data_[index]);
//	 return dynamic_cast< T*>(*(begin() + index) );
 cerr << "Error: Rules array index (" << index << ") <in ["<<getCollectionKeyword()<<"] is out of array dimensions!\n";
 return 0 ;
}

template <class T> T* RulesCollection <T>::operator [] (const string &tag)
{
RulesIterator iter;
// vector<GameData *>::const_iterator iter;
 for (iter = begin(); iter != end(); iter++)
   {
      if ((*iter) ->getTag() == tag)
       return dynamic_cast< T*> (*iter);
//       return  (*iter);
   } 

 //cerr << "Error []: Tag (" << tag  << ") was not found in "<<collectionKeyword_<<"-s collection\n";
    cerr << "Error: Tag [" << tag << "] ("<<getStorageName()
           <<":"<<getStorageLocator() <<") was not found in "
           << "["<<getCollectionKeyword()<<"]"<<" collection"<<endl;
    return 0 ;
}


