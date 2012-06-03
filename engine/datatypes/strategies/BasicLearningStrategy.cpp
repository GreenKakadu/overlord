/***************************************************************************
                          BasicLearningStrategy.cpp
                             -------------------
    begin                : Mon Feb 10 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
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
   //name_ = "a generic skill";
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



void BasicLearningStrategy::save(ostream &out)
{
    if(special_) out<< "SPECIALIST"<<endl;
    if(bonusItem_)
    {
        out<< "WISDOM"<<" ";
        bonusItem_->save(out);
    }
    if(itemRequired_)
    {
        out<< "ENABLE"<<" ";
        itemRequired_->save(out);
    }

    if(racialClass_) out<< "STUDENT"<<" "<<racialClass_->getKeyword() << endl;
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
LEARNING_RESULT BasicLearningStrategy::mayStudy(TokenEntity * tokenEntity, SkillRule * skill) 
{
// Check that Entity is able to study this skill at current level
    LEARNING_RESULT  result = tokenEntity->mayLearn(skill);

      if (result != LEARNING_OK)
        return   result;


// Some skills may be studied only by specific races or types of Entities
  if( (racialClass_ != 0) && !tokenEntity->isOfType(racialClass_))
{
//	cout << "Race failure"<<endl; // This is used in printing 'May learn"
      return RACE_FAILURE;
}
// Some skills may require item in order to be studied
   if(itemRequired_)
   {
      if(!tokenEntity->isEquiped(itemRequired_))
          return ITEM_REQUIRED_FAILURE;

   }
// Meet requirement to learn NEXT level?
  SkillLevelElement * requirement =
			 skill->getRequirement(tokenEntity->getSkillLevel(skill) +1);
  if(requirement)
    {
      if ( ! tokenEntity->hasSkill(requirement))
        return REQUIREMENT_FAILURE;
    }
  // Special case of magecraft skill limitations will be considered in
  // special MagecraftLearningStrategy
if(tokenEntity->isTraced())
{
//cout <<"?";
}
  if(tokenEntity->hasSkill(skill->getMax()) )
    return MAX_LEVEL_FAILURE;
if(tokenEntity->isTraced())
{
//cout <<"!";
}
  if(skill->isElementalMagicSkill())
{
  int magecraftLevel = tokenEntity->getSkillLevel(gameFacade->skills["mage"]);
  int elementalSkillKnown = 0;
  int fireLevel = tokenEntity->getSkillLevel(gameFacade->skills["fire"]);
  if(fireLevel >= 1)
    elementalSkillKnown++;
  int airLevel = tokenEntity->getSkillLevel(gameFacade->skills["airs"]);
  if( airLevel >= 1)
    elementalSkillKnown++;
  int waterLevel = tokenEntity->getSkillLevel(gameFacade->skills["wate"]);
  if( waterLevel >= 1)
    elementalSkillKnown++;
  int earthLevel = tokenEntity->getSkillLevel(gameFacade->skills["eart"]);
  if( earthLevel >= 1)
    elementalSkillKnown++;
  int voidLevel = tokenEntity->getSkillLevel(gameFacade->skills["void"]);
  if( voidLevel >= 1)
    elementalSkillKnown++;
  
  if(magecraftLevel <= elementalSkillKnown)
    return ELEMENTAL_SKILL_LIMIT_FAILURE;
  else
    return   LEARNING_OK;
}
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
 if(unit->isTraced())
 {
   cout << "calculateUnitLearningExperience for "<<unit->print()<<endl;
 }
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
      if (leader->hasSkillLevel(skill, unit->getSkillLevel(skill)+1))
        {
//  if(unit->isTraced())
//  {
//    cout << "Bonus from "<<leader->print()<<" for "<<skill->print() <<" " <<unit->getSkillLevel(skill) <<endl; 
//  }
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

   LEARNING_RESULT result = tokenEntity->mayLearn(skill.getSkill());
   if((result != LEARNING_OK) && (result != TEACHING_REQUIRED))
    return;
    tokenEntity->addSkill(skill);
   // add experience to parent skills
   int exp = skill.getExpPoints()/10;

   int level = tokenEntity->getSkillLevel(skill.getSkill());
   int tryLevel;
   vector<SkillRule *> reqList;
   for (tryLevel = 0; tryLevel<= level ; tryLevel++)
    {
      SkillLevelElement * requirement = skill.getSkill()->getRequirement(tryLevel);
      if(requirement != 0)
        {
          if(reqList.end() == find(reqList.begin(),reqList.end(),requirement->getSkill()))
          {
            SkillElement recursive(requirement->getSkill(),exp);
            reqList.push_back(requirement->getSkill());
            addRecursiveLearningExperience(tokenEntity,recursive);
          }
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
   vector<SkillRule *> reqList;
   for (tryLevel = 0; tryLevel< level ; tryLevel++)
    {
      SkillLevelElement * requirement = skill.getSkill()->getRequirement(tryLevel);
       if(requirement != 0)
        {
          if(reqList.end() == find(reqList.begin(),reqList.end(),requirement->getSkill()))
          {
          SkillElement recursive(requirement->getSkill(),skill.getExpPoints());
          reqList.push_back(requirement->getSkill());
//  	if(tokenEntity->isTraced())
//   	{
// 	SkillElement * se =tokenEntity->getSkillElement(requirement->getSkill());
// 	cout << "requirement: " <<requirement->getSkill()->printTag()<<" -> "<< se->getSkill()->printTag();
// 	cout<< " + "<<se->getExpPoints() <<endl;
//	}
          addRecursiveLearningExperience(tokenEntity, recursive );
          }
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



BasicLearningStrategy * BasicLearningStrategy::cloneSelf()
{
 BasicLearningStrategy * copyOfSelf = new BasicLearningStrategy(keyword_,parent_);
 *copyOfSelf = *this;
 return copyOfSelf;
}
