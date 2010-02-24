/***************************************************************************
                          BasicUsingStrategy.cpp
                             -------------------
    begin                : Wed Feb 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "BasicUsingStrategy.h"
#include "TokenEntity.h"
#include "UnitEntity.h"

BasicUsingStrategy        sampleUsing             ("USING",              &sampleGameData);
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



void BasicUsingStrategy::addUsingExperience(TokenEntity * tokenEntity, SkillElement & skill)
{
//    if(tokenEntity->isTraced())
//    {
//     cout <<"== TRACING " <<tokenEntity->printTag()<< " ==>  " << skill.getExpPoints()<<" using experience added to " << skill.getSkill()->printTag()<<endl;
//    }

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
          addRecursiveUsingExperience(tokenEntity,requirement->getSkill(),exp);
        }
     }

}



void BasicUsingStrategy::addRecursiveUsingExperience(TokenEntity * tokenEntity,  SkillRule * skill, int experience)
{
/*   if(tokenEntity->isTraced())
   {
    cout <<"== TRACING " <<tokenEntity->printTag()<< " ==>  " << experience<<" recursive using experience added to " << skill->printTag()<<endl;
   }*/
    tokenEntity->addSkill(skill,experience);

   int level = tokenEntity->getSkillLevel(skill);
   int tryLevel;
   for (tryLevel = 0; tryLevel< level ; tryLevel++)
    {
      SkillLevelElement * requirement = skill->getRequirement(tryLevel);
      if(requirement != 0)
        {
          addRecursiveUsingExperience(tokenEntity,requirement->getSkill(),experience);
        }
     }

}



int BasicUsingStrategy::expBase_ = 10;



int
BasicUsingStrategy::calculateUsingExperience(TokenEntity * tokenEntity, SkillRule * skill)
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


/*
 * This is a part of simplified double dispatching mechanism for skill use.
 * for each type of entity using skill appropriate method should be called
 * currently only units may use skills
 */

USING_RESULT BasicUsingStrategy::mayUse(TokenEntity * tokenEntity, SkillRule * skill)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(tokenEntity);
  if(unit)
     return unitMayUse(unit, skill);
  else
     return CANNOT_USE;
}


/*
 * This is a part of simplified double dispatching mechanism for skill use.
 * for each type of entity using skill appropriate method should be called
 * currently only units may use skills
 */

USING_RESULT BasicUsingStrategy::use(TokenEntity * tokenEntity, SkillRule * skill, int & useCounter,
					OrderLine * order)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(tokenEntity);
  if(unit)
     return unitUse(unit, skill,useCounter,order);
  else
     return CANNOT_USE;
}






USING_RESULT BasicUsingStrategy::unitUse(UnitEntity * unit, SkillRule * skill, 
	int & useCounter,OrderLine * order)
{
  // report Skill can't be used
    if(unit->isTraced())
    {
        cout << "BasicUsingStrategy::unitUse"<<endl;
    }
  return UNUSABLE;
}

int BasicUsingStrategy::calculateProductionBonus(TokenEntity * tokenEntity, SkillRule * skill)
{
	return tokenEntity->getProductionBonus(skill);
}





void    BasicUsingStrategy::extractKnowledge (Entity * recipient, int parameter)
{
}



void BasicUsingStrategy::reportUse(USING_RESULT result, TokenEntity * tokenEntity)
{
}



int BasicUsingStrategy::getUseDuration()
{
  return 1;
}

BasicUsingStrategy * BasicUsingStrategy::cloneSelf()
{
 BasicUsingStrategy * copyOfSelf = new BasicUsingStrategy(keyword_,parent_);
 *copyOfSelf = *this;
 return copyOfSelf;
}
