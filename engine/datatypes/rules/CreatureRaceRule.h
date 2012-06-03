/***************************************************************************
                          CreatureRaceRule.h 
                             -------------------
    begin                : Thu Apr 17 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
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
      virtual void save(ostream &out);
      void printTypeSpecificDescription(ReportPrinter & out);
      vector <AbstractData *> aPrintTypeSpecificDescription();
      LEARNING_RESULT mayLearn(SkillRule * skill, UnitEntity * unit);
      GameData * createInstanceOfSelf();
      bool mayWork();
};
extern CreatureRaceRule   sampleCreatureRaceRule;
#endif
