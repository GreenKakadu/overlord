/***************************************************************************
                          BasicVarietiesCollection.h

    Basic Collection of Polymorphic Persistent objects
                             -------------------
    begin                : Mon Jun 10 12:24:42 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef BASIC_VARIETIES_COLLECTION_H
#define BASIC_VARIETIES_COLLECTION_H
#include <string>
#include <vector>
//#include "SystemTypes.h"
#include "OverlordTypes.h"
#include "BasicCollection.h"
using namespace std;
class  Variety;
class  DataStorageHandler;
typedef     vector <Variety *>::iterator VarietiesIterator;
/** Defines interface for rules collections */
class BasicVarietiesCollection : public BasicCollection{
public:
	  BasicVarietiesCollection(DataStorageHandler * handler):BasicCollection(handler){}
  virtual ~BasicVarietiesCollection();
	        GameData* findByTag        (const string &tag, bool errorReportEnabled = true);
	        GameData* findByIndex      (const long int index, bool errorReportEnabled = true);
          void          add          (GameData * /*const*/ newVariety)   ;
          bool          isValidTag   (const string &tag)  ;//const;
          long int       getIndex     (const string &tag)  ;

   inline long int        size() const    {return data_.size();}
   inline VarietiesIterator begin()         {return data_.begin();}
   inline VarietiesIterator end()           {return data_.end();}
protected:
    vector <Variety *> data_;
private:


};

#endif

