/***************************************************************************
                          PrototypeManager.cpp  
                             -------------------
    begin                : Fri Nov  9 19:24:42 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "PrototypeManager.h"
PrototypeManager * prototypeManager = 0; // will be created on first call

PrototypeManager::PrototypeManager()
{
}

PrototypeManager::~PrototypeManager()
{
 vector<GameData *>::const_iterator iter;
                                          
 for ( iter = registry.begin(); iter != registry.end(); iter++)
   {
       delete (*iter);
   }
}



void      PrototypeManager::addToRegistry  (  GameData * object)
{
  registry.push_back(object);
}



GameData * PrototypeManager::findInRegistry ( const string &keyword) const 
{
  if (registry.size() == 0)
    return 0;
 
 vector<GameData *>::const_iterator iter;
                                          
 for ( iter = registry.begin(); iter != registry.end(); iter++)
   {


     if ((*iter) -> getKeyword() == keyword)
       {

       return (*iter);
       }
   }


 return 0;
}
