/***************************************************************************
                          BasicLearningStrategy.cpp
                             -------------------
    begin                : Mon Feb 10 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#include "BasicLearningStrategy.h"
#include "TokenEntity.h"
#include "LocationEntity.h"
#include "RaceRule.h"
#include "SeasonRule.h"
#include "WeatherRule.h"
#include "BonusElement.h"
#include "TeachingOffer.h"
#include "ConstructionEntity.h"
#include "ConstructionRule.h"
BasicLearningStrategy     sampleLearning          ("LEARNING",           &sampleGameData);

BasicLearningStrategy::BasicLearningStrategy ( const BasicLearningStrategy * prototype ) : Strategy(prototype)
{
   racialClass_ = 0;
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
      bonusItem_   = InventoryElement::readElement(parser);
      return OK;
    }
  if (parser->matchKeyword ("ENABLE") )
    {
      itemRequired_   = InventoryElement::readElement(parser);
      return OK;
    }
  if (parser->matchKeyword ("STUDENT") )
    {
      // Skill is available for some race class only
      string type = parser->getWord();
      if(!type.empty())
       racialClass_ = dynamic_cast<Rule *>(createByKeyword(type)) ;
      return OK;
    }
      return OK;


}



bool BasicLearningStrategy::teacherRequired(Entity * entity, SkillRule * skill)
{
  if( special_ )
    {
        return true;
    }

    return entity->teacherRequired(skill);
}


/*
 * Ability of entity to study skill
 *  Level limitations also taken into considerations
 */
LEARNING_RESULT BasicLearningStrategy::mayStudy(TokenEntity * tokenEntity, SkillRule * skill) const
{
// Check that Entity is able to study this skill at current level
    LEARNING_RESULT  result = tokenEntity->mayLearn(skill);

      if (result != LEARNING_OK)
        return   result;


// Some skills may be studied only by specific races or types of Entities
  if( (racialClass_ != 0) && !tokenEntity->isOfType(racialClass_))
      return RACE_FAILURE;

// Some skills may require item in order to be studied
   if(itemRequired_)
   {
      if(!tokenEntity->isEquiped(itemRequired_))
          return ITEM_REQUIRED_FAILURE;

   }

  SkillLevelElement * requirement =
			 skill->getRequirement(tokenEntity->getSkillLevel(skill));
  if(requirement)
    {
      if ( ! tokenEntity->hasSkill(requirement))
        return REQUIREMENT_FAILURE;
    }
  // Special case of magecraft skill limitations will be considered in
  // special MagecraftLearningStrategy

  if(tokenEntity->hasSkill(skill->getMax()) )
    return MAX_LEVEL_FAILURE;
return LEARNING_OK;
}
/** applies all bonuses and returns number of experience points gained by one day
  * of learning
  */
int BasicLearningStrategy::leaderBonus_ = 10;
int BasicLearningStrategy::expBase_ = 100;



int BasicLearningStrategy::calculateLearningExperience(TokenEntity * tokenEntity, SkillRule * skill, TeachingOffer * teacher)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(tokenEntity);
  if(unit)
    return calculateUnitLearningExperience(unit,skill,teacher);
  else
    return 0;
}



int  BasicLearningStrategy::calculateUnitLearningExperience(UnitEntity * unit, SkillRule * skill, TeachingOffer * teacher)
{
 int exp = BasicLearningStrategy::expBase_; // normal base 100 points for a day,
 UnitEntity * leader;
 bool needsTeacher = skill->teacherRequired(unit);
 if( needsTeacher)
  exp = 0; // slow study for skills above normal limit
   // apply study bonuses:
   // 1. Location bonuses
	 LocationEntity * location = unit -> getLocation();
   exp += location->getBonus(skill);
   // 2. Climate season and terran bonus
   exp += location->getWeather()->getStudyBonus(skill);
   exp += location->getSeason()->getStudyBonus(skill);
   exp += location->getTerrain()->getStudyBonus(skill);
   // 3. Building bonuses
   ConstructionEntity * building = unit ->getContainingConstruction();
   if(building)
   {
    exp += building->getConstructionType()->getStudyBonus(skill);
   }
   // 4. Stack leader bonus
   for (leader = unit->getLeader(); leader != 0; leader = leader->getLeader() )
    {
      if (leader->hasSkillLevel(skill, unit->getSkillLevel(skill)))
        {
            exp += BasicLearningStrategy::leaderBonus_;
            break;
        }
    }
   // 5. Title Bonus
   exp += unit->getTitleStudyBonus(skill);
   // 6. Race Bonus
    exp += unit->getRace()->getStudyBonus(skill);
   // 7. Item Bonus
   if(bonusItem_ != 0) // This for the case when bonus defined in skill data
   exp += (20 * bonusItem_->getEquipedNumber())/(unit->getFiguresNumber());
	 exp += unit->getItemStudyBonus(skill);
   // 8. Teacher Bonus
   if(teacher != 0)
   exp += teacher->getTeachingBonus();
   // 9. Skill Bonus
   exp += unit->getSkillStudyBonus(skill);
   // 10. Enchantment Bonus
   exp += unit->getEnchantmentStudyBonus(skill);
   // Guarding unit gets 1/2
   if(unit->isGuarding())
    exp /= 2;
   // add experience
   if(exp < 0)
      exp = 0;
   return  exp;
}



void BasicLearningStrategy::addLearningExperience(TokenEntity * tokenEntity, SkillElement & skill)
{
   if(tokenEntity->isTraced())
   {
    cout <<"== TRACING " <<tokenEntity->printTag()<< " ==>  " << skill.getExpPoints()<<" learning experience added to " << skill.getSkill()->printTag()<<endl;
   }
   // This is additional check. Really ability of entity to learn was
   // already checked before studying started, but for the cases of group
   // study (like staff training) study may start if at least some entities
   // may learn. Entities unable to learn should not benefit from that
   if(tokenEntity->mayLearn(skill.getSkill()) != LEARNING_OK)
    return;
    tokenEntity->addSkill(skill);
   // add experience to parent skills
   int exp = skill.getExpPoints()/10;

   int level = tokenEntity->getSkillLevel(skill.getSkill());
   int tryLevel;
   for (tryLevel = 0; tryLevel<= level ; tryLevel++)
    {
      SkillLevelElement * requirement = skill.getSkill()->getRequirement(tryLevel);
      if(requirement != 0)
        {
          SkillElement recursive(requirement->getSkill(),exp);
          addRecursiveLearningExperience(tokenEntity,recursive);
        }
     }
      //  problem with teaching requests
}
void BasicLearningStrategy::addRecursiveLearningExperience(TokenEntity * tokenEntity, SkillElement  & skill)
{
   if(tokenEntity->isTraced())
   {
    cout <<"== TRACING " <<tokenEntity->printTag()<< " ==>  " << skill.getExpPoints()<<" recursive learning experience added to " << skill.getSkill()->printTag()<<endl;
   }
    tokenEntity->addSkill(skill);

   int level = tokenEntity->getSkillLevel(skill.getSkill());
   int tryLevel;
   for (tryLevel = 0; tryLevel< level ; tryLevel++)
    {
      SkillLevelElement * requirement = skill.getSkill()->getRequirement(tryLevel);
      if(requirement != 0)
        {
          addRecursiveLearningExperience(tokenEntity,skill);
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
