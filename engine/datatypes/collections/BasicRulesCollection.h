/***************************************************************************
                          BasicRulesCollection.h

    Basic Collection of Polymorphic Persistent objects
                             -------------------
    begin                : Mon Jun 10 12:24:42 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef BASIC_RULES_COLLECTION_H
#define BASIC_RULES_COLLECTION_H
#include <string>
#include <vector>
//#include "SystemTypes.h"
#include "OverlordTypes.h"
#include "BasicCollection.h"
using namespace std;
class  Rule;
class  DataStorageHandler;
typedef     vector <Rule *>::iterator RulesIterator;
/** Defines interface for rules collections */
class BasicRulesCollection : public BasicCollection{
public:
	  BasicRulesCollection(DataStorageHandler * handler):BasicCollection(handler){}
  virtual ~BasicRulesCollection();
	        GameData* findByTag        (const string &tag);
	        GameData* findByIndex      (const long int index);
          void          add          (GameData * /*const*/ newRule)   ;
          bool          isValidTag   (const string &tag)  ;//const;
          long int       getIndex     (const string &tag)  ;

   inline long int        size() const    {return data_.size();}
   inline RulesIterator begin()         {return data_.begin();}
   inline RulesIterator end()           {return data_.end();}
protected:
    vector <Rule *> data_;
private:


};

#endif
