/***************************************************************************
                          DamageTypeVariety.h
                        Defines types of damage
                             -------------------
    begin                : Sun Nov 20 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef DAMAGE_TYPE_H
#define DAMAGE_TYPE_H

#include "Variety.h"

class DamageTypeVariety : public Variety  {
public: 
      DamageTypeVariety ( const string & keyword, GameData * parent): Variety(keyword, parent){}
      DamageTypeVariety ( const DamageTypeVariety * prototype ) : Variety(prototype){}
		 ~DamageTypeVariety(){};
      GameData * createInstanceOfSelf();
    protected:
    private:
};
extern VarietiesCollection <DamageTypeVariety>     combatRanks;
extern DamageTypeVariety  *defaultDamageType;
#endif
