/***************************************************************************
                          BasicCollection.cpp  
              Basic Collection of Polymorphic Persistent objects
                             -------------------
    begin                : Mon Jun 10 12:24:42 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include <time.h>
#include "BasicCollection.h"
#include "DataStorageHandler.h"
BasicCollection::~BasicCollection()
{
  // delete handler_; // <- check
}

bool BasicCollection::isValidTag (const string &) //const
{
 return false;
}



bool      BasicCollection::checkDataType    (const string &)
{
 return true;
}



void BasicCollection::bindHandler(DataStorageHandler * handler)
{
  handler_ = handler;
  handler->setCollection(this);
//  cout << " Handled binded for collection "  <<collectionKeyword_ << endl;
}



long  BasicCollection::size() const
{
  return 0;
  
}
GameData* BasicCollection::findByTag        (const string &, bool ){return 0;}
GameData* BasicCollection::findByIndex      (const long int , bool){return 0;}


void BasicCollection::add  ( GameData * /*const*/ newRule)
{
}



long   BasicCollection::getIndex (const string &)
{
  return 0;
}


void       BasicCollection::redimention (long int )
{
}

NewEntityPlaceholder * BasicCollection::findOrAddPlaceholder(const string &)
{
  return 0;
}

NewEntityPlaceholder * BasicCollection::checkPlaceholder(const string &)
{
  return 0;
}

