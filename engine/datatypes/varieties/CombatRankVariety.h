/***************************************************************************
                          CombatRankVariety.h
      Defines possible values for combat rank settings                    
                             -------------------
    begin                : Sun Jun 20 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#ifndef COMBAT_RANK_H
#define COMBAT_RANK_H

#include "Variety.h"
#include "VarietiesCollection.h"

class CombatRankVariety : public Variety  {
public: 
      CombatRankVariety ( const string & keyword, GameData * parent): Variety(keyword, parent){}
      CombatRankVariety ( const CombatRankVariety * prototype ) : Variety(prototype){}
		 ~CombatRankVariety(){};
      GameData * createInstanceOfSelf();
    protected:
    private:
};

//extern VarietiesCollection <CombatRankVariety>     combatRanks;
extern CombatRankVariety  *defaultCombatRank;
#endif
