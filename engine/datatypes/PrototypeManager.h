/***************************************************************************
                          PrototypeManager.h
This object is used in implementation of Prototype design pattern
(see Design Messages. E. Gamma, R. Helm, R. Johnson, J. Vlissides)
                             -------------------
    begin                : Fri Nov  9 19:24:42 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#ifndef PROTOTYPE_MANAGER_H
#define PROTOTYPE_MANAGER_H
#include <vector>
#include <string>
#include "GameData.h"
using namespace std;

class PrototypeManager
{
    public:
      PrototypeManager();
      ~PrototypeManager();
      GameData * findInRegistry ( const string& keyword) const ;
      void addToRegistry (GameData  * object);
    private:
      vector <GameData *> registry;
};
extern PrototypeManager * prototypeManager;

#endif
