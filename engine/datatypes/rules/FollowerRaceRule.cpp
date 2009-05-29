/***************************************************************************
                          FollowerRaceRule.cpp 
                             -------------------
    begin                : Thu Apr 17 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "FollowerRaceRule.h"
FollowerRaceRule sampleFollowerRaceRule= FollowerRaceRule("FOLLOWER",&sampleRace);

FollowerRaceRule::FollowerRaceRule ( const FollowerRaceRule * prototype ) : RaceRule(prototype)
{
  hiringCost_ = 500;
  hiringProbability_ = 100;
  hiringMax_ = 100;
  controlPointsFraction_ = 20;
  name_ = "follower";
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

//   cout << "==check== Can "<< unit->print()<< " learn " << skill->print()<<" ?\n";

   if(skill->getRequirement(0))
        return true;   //    This is not basic skill
   if(unit->getSkillPoints(skill))
        return true;   //unit already knows this skill
   vector < SkillElement>::iterator iter;
   
    SkillRule * scouting = skills["scou"];
    for(iter = unit->getAllSkills().begin(); iter != unit->getAllSkills().end(); ++iter)
   {
     if((*iter).getSkill()->getBasicSkill() == scouting) //Do not take scouting into account
		continue;
     if( (*iter).getSkill()->getBasicSkill() != skill->getBasicSkill() )
      return false;
       
   }
   
   return true;
}



bool FollowerRaceRule::teacherRequired(SkillRule * skill, UnitEntity * unit)
{
  if(skill->isCombatSkill())  // if it is a combat skill and level >= 2
   {
    if(unit->getSkillLevel(skill) >= 2)
      return true;
    else
      return false;
   }
  else // if it is any other skill and level >= 1
  {
    if( unit->getSkillLevel(skill) >= 1)
      return true;
    else
    return false;
  } 
}



/*
 *  Intristic ability of unit to study skill
 *  Level limitations also taken into considerations
 */
extern SkillRule * combatSkill; 
LEARNING_RESULT FollowerRaceRule::mayLearn(SkillRule * skill, UnitEntity * unit)
{
  int  level = unit->getSkillLevel(skill);
// Follower may not study magic skills
   if(skill->isMagicSkill())
	{
		if(unit->isTraced())
		{
                  cerr << unit->printTag()<<" Can't study magic skill "<<skill->print()<<endl;
		}
     	return CANNOT_STUDY_FAILURE;
	}
        if(skill->isRacialEnabled(this))
        {
          if(unit->isTraced())
          {
            cerr << " Can't study Racial skill "<<skill->print()<<endl;
          }
          return CANNOT_STUDY_FAILURE;
        }
        int bonus = unit->getLearningLevelBonus(skill);
// Follower may not study non-combat skill above 1-st level
// and combat skill above 2-nd level
  if(level >= 1 + bonus)
  {
   if(!skill->isCombatSkill())
     return TEACHING_REQUIRED;
   else
    if(level >=2+ bonus)
     return TEACHING_REQUIRED;
  }
  
  if (!skillCompartibilityCheck( skill, unit) )
   return FOLLOWER_CANNOT_STUDY_SECOND_BASIC_SKILL_FAILURE;
 
  else
   return LEARNING_OK;

}

bool FollowerRaceRule::mayMove(UnitEntity * unit)
{
  SkillRule * scouting = skills["scou"];
  if (unit->hasSkillLevel(scouting,1))
    return true;
  else  
    return false;
}

/** How many seats occupies this entity in the class. Number of entities that can be tought by one teacher determined by this value. */
int FollowerRaceRule::getLearningCapacity()
{
  return 2;
}



void FollowerRaceRule::printTypeSpecificDescription(ReportPrinter & out)
{
  out << " This is a follower.";
}

