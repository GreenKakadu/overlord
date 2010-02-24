/***************************************************************************
                          CreatureRaceRule.cpp 
                             -------------------
    begin                : Thu Apr 17 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "CreatureRaceRule.h"
CreatureRaceRule sampleCreatureRaceRule=CreatureRaceRule("CREATURE", &sampleRace);
CreatureRaceRule::CreatureRaceRule ( const CreatureRaceRule * prototype ) : RaceRule(prototype)
{
  hiringCost_ = 500;
  hiringProbability_ = 100;
  hiringMax_ = 100;
  controlPointsFraction_ = 20;
  name_ = "creature";
}

GameData * CreatureRaceRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<CreatureRaceRule> (this);
}

STATUS
CreatureRaceRule::initialize        ( Parser *parser )
{
  if (parser->matchKeyword ("SPECIALIST") )
    {
//			special_ = true;
      return OK;
    }
      return RaceRule::initialize(parser);
}

bool CreatureRaceRule::mayWork()
{
  return false;
}



void CreatureRaceRule::printTypeSpecificDescription(ReportPrinter & out)
{
  out << " This is a creature.";
}


LEARNING_RESULT CreatureRaceRule::mayLearn(SkillRule * skill, UnitEntity * unit)
{
   return CANNOT_STUDY_FAILURE;
}
