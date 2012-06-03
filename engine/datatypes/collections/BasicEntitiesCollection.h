/***************************************************************************
                          BasicEntitiesCollection.h

    Basic Collection of Polymorphic Persistent objects
                             -------------------
    begin                : Mon Jun 10 12:24:42 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef BASIC_ENTITIES_COLLECTION_H
#define BASIC_ENTITIES_COLLECTION_H
#include <string>
#include <vector>
#include "OverlordTypes.h"
#include "BasicCollection.h"
using namespace std;
class  Entity;
class  DataStorageHandler;
typedef     vector <Entity *>::iterator EntitiesIterator;
/** Defines interface for entities collections */

class BasicEntitiesCollection : public  BasicCollection{
public:
    BasicEntitiesCollection(DataStorageHandler *handler =0, GameData *sample=0):BasicCollection(handler,sample){}
    BasicEntitiesCollection (DataStorageHandler * handler =0, GameData *sample=0,
                                          long int dimensions=1000);
  virtual ~BasicEntitiesCollection();
  const static int incrementSize;
GameData* findByTag        (const string &tag, bool errorReportEnabled = true);
GameData* findByIndex      (const long int index, bool errorReportEnabled = true);
GameData* findByName       (const string &name, bool errorReportEnabled = true);
          bool          isValidTag   (const string &tag)  ;//const;
          long int        getIndex     (const string &tag, bool errorReportEnabled = true)  ;
          long int        extractIndex     (const string &tag, bool errorReportEnabled = true)  ;
  virtual void             add      (GameData* /*const*/ newEntity, bool isReportDuplication =true);
  inline long int        size() const    {return data_.size();}
  inline  EntitiesIterator begin()         {return data_.begin();}
  inline  EntitiesIterator end()           {return data_.end();}
    void                   redimention        (long int size);
    void                   remove             (const string &tag);
    bool                   checkDataType    (const string &tag);
    void                   setEntityTagPrefix (char prefix);
		char                   getEntityTagPrefix ();
    void                   clear();
    string tagFromIndex(long int index);
   void addRIPindex(long int index);
   long int getRIPsize() const;
   long int getRIPbyIndex(long int i) const;
	 NewEntityPlaceholder * findOrAddPlaceholder(const string &tag);
	 NewEntityPlaceholder * checkPlaceholder(const string &tag);
protected:
    vector <Entity*> data_;
    char prefix_;
    long int emptyPlaces_;
    long int dimensions_;
    vector <long int> RIP_;
    vector <NewEntityPlaceholder * > newEntities_;
private:


};

#endif

