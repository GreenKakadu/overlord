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

#ifndef RULESCOLLECTION_H
#define RULESCOLLECTION_H
#include <string>
#include <list>
#include "BasicCollection.h"
#include "GameData.h"


template <class T> class RulesCollection : public BasicCollection {
  public:
    RulesCollection();
    ~RulesCollection();
    T* operator []       (unsigned long        index);
    T* operator []       (const string &tag);       
    void       add  ( GameData * const newRule)   ; 
      bool      isValidTag (const string &tag)  const;
      unsigned long        getSize()                       const;
      unsigned long        getIndex    (const string &tag)  const;
      void  initialize            ();
      void  print                ();

  protected:
  private:
      list<T*> data_;

};

#ifndef RULESCOLLECTIONEXPLICIT_H
#include "RulesCollection.cpp"
#endif


#endif

