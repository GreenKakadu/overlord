/***************************************************************************
                          CombatRankVariety.cpp  
                             -------------------
      Defines possible values for combat rank settings
    begin                : Sun Jun 20 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "CombatRankVariety.h"
CombatRankVariety    sampleCombatRank    ("COMBAT_RANK",   &sampleGameData);
//VarietiesCollection <CombatRankVariety>    combatRanks(new DataStorageHandler("combat_ranks.var"));

CombatRankVariety  *defaultCombatRank;
GameData * CombatRankVariety::createInstanceOfSelf()
{
  return CREATE_INSTANCE<CombatRankVariety> (this);
}


