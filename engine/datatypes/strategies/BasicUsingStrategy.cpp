/***************************************************************************
                          BasicUsingStrategy.cpp 
                             -------------------
    begin                : Wed Feb 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "BasicUsingStrategy.h"
#include "UnitEntity.h"

BasicUsingStrategy::BasicUsingStrategy ( const BasicUsingStrategy * prototype ) : Strategy(prototype)
{

}


GameData * BasicUsingStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<BasicUsingStrategy> (this);
}

STATUS
BasicUsingStrategy::initialize        ( Parser *parser )
{
  if (parser->matchKeyword ("SPECIALIST") )
    {
//			special_ = true;
      return OK;
    }
      return OK;


}
void BasicUsingStrategy::addUsingExperience(UnitEntity * unit, SkillElement & skill)
{
   if(unit->isTraced())
   {
    cout <<"== TRACING " <<unit->printTag()<< " ==>  " << skill.getExpPoints()<<" using experience added to " << skill.getSkill()->printTag()<<endl;
   }
    unit->addSkill(skill);
   // add experience to parent skills
   int exp = skill.getExpPoints()/2; // This is the difference from learning

   int level = unit->getSkillLevel(skill.getSkill());
   int tryLevel;
   for (tryLevel = 0; tryLevel<= level ; tryLevel++)
    {
      SkillLevelElement * requirement = skill.getSkill()->getRequirement(tryLevel);
      if(requirement != 0)
        {
          SkillElement recursive(requirement->getSkill(),exp);
          addRecursiveUsingExperience(unit,recursive);
        }
     }

}
void BasicUsingStrategy::addRecursiveUsingExperience(UnitEntity * unit, SkillElement  & skill)
{
   if(unit->isTraced())
   {
    cout <<"== TRACING " <<unit->printTag()<< " ==>  " << skill.getExpPoints()<<" recursive using experience added to " << skill.getSkill()->printTag()<<endl;
   }
    unit->addSkill(skill);

   int level = unit->getSkillLevel(skill.getSkill());
   int tryLevel;
   for (tryLevel = 0; tryLevel< level ; tryLevel++)
    {
      SkillLevelElement * requirement = skill.getSkill()->getRequirement(tryLevel);
      if(requirement != 0)
        {
          addRecursiveUsingExperience(unit,skill);
        }
     }

}
int BasicUsingStrategy::expBase_ = 10;



int
BasicUsingStrategy::calculateUsingExperience(UnitEntity * unit, SkillRule * skill)
{
// Amount of experience unit getting from skill use may vary but in Overlord it
// is constant - 1/10 of base.


 int exp = BasicUsingStrategy::expBase_;     // normal base 10 points for a day,

   if(unit->isGuarding())
    exp /= 2;
   // add experience
   return  exp;
}

USING_RESULT BasicUsingStrategy::mayUse(UnitEntity * unit, SkillRule * skill)
{
  return  UNUSABLE;
}


bool BasicUsingStrategy::use(UnitEntity * unit, Order * OrderId)
{
  // report Skill can't be used
  return false;
}



void    BasicUsingStrategy::extractKnowledge (Entity * recipient, int parameter)
{
}



void BasicUsingStrategy::reportUse(USING_RESULT result, UnitEntity * unit, Order * OrderId)
{
}
