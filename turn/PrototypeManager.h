/***************************************************************************
                          PrototypeManager.h 
This object is used in implementation of Prototype design pattern 
(see Design Patterns. E. Gamma, R. Helm, R. Johnson, J. Vlissides)
                             -------------------
    begin                : Fri Nov  9 19:24:42 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef PROTOTYPEMANAGER_H
#define PROTOTYPEMANAGER_H
#include <list>
#include <string>
#include "GameData.h"

class PrototypeManager
{
public:
        PrototypeManager();
       ~PrototypeManager();
        GameData * findInRegistry ( const string& keyword) const ;
        void addToRegistry (GameData  * object);
private:
           list <GameData *> registry;
};

#endif
