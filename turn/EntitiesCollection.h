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

#ifndef ENTITIESCCOLLECTION_H
#define ENTITIESCCOLLECTION_H
#include <string>
#include <vector>
#include "BasicCollection.h"
#include "GameData.h"
#include "OverlordTypes.h"


template <class T> class EntitiesCollection : public BasicCollection {
  public:
    EntitiesCollection(/*size_type*/ unsigned long dimensions = 10000);
    ~EntitiesCollection();
    T* operator []       (unsigned long        index);
    T* operator []       (const string &tag);       
    void       add  ( GameData * const newEntity)   ;
    void       addNew  (GameData * const newEntity)   ; 
      bool      isValidTag (const string &tag);
      unsigned long        getSize()                       const;
      STATUS        getIndex    (const string &tag, unsigned long &index);
      /* STATUS        getIndex    (const char *tag, unsigned long &index);  */
      void  initialize            ();
      void  print                ();
      void  resize                (unsigned long size);
      void  remove                (const string &tag);             

  protected:
  private:
      vector<T*> data_;
      char prefix_;
      unsigned long emptyPlaces_;
      unsigned long dimensions_;
};

#ifndef ENTITIESCOLLECTIONEXPLICIT_H
#include "EntitiesCollection.cpp"
#endif


#endif

