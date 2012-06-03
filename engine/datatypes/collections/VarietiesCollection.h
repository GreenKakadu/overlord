/***************************************************************************
                          VarietiesCollection.h
  Template Classes for storing collection of Overlord data objects.
	It includes specific search and access methods.
                             -------------------
    begin                : Mon Jun 10 12:24:42 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef VARIETIES_COLLECTION_H
#define VARIETIES_COLLECTION_H
#include <string>
//#include "SystemTypes.h"
#include "BasicVarietiesCollection.h"
using namespace std;
class  DataStorageHandler;
class  GameData;

template <class T> class VarietiesCollection : public BasicVarietiesCollection {
  public:
    VarietiesCollection(DataStorageHandler * handler = 0, GameData *sample=0) : BasicVarietiesCollection(handler,sample){}
    ~VarietiesCollection(){/*clear();*/}
          T*            operator []  (long int        index);
          T*            operator []  (const string &tag);       
//      void  initialize            ();


  protected:
  private:

};

#ifndef VARIETIESCOLLECTIONEXPLICIT_H
#include "VarietiesCollection.cpp"
#endif


#endif

