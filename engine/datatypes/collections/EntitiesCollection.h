/***************************************************************************
                          EntitiesCollection.h
  Template Classes for storing collection of Overlord data objects.
	It includes specific search and access methods.

                             -------------------
    begin                : Mon Jun 10 12:24:42 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef ENTITIES_COLLECTION_H
#define ENTITIES_COLLECTION_H
#include <string>
#include <vector>
#include "BasicEntitiesCollection.h"
//#include "SystemTypes.h"
#include "OverlordTypes.h"
using namespace std;
class  DataStorageHandler;
class  GameData;

template <class T> class EntitiesCollection : public BasicEntitiesCollection {
  public:
    EntitiesCollection(DataStorageHandler * handler, long int dimensions = 1000);
    ~EntitiesCollection(){	/*clear();*/}
    T* operator []       (long int        index);
    T* operator []       (const string &tag);       
    STATUS                   addNew     (T * const newEntity)   ; 
//      void  initialize            ();
  protected:
  private:

};

#ifndef ENTITIESCOLLECTIONEXPLICIT_H
#include "EntitiesCollection.cpp"
#endif


#endif

