/***************************************************************************
                          LeaderRaceRule.cpp 
                             -------------------
    begin                : Thu Apr 17 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "LeaderRaceRule.h"

LeaderRaceRule::LeaderRaceRule ( const LeaderRaceRule * prototype ) : RaceRule(prototype)
{
  hiringCost_ = 500;
  hiringProbability_ = 100;
  hiringMax_ = 100;
}

GameData * LeaderRaceRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<LeaderRaceRule> (this);
}

STATUS
LeaderRaceRule::initialize        ( Parser *parser ) 
{
  if (parser->matchKeyword ("SPECIALIST") )
    {
//			special_ = true;
      return OK;
    }
      return RaceRule::initialize(parser);
}

LEARNING_RESULT LeaderRaceRule::mayLearn(SkillRule * skill, UnitEntity * unit)
{
   return LEARNING_OK;
}


bool LeaderRaceRule::teacherRequired(SkillRule * skill, UnitEntity * unit)
{
  // if it is a combat skill and level >= 3
  if(skill->isCombatSkill() && unit->getSkillLevel(skill) >= 3)
    return true;

  // if it is a magic skill and level >= 1
  if(skill->isMagicSkill() && unit->getSkillLevel(skill) >= 1)
    return true;

  // if it is any other skill and level >= 2
  if( unit->getSkillLevel(skill) >= 2)
    return true;
  else
  return false;
}
bool LeaderRaceRule::mayRectuit()
{
  return true;
}
bool LeaderRaceRule::mayTrade()
{
  return true;
}
bool LeaderRaceRule::mayTransferFigures()
{
  return false;
}
/** How many seats occupies this entity in the class. Number of entities that can be tought by one teacher determined by this value. */
int LeaderRaceRule::getLearningCapacity()
{
  return 100;
}
