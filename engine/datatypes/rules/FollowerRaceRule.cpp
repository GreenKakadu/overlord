/***************************************************************************
                          FollowerRaceRule.cpp 
                             -------------------
    begin                : Thu Apr 17 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "FollowerRaceRule.h"

FollowerRaceRule::FollowerRaceRule ( const FollowerRaceRule * prototype ) : RaceRule(prototype)
{
  hiringCost_ = 500;
  hiringProbability_ = 100;
  hiringMax_ = 100;
}

GameData * FollowerRaceRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<FollowerRaceRule> (this);
}

STATUS
FollowerRaceRule::initialize        ( Parser *parser )
{
//  if (parser->matchKeyword ("SPECIALIST") )
//    {
//      return OK;
//    }
      return RaceRule::initialize(parser);
}
bool FollowerRaceRule::skillCompartibilityCheck(SkillRule * skill, UnitEntity * unit)
{
  // if it is a basic skill AND level is 0 AND unit already has some other skill

//   cout << "==check== Can "<< unit->printName()<< " learn " << skill->printName()<<" ?\n";

//   if(skill->getRequirement(0))
//        return true;   //    This is not basic skill
   if(unit->getSkillPoints(skill))
        return true;   //unit already knows this skill
   vector < SkillElement>::iterator iter;
   
   for(iter = unit->getAllSkills().begin(); iter != unit->getAllSkills().end(); ++iter)
   {
     if( (*iter).getSkill()->getBasicSkill() != skill->getBasicSkill() )
      return false;
       
   }
   
   return true;
}
bool FollowerRaceRule::teacherRequired(SkillRule * skill, UnitEntity * unit)
{
  // if it is a combat skill and level >= 2
  if(skill->isCombatSkill() && unit->getSkillLevel(skill) >= 2)
    return true;

  // if it is any other skill and level >= 1
  if( unit->getSkillLevel(skill) >= 1)
    return true;
  else
  return false;
}



LEARNING_RESULT FollowerRaceRule::mayLearn(SkillRule * skill, UnitEntity * unit)
{
  if (!skillCompartibilityCheck( skill, unit) )
   return FOLLOWER_CANNOT_STUDY_SECOND_BASIC_SKILL_FAILURE;
 
  else
   return LEARNING_OK;

}

bool FollowerRaceRule::mayMove()
{
  return false;
}

/** How many seats occupies this entity in the class. Number of entities that can be tought by one teacher determined by this value. */
int FollowerRaceRule::getLearningCapacity()
{
  return 2;
}
