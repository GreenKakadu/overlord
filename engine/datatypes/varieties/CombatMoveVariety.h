/***************************************************************************
                          CombatMoveVariety.h
      Defines possible values for combat move settings                    
                             -------------------
    begin                : Sun Jun 20 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef COMBAT_MOVE_H
#define COMBAT_MOVE_H

#include "Variety.h"

class CombatMoveVariety : public Variety  {
public: 
      CombatMoveVariety ( const string & keyword, GameData * parent): Variety(keyword, parent){}
      CombatMoveVariety ( const CombatMoveVariety * prototype ) : Variety(prototype){}
		 ~CombatMoveVariety(){};
      GameData * createInstanceOfSelf();
    protected:
    private:
};

extern VarietiesCollection <CombatMoveVariety>     combatMoves;
extern CombatMoveVariety  *defaultCombatMove; 
#endif
