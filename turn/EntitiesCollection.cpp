/***************************************************************************
                          EntitiesCollection.h
  Template Classes for storing collection of Overlord data objects.
It includes access methods and methods for performing some generic
operations on all objects stored in the collection.

                             -------------------
    begin                : Mon Jun 10 12:24:42 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "EntitiesCollectionExplicit.h"
#include "Parser.h"
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "UnitEntity.h"
extern UnitEntity sampleUnit;


template <class T>
EntitiesCollection<T>::EntitiesCollection (unsigned long /*size_type*/ dimensions)
{

  data_.resize(dimensions);
  //data_[0] = &sampleUnit; /* 0 is invalid index used solely for error indication  */


  dimensions_ = dimensions;
  emptyPlaces_ = dimensions - 1;
  prefix_ = 'U';
}




template <class T> EntitiesCollection <T>::~EntitiesCollection()
{
  unsigned long i;
  for (i = 0; i < data_.size(); i++)
    {
      if(data_[i])
	delete  data_[i];
    } 
}



template <class T> T* EntitiesCollection <T>::operator [] (unsigned long index)
{
  assert(index < dimensions_);
    return data_[index];  
}



template <class T> T* EntitiesCollection <T>::operator [] (const string &tag)
{
  unsigned long index;
  
  assert (getIndex(tag, index) == OK);
  return data_[index];       
}


template <class T>
void   EntitiesCollection <T>::add  ( GameData * const newEntity)
{
 unsigned long index ;
 if (getIndex(newEntity->getTag(), index) == ERROR)
   return;
 
 if (data_[index] != 0)
   {
     cout << "Duplicated tag " << newEntity->getTag() <<endl;
     return;
   }
 
 else

   data_[index] = dynamic_cast< T*> (newEntity);
   emptyPlaces_ --;
   if(dimensions_ / emptyPlaces_ > 10 ) // Collection is almost full 
     {
       resize(dimensions_ * 1.1 ); // reallocate additional 10% of storage
     }
      
}



template <class T>
void   EntitiesCollection <T>::addNew  ( GameData * const newEntity)
{
  int i;
  char buffer[Parser::INTEGER_LENGTH];
  unsigned long randomIndex;
  for (i=0; i <1000; i++)
    {
      randomIndex = rand() % dimensions_;
      //  cout << "randomIndex " << randomIndex << endl;
      if(data_[randomIndex] == 0)
	{
	  
	  //	  newEntity->setTag(prefix_ + itoa(randomIndex,buffer,10));
	  sprintf(buffer,"%c%d",prefix_,randomIndex); newEntity->setTag(buffer );
	  newEntity->setName(newEntity->getKeyword() );
	  data_[randomIndex] = dynamic_cast< T*> (newEntity);
	  emptyPlaces_ --;
	  if(dimensions_ / emptyPlaces_ > 10 ) // Collection is almost full 
	    {
	      resize(dimensions_ * 1.1 ); // reallocate additional 10% of storage
	    }
	  return;

	}
    }
  cout << "Failed to find vacant id for" << newEntity->getName() << endl;
}



template <class T>
bool EntitiesCollection <T>::isValidTag (const string &tag) 
{

 unsigned long index ;
 if (getIndex(tag, index) == ERROR)
   return false;

 if (data_[index] == 0)
   return false;
   
 else   
   return true;
}




template <class T>
void EntitiesCollection <T>::print () 
{
unsigned long i;
 for (i = 0; i < data_.size(); i++)
   {
     if(data_[i])
       (data_[i])->print();
   }

}


template <class T>
void EntitiesCollection <T>::resize (unsigned long size) 
{
  data_.resize(size);
  emptyPlaces_ = emptyPlaces_ + (size - dimensions_);
  cout << dimensions_<< " - Reallocating memory to " <<size<<endl;// for test
  dimensions_ = size;

}




template <class T>
unsigned long  EntitiesCollection <T>::getSize() const
{
  return data_.size();
}


template <class T>
STATUS EntitiesCollection <T>::getIndex (const string &tag, unsigned long &index) 
{
 unsigned i;
 index = 0;
  if (!isalpha (tag[0]) )
    {
      cout << "Tag " << tag << " is invalid (non-alphabetical prefix)" <<endl;
      return ERROR;
    }
 if ( (tag[0] & 0x1F) != (prefix_ & 0x1F) )// after throwing away all 
   //non-alpha  symbols this works as case-nonsensitive comparison of symbols

    {
      cout << "Tag " << tag << " is invalid (wrong prefix)" <<endl;
      return ERROR;
    }


  
 for (i=0; i< Parser::INTEGER_LENGTH; i++)
   {
     if (!isdigit (tag[i+1]) )  // Non-digit was found
       

       break;
       
     if (!tag[i+1])  // End of string
       break; 
   }
 
 if ( i == 0 ) // No digits were found.
    {
      cout << "Tag " << tag << " is invalid (no  digits)" <<endl;
      return ERROR;
    }
 if ( i >=  Parser::INTEGER_LENGTH  ) // More than maximum of digits were found.
    {
      cout << "Tag " << tag << " is invalid (too many digits)" <<endl;
      return ERROR;
    }
    
 index = atoi (tag.c_str() +1);

 if (index >= dimensions_)
   return ERROR;
 else
   
   return OK;
}
template <class T>
void EntitiesCollection <T>:: remove  (const string &tag)
{
 unsigned long index ;
 if (getIndex(tag, index) == ERROR)
   return;

 if (data_[index] == 0)
   return;
   
 else
   delete data_[index];
 data_[index] = 0; // Check
}
