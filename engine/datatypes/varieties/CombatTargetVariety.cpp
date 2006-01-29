/***************************************************************************
                          CombatTargetVariety.cpp
                             -------------------
      Defines possible values for combat targets
    begin                : Sun Jun 20 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "CombatTargetVariety.h"
#include "DataStorageHandler.h"
#include "VarietiesCollection.h"
CombatTargetVariety  *defaultCombatTarget;
CombatTargetVariety sampleCombatTarget  ("COMBAT_TARGET",   &sampleGameData);
VarietiesCollection <CombatTargetVariety>    combatTargets(new DataStorageHandler("combat_target.var"));
GameData * CombatTargetVariety::createInstanceOfSelf()
{
  return CREATE_INSTANCE<CombatTargetVariety> (this);
}


