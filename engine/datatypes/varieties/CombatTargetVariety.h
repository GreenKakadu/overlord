/***************************************************************************
                          CombatTargetVariety.h
      Defines possible values for combat targets
                             -------------------
    begin                : Sun Nov 20 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef COMBAT_TARGET_H
#define COMBAT_TARGET_H

#include "Variety.h"

class CombatTargetVariety : public Variety  {
public: 
      CombatTargetVariety ( const string & keyword, GameData * parent): Variety(keyword, parent){}
      CombatTargetVariety ( const CombatTargetVariety * prototype ) : Variety(prototype){}
		 ~CombatTargetVariety(){};
      GameData * createInstanceOfSelf();
    protected:
    private:
};

extern VarietiesCollection <CombatTargetVariety>     combatTargets;
extern CombatTargetVariety  *defaultCombatTarget;
#endif
