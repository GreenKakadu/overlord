/***************************************************************************
                          RulesCollection.h
  Template Classes for storing collection of Overlord data objects.
It includes access methods and methods for performing some generic
operations on all objects stored in the collection.

                             -------------------
    begin                : Mon Jun 10 12:24:42 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "RulesCollectionExplicit.h"
#include <time.h>



template <class T>
RulesCollection<T>::RulesCollection ()
{  
  // list <T*> data_;
}



template <class T> RulesCollection <T>::~RulesCollection()
{



 list<T *>::const_iterator dataIterator;

 for (dataIterator = data_.begin(); dataIterator != data_.end(); dataIterator++)
   {
     delete *dataIterator;
   } 
 // not used yet: list<OvlObj *>::const_iterator childsIterator;

}



template <class T> T* RulesCollection <T>::operator [] (unsigned long index)
{
 list<T *>::const_iterator iter;
 unsigned long i=0;                                            
 for (iter = data_.begin(); iter != data_.end(); iter++)
   {
     if (i == index)
       return (*iter);
     i++;
   }
 cerr << "Error: Array index (" << index << ") is out of array dimensions!\n";
// cerr << "Last valid field was " << (*(iter--))->getName()<<"\n";
 return 0 ;    
}



template <class T> T* RulesCollection <T>::operator [] (const string &tag)
{
 list<T *>::const_iterator iter;
 for (iter = data_.begin(); iter != data_.end(); iter++)
   {
      if ((*iter) ->getTag() == tag)
       return (*iter);

   } 
 cerr << "Error: Tag (" << tag  << ") was not found!\n";
// cerr << "Last valid field was " << (*data_.end())->getName()<<"\n";
 return 0 ;    
}


template <class T>
void   RulesCollection <T>::add  ( GameData * const newRule)
{
  data_.push_back ( dynamic_cast< T*> (newRule) );
}



template <class T>
bool RulesCollection <T>::isValidTag (const string &tag) const
{
 list<T *>::const_iterator iter;
 for (iter = data_.begin(); iter != data_.end(); iter++)
   {
      if ((*iter) ->getTag() == tag)
       return true;

   } 
 return false;
}




template <class T>
void RulesCollection <T>::print () 
{
 list<T *>::const_iterator iter;
 for (iter = data_.begin(); iter != data_.end(); iter++)
   {
      (*iter) ->print();
   }

}




template <class T>
unsigned long  RulesCollection <T>::getSize() const
{
  return data_.size();
}




template <class T>
unsigned long  RulesCollection <T>::getIndex (const string &tag) const
{
 list<T *>::const_iterator iter;
 unsigned long i=0;

 for (iter = data_.begin(); iter != data_.end(); iter++)
   {
      if ((*iter) ->getTag() == tag)
       return i;
      i++;
   } 
 return i-1;

}
