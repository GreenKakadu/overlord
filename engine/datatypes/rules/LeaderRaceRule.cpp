/***************************************************************************
                          LeaderRaceRule.cpp 
                             -------------------
    begin                : Thu Apr 17 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "LeaderRaceRule.h"
//LeaderRaceRule     sampleLeaderRaceRule =     LeaderRaceRule("LEADER", &sampleRace);

LeaderRaceRule::LeaderRaceRule ( const LeaderRaceRule * prototype ) : RaceRule(prototype)
{
  hiringCost_ = 500;
  hiringProbability_ = 100;
  hiringMax_ = 100;
  controlPointsFraction_ =1;
  name_ = "leader";
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



/*
 *  Intristic ability of race to study  skill
 *  Level limitations also taken into considerations
 */
LEARNING_RESULT LeaderRaceRule::mayLearn(SkillRule * skill, UnitEntity * unit)
{
  int  level = unit->getSkillLevel(skill);
  int learningLevelBonus = unit->getLearningLevelBonus(skill);
// Leader may not study magic skills  above 1-st level
// non-combat skill above 2-nd level
// and combat skill above 3-rd level
// Heroes may not study combat skill above 4-th level
   



  if(level < 1 + learningLevelBonus)
      return LEARNING_OK;
  // from here level >= 1
      
  if(skill->isMagicSkill())
      return TEACHING_REQUIRED;

  if(level < 2 + learningLevelBonus)
      return LEARNING_OK;
  // from here level >= 2
  if(!skill->isCombatSkill())
    return TEACHING_REQUIRED;

  if(level < 3 + learningLevelBonus)
      return LEARNING_OK;

  // from here level >= 3
     return TEACHING_REQUIRED;
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



void LeaderRaceRule::printTypeSpecificDescription(ReportPrinter & out)
{
  out << " This is a leader.";
}



bool LeaderRaceRule::mayHoldTitles()
{
  return true;
}

