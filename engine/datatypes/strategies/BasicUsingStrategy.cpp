/***************************************************************************
                          BasicUsingStrategy.cpp 
                             -------------------
    begin                : Wed Feb 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "BasicUsingStrategy.h"
#include "PhysicalEntity.h"
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



void BasicUsingStrategy::addUsingExperience(PhysicalEntity * tokenEntity, SkillElement & skill)
{
   if(tokenEntity->isTraced())
   {
    cout <<"== TRACING " <<tokenEntity->printTag()<< " ==>  " << skill.getExpPoints()<<" using experience added to " << skill.getSkill()->printTag()<<endl;
   }

   if(!tokenEntity->mayGainExperience())
    return;

    tokenEntity->addSkill(skill);
   // add experience to parent skills
   int exp = skill.getExpPoints()/2; // This is the difference from learning

   int level = tokenEntity->getSkillLevel(skill.getSkill());
   int tryLevel;
   for (tryLevel = 0; tryLevel<= level ; tryLevel++)
    {
      SkillLevelElement * requirement = skill.getSkill()->getRequirement(tryLevel);
      if(requirement != 0)
        {
          SkillElement recursive(requirement->getSkill(),exp);
          addRecursiveUsingExperience(tokenEntity,recursive);
        }
     }

}



void BasicUsingStrategy::addRecursiveUsingExperience(PhysicalEntity * tokenEntity, SkillElement  & skill)
{
   if(tokenEntity->isTraced())
   {
    cout <<"== TRACING " <<tokenEntity->printTag()<< " ==>  " << skill.getExpPoints()<<" recursive using experience added to " << skill.getSkill()->printTag()<<endl;
   }
    tokenEntity->addSkill(skill);

   int level = tokenEntity->getSkillLevel(skill.getSkill());
   int tryLevel;
   for (tryLevel = 0; tryLevel< level ; tryLevel++)
    {
      SkillLevelElement * requirement = skill.getSkill()->getRequirement(tryLevel);
      if(requirement != 0)
        {
          addRecursiveUsingExperience(tokenEntity,skill);
        }
     }

}



int BasicUsingStrategy::expBase_ = 10;



int
BasicUsingStrategy::calculateUsingExperience(PhysicalEntity * tokenEntity, SkillRule * skill)
{
// Amount of experience tokenEntity getting from skill use may vary but in Overlord it
// is constant - 1/10 of base.


 int exp = BasicUsingStrategy::expBase_;     // normal base 10 points for a day,

   if(tokenEntity->isGuarding())
    exp /= 2;
   return  exp;
}



USING_RESULT BasicUsingStrategy::unitMayUse(UnitEntity * unit, SkillRule * skill)
{
  return  UNUSABLE;
}



USING_RESULT BasicUsingStrategy::mayUse(PhysicalEntity * tokenEntity, SkillRule * skill)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(tokenEntity);
  if(unit)
     return unitMayUse(unit, skill);
  else
     return CANNOT_USE;
}



USING_RESULT BasicUsingStrategy::use(PhysicalEntity * tokenEntity, SkillRule * skill, int & useCounter)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(tokenEntity);
  if(unit)
     return unitUse(unit, skill,useCounter);
  else
     return CANNOT_USE;
}






USING_RESULT BasicUsingStrategy::unitUse(UnitEntity * unit, SkillRule * skill, int & useCounter)
{
  // report Skill can't be used
  return UNUSABLE;
}






void    BasicUsingStrategy::extractKnowledge (Entity * recipient, int parameter)
{
}



void BasicUsingStrategy::reportUse(USING_RESULT result, PhysicalEntity * tokenEntity)
{
}



int BasicUsingStrategy::getUseDuration()
{
  return 1;
}
