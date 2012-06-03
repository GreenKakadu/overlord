/***************************************************************************
                          RulesCollection.h
  Template Classes for storing collection of Overlord data objects.
	It includes specific search and access methods.
                             -------------------
    begin                : Mon Jun 10 12:24:42 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef RULES_COLLECTION_H
#define RULES_COLLECTION_H
#include <string>
//#include "SystemTypes.h"
#include "BasicRulesCollection.h"
//#include "../general/GameData.h"
using namespace std;
class  DataStorageHandler;
class  GameData;

template <class T> class RulesCollection : public BasicRulesCollection {
  public:
    RulesCollection(DataStorageHandler * handler = 0, GameData *sample=0) : BasicRulesCollection(handler,sample){}
    ~RulesCollection(){/*clear();*/}
          T*            operator []  (long int        index);
          T*            operator []  (const string &tag);       
//      void  initialize            ();


  protected:
  private:

};

#ifndef RULESCOLLECTIONEXPLICIT_H
#include "RulesCollection.cpp"
#endif


#endif

