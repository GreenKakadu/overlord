/***************************************************************************
                          BasicCollection.h

    Basic Collection of Polymorphic Persistent objects
                             -------------------
    begin                : Mon Jun 10 12:24:42 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef BASICCOLLECTION_H
#define BASICCOLLECTION_H
#include <string>
#include <list>
#include "GameData.h"



class BasicCollection {
public:
	  BasicCollection();
	  virtual ~BasicCollection();

	  /* virtual GameData* findByTag    (const string &tag);        //  access by tag */
	  virtual GameData* operator []       (const string &tag);        //  access by tag
          virtual bool      isValidTag (const string &tag)  const; // check 
                                             //for string being valid title tag
	  virtual int        getSize()                       const;
          virtual int        getIndex    (const string &tag)  const;
          virtual void       add  ( GameData * newRule)   ;
          virtual void       print ()  ;

protected:
private:


};
#endif

