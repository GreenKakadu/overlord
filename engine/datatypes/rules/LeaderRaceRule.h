/***************************************************************************
                          LeaderRaceRule.h 
                             -------------------
    begin                : Thu Apr 17 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef LEADER_RACERULE_H
#define LEADER_RACERULE_H

#include "RaceRule.h"

/**
  *@author Alex Dribin
  */

class LeaderRaceRule : public RaceRule  {
public: 
      LeaderRaceRule ( const string & keyword, GameData * parent) : RaceRule(keyword, parent){}
      LeaderRaceRule ( const LeaderRaceRule * prototype );
    	~LeaderRaceRule(){}
       STATUS     initialize      ( Parser *parser);
      GameData * createInstanceOfSelf();
      void printTypeSpecificDescription(ReportPrinter & out);
   LEARNING_RESULT mayLearn(SkillRule * skill, UnitEntity * unit);
       bool teacherRequired(SkillRule * skill, UnitEntity * unit);
       bool mayRectuit();
       bool mayTrade();
       bool mayTransferFigures();
       bool mayHoldTitles();
       int getLearningCapacity();
};
extern LeaderRaceRule     sampleLeaderRaceRule;

#endif
