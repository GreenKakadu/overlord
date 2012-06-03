/***************************************************************************
                          FollowerRaceRule.h 
                             -------------------
    begin                : Thu Apr 17 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#ifndef FOLLOWER_RACE_H
#define FOLLOWER_RACE_H

#include "RaceRule.h"

/**
  *@author Alex Dribin
  */

class FollowerRaceRule : public RaceRule  {
public: 
      FollowerRaceRule ( const string & keyword, GameData * parent) : RaceRule(keyword, parent){}
      FollowerRaceRule ( const FollowerRaceRule * prototype );
    	~FollowerRaceRule(){}
       STATUS     initialize      ( Parser *parser);
      virtual void save(ostream &out);
      GameData * createInstanceOfSelf();
      void printTypeSpecificDescription(ReportPrinter & out);
      vector <AbstractData *> aPrintTypeSpecificDescription();
      LEARNING_RESULT mayLearn(SkillRule * skill, UnitEntity * unit);
      bool teacherRequired(SkillRule * skill, UnitEntity * unit);
      bool mayMove(UnitEntity * unit);
      bool skillCompartibilityCheck(SkillRule * skill, UnitEntity * unit);
      int getLearningCapacity();
};
extern FollowerRaceRule   sampleFollowerRaceRule;
#endif
