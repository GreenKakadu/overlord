/***************************************************************************
                          CreatureRaceRule.h 
                             -------------------
    begin                : Thu Apr 17 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef CREATURE_RACE_H
#define CREATURE_RACE_H

#include "RaceRule.h"

/**
  *@author Alex Dribin
  */

class CreatureRaceRule : public RaceRule  {
public: 
      CreatureRaceRule ( const string & keyword, GameData * parent) : RaceRule(keyword, parent){}
      CreatureRaceRule ( const CreatureRaceRule * prototype );
    	~CreatureRaceRule(){}
       STATUS     initialize      ( Parser *parser);
      GameData * createInstanceOfSelf();
      bool mayWork();

};

#endif
