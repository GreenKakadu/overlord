/***************************************************************************
                          MagicLearningStrategy.cpp 
                             -------------------
    begin                : Tue Feb 18 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "MagicLearningStrategy.h"
#include "TokenEntity.h"
//#include "SkillRule.h"
MagicLearningStrategy     sampleMagicLearning     ("LEARNING_ELEMENTAL_MAGIC",     &sampleLearning);

MagicLearningStrategy::MagicLearningStrategy( const MagicLearningStrategy * prototype ):
           BasicLearningStrategy(prototype)
{
  name_ = "elemental magic skill";
}



GameData * MagicLearningStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<MagicLearningStrategy> (this);
}



STATUS
MagicLearningStrategy::initialize        ( Parser *parser )
{
      return BasicLearningStrategy::initialize(parser);
}

LEARNING_RESULT MagicLearningStrategy::mayStudy(TokenEntity * tokenEntity, SkillRule * skill)
{
  LEARNING_RESULT  result = BasicLearningStrategy::mayStudy(tokenEntity, skill);
	    if (result != LEARNING_OK)
        return   result;
            
//            if (tokenEntity->isTraced())
//              cout <<"== TRACING " << skill->print()<< "MagicLearningStrategy: May learn " << endl; 
        if(skill->isElementalMagicSkill())
        {
          int magecraftLevel = tokenEntity->getSkillLevel(skills["mage"]);
          int elementalSkillKnown = 0;
          int fireLevel = tokenEntity->getSkillLevel(skills["fire"]);
          if(fireLevel >= 1)
                  elementalSkillKnown++;
          int airLevel = tokenEntity->getSkillLevel(skills["airs"]);
          if( airLevel >= 1)
                  elementalSkillKnown++;
          int waterLevel = tokenEntity->getSkillLevel(skills["wate"]);
          if( waterLevel >= 1)
                  elementalSkillKnown++;
          int earthLevel = tokenEntity->getSkillLevel(skills["eart"]);
          if( earthLevel >= 1)
                  elementalSkillKnown++;
          int voidLevel = tokenEntity->getSkillLevel(skills["void"]);
          if( voidLevel >= 1)
                  elementalSkillKnown++;
  
          if(magecraftLevel <= elementalSkillKnown)
                          return ELEMENTAL_SKILL_LIMIT_FAILURE;
          else
          return   LEARNING_OK;
        }
        else
          return   LEARNING_OK;
}


BasicLearningStrategy * MagicLearningStrategy::cloneSelf()
{
 MagicLearningStrategy * copyOfSelf = new MagicLearningStrategy(keyword_,parent_);
 *copyOfSelf = *this;
 return copyOfSelf;
}
