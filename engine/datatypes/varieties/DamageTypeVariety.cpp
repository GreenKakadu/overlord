/***************************************************************************
                          DamageTypeVariety.cpp
                             -------------------
                        Defines types of damage
    begin                : Sun Nov 20 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "DamageTypeVariety.h"
#include "DataStorageHandler.h"
#include "VarietiesCollection.h"
DamageTypeVariety  *defaultDamageType;
DamageTypeVariety    sampleDamageType    ("DAMAGE_TYPE",   &sampleGameData);
VarietiesCollection <DamageTypeVariety>   damageTypes(new						DataStorageHandler("damage_type.var"));

GameData * DamageTypeVariety::createInstanceOfSelf()
{
  return CREATE_INSTANCE<DamageTypeVariety> (this);
}


