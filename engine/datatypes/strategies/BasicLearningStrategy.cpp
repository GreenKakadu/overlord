/***************************************************************************
                          BasicLearningStrategy.cpp 
                             -------------------
    begin                : Mon Feb 10 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#include "BasicLearningStrategy.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "RaceRule.h"
#include "BonusElement.h"
#include "TeachingOffer.h"

BasicLearningStrategy::BasicLearningStrategy ( const BasicLearningStrategy * prototype ) : Strategy(prototype)
{
   racial_ = 0;
   special_ = false;
   itemRequired_ = 0;
   bonusItem_ = 0;
}

GameData * BasicLearningStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<BasicLearningStrategy> (this);
}


STATUS
BasicLearningStrategy::initialize        ( Parser *parser )
{
  if (parser->matchKeyword ("SPECIALIST") )
    {
			special_ = true;
      return OK;
    }
  if (parser->matchKeyword ("WISDOM") )
    {
//       = parser->get();
      return OK;
    }
  if (parser->matchKeyword ("ENABLE") )
    {
//       = parser->getInteger();
      return OK;
    }
  if (parser->matchKeyword ("STUDENT") )
    {
//       = parser->getInteger();
      return OK;
    }
      return OK;


}
bool BasicLearningStrategy::teacherRequired(Entity * unit, SkillRule * skill) 
{
  if( special_ )
    {
        return true;
    }

    return unit->teacherRequired(skill);
}
/** No descriptions */
LEARNING_RESULT BasicLearningStrategy::mayLearn(UnitEntity * unit, SkillRule * skill) const
{
    LEARNING_RESULT  result = unit->getRace()->mayLearn(skill, unit);

      if (result != LEARNING_OK)  
        return   result;

  if( (racial_ != 0) && (racial_ != unit->getRace()))
    return RACE_FAILURE;
      SkillLevelElement * requirement = skill->getRequirement(unit->getSkillLevel(skill));
  if(requirement)
    {
      if ( ! unit->hasSkill(requirement)) 
        return REQUIREMENT_FAILURE;
    }
  // Special case of magecraft skill limitations will be considered in
  // special MagecraftLearningStrategy 

  if(unit->hasSkill(skill->getMax()) )
    return MAX_LEVEL_FAILURE;         
return LEARNING_OK;
}
/** applies all bonuses and returns number of experience points gained by one day 
  * of learning
  */
int BasicLearningStrategy::leaderBonus_ = 10;
int BasicLearningStrategy::expBase_ = 100;

int 
BasicLearningStrategy::calculateLearningExperience(UnitEntity * unit, SkillRule * skill, TeachingOffer * teacher)
{
 int exp = BasicLearningStrategy::expBase_; // normal base 100 points for a day,
 UnitEntity * leader;
 bool needsTeacher = skill->teacherRequired(unit);
 if( needsTeacher)
  exp = 0; // slow study for skills above normal limit
   // apply study bonuses:
   // 1. Location bonus
   exp += unit -> getLocation()->getBonus(skill);
   // 2. Stack leader bonus
   for (leader = unit->getLeader(); leader != 0; leader = leader->getLeader() )
    {
      if (leader->hasSkill(skill, unit->getSkillLevel(skill)))
        {
            exp += BasicLearningStrategy::leaderBonus_;
            break;
        }
    }
   // 3. Climate bonus
   // 4. Title Bonus
   // 5. Race Bonus
   exp += unit->getRace()->getBonus(skill); 
   // 6. Item Bonus
   if(bonusItem_ != 0)
   exp += (20 * bonusItem_->getEquipedNumber())/(unit->getFiguresNumber());
   // 6. Teacher Bonus
   if(teacher != 0)
   exp += teacher->getTeachingBonus();
   // Guarding unit gets 1/2
   if(unit->isGuarding())
    exp /= 2;
   // add experience
   return  exp;
}



//void BasicLearningStrategy::addLearningExperience(UnitEntity * unit, SkillRule * skill, int exp);
//{
//   if(unit->isTraced())
//   {
//    cout <<"== TRACING " <<unit->printTag()<< " ==>  " << skill.getExpPoints()<<" learning experience added to " << skill.getSkill()->printTag()<<endl;
//   }
//    unit->addSkill(skill);
//   // add experience to parent skills
//   int exp = skill.getExpPoints()/10;
//
//   int level = unit->getSkillLevel(skill.getSkill());
//   int tryLevel;
//   for (tryLevel = 0; tryLevel<= level ; tryLevel++)
//    {
//      SkillLevelElement * requirement = skill.getSkill()->getRequirement(tryLevel);
//      if(requirement != 0)
//        {
//          SkillElement recursive(requirement->getSkill(),exp);
//          addRecursiveLearningExperience(unit,recursive);
//        }
//     }
//      //  problem with teaching requests
//}



void BasicLearningStrategy::addLearningExperience(UnitEntity * unit, SkillElement & skill)
{
   if(unit->isTraced())
   {
    cout <<"== TRACING " <<unit->printTag()<< " ==>  " << skill.getExpPoints()<<" learning experience added to " << skill.getSkill()->printTag()<<endl;
   }
    unit->addSkill(skill);
   // add experience to parent skills
   int exp = skill.getExpPoints()/10;

   int level = unit->getSkillLevel(skill.getSkill());
   int tryLevel;
   for (tryLevel = 0; tryLevel<= level ; tryLevel++)
    {
      SkillLevelElement * requirement = skill.getSkill()->getRequirement(tryLevel);
      if(requirement != 0)
        {
          SkillElement recursive(requirement->getSkill(),exp);
          addRecursiveLearningExperience(unit,recursive);
        }
     }  
      //  problem with teaching requests
}
void BasicLearningStrategy::addRecursiveLearningExperience(UnitEntity * unit, SkillElement  & skill)
{
   if(unit->isTraced())
   {
    cout <<"== TRACING " <<unit->printTag()<< " ==>  " << skill.getExpPoints()<<" recursive learning experience added to " << skill.getSkill()->printTag()<<endl;
   }
    unit->addSkill(skill);
    
   int level = unit->getSkillLevel(skill.getSkill());
   int tryLevel;
   for (tryLevel = 0; tryLevel< level ; tryLevel++)
    {
      SkillLevelElement * requirement = skill.getSkill()->getRequirement(tryLevel);
      if(requirement != 0)
        {
          addRecursiveLearningExperience(unit,skill);
        }
     }
  
}
void    BasicLearningStrategy::extractKnowledge (Entity * recipient, int parameter)
{
  if(itemRequired_)
    {
      if(itemRequired_->getItemType())
      {
        if(recipient->addKnowledge(itemRequired_->getItemType()))
          itemRequired_->getItemType()->extractKnowledge(recipient);
      }
    }
  if(bonusItem_)
    {
      if(bonusItem_->getItemType())
      {
        if(recipient->addKnowledge(bonusItem_->getItemType()))
          bonusItem_->getItemType()->extractKnowledge(recipient);
      }
    }
}
