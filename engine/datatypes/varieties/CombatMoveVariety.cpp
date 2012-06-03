/***************************************************************************
                          CombatMoveVariety.cpp  
                             -------------------
      Defines possible values for combat move settings
    begin                : Sun Jun 20 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "CombatMoveVariety.h"
CombatMoveVariety    sampleCombatMove    ("COMBAT_MOVE",   &sampleGameData);
//VarietiesCollection <CombatMoveVariety>    combatMoves(new DataStorageHandler("combat_moves.var"));
CombatMoveVariety  *defaultCombatMove; 
GameData * CombatMoveVariety::createInstanceOfSelf()
{
  return CREATE_INSTANCE<CombatMoveVariety> (this);
}


