/***************************************************************************
                          CombatFileVariety.cpp  
                             -------------------
      Defines possible values for combat file settings
    begin                : Sun Jun 20 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "CombatFileVariety.h"
CombatFileVariety    sampleCombatFile    ("COMBAT_FILE",   &sampleGameData);
CombatFileVariety  *defaultCombatFile;
VarietiesCollection <CombatFileVariety>    combatFiles(new DataStorageHandler("combat_files.var"));

GameData * CombatFileVariety::createInstanceOfSelf()
{
  return CREATE_INSTANCE<CombatFileVariety> (this);
}


