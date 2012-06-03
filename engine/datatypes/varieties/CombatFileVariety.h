/***************************************************************************
                          CombatFileVariety.h
      Defines possible values for combat file settings                    
                             -------------------
    begin                : Sun Jun 20 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#ifndef COMBAT_FILE_H
#define COMBAT_FILE_H

#include "Variety.h"
#include "VarietiesCollection.h"

class CombatFileVariety : public Variety  {
public: 
      CombatFileVariety ( const string & keyword, GameData * parent): Variety(keyword, parent){}
      CombatFileVariety ( const CombatFileVariety * prototype ) : Variety(prototype){}
		 ~CombatFileVariety(){};
      GameData * createInstanceOfSelf();
    protected:
    private:
};

//extern VarietiesCollection <CombatFileVariety>     combatFiles;
extern CombatFileVariety  *defaultCombatFile;
#endif
