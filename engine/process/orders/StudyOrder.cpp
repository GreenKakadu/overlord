/***************************************************************************
                          StudyOrder.cpp 
                             -------------------
    begin                : Thu Feb 13 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#include "StudyOrder.h"
#include "IntegerData.h"
#include "SkillLevelElementData.h"
#include "BasicLearningStrategy.h"
#include "SkillRule.h"
#include "RaceRule.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "FactionEntity.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "TertiaryPattern.h"
#include "TeachingOffer.h"

extern RulesCollection <SkillRule>     skills;
extern Reporter * cannotStudyReporter;
extern Reporter * raceErrorReporter;
extern Reporter * requirementErrorReporter;
extern Reporter * teachingErrorReporter;
extern Reporter * maxLevelErrorReporter;
extern Reporter * paymentErrorReporter;
extern Reporter * learningStartedReporter;
extern Reporter * followerSkillLimitReporter ;

StudyOrder::StudyOrder()
{
   keyword_ = "study";
  description = string("STUDY <skill tag> [level] \n") +
  "Full-day.  This order executes as soon as the requirement in other skills\n" +
  "are fulfilled, if any.  The order fails if the unit cannot learn the skill\n"  +
  "at all.  Creatures cannot use this order.  The unit adds one day of study of\n" +
  "the skill to its experience, spending the indicated amount of coins per\n"  +
  "figure.  If a skill level is indicated, and no duration specified for the\n" +
  "order, the order will reschedule itself automatically.\n\n" +

  "Bonuses to study may occur, see the section on skills for more.\n" +
  "If the faction knows the skill, but the unit lacks a required skill level,\n" +
  "the STUDY will first attempt to study the intermediate skills if this is\n" +
  "possible.  Teaching of intermediate skills will not be allowed; the study\n"  +
  "must be explicit for the teacher to notice.\n\n" +

  "The order will complete as soon as the unit can no longer study the skill, or\n" +
  "as soon as the skill level indicated is reached, whichever comes first.\n";

  orderType_   = DAY_LONG_ORDER;
}
STATUS StudyOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, skills, "skill tag", parameters))
            return IO_ERROR;

    parseIntegerParameter(parser, parameters);

  return OK;
     

}


ORDER_STATUS StudyOrder::process (Entity * entity, vector <AbstractData *>  &parameters, Order * orderId)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);

  SkillRule * skill = dynamic_cast<SkillRule *>(parameters[0]);

   if ( skill == 0)
    {
	    ReportRecord * currentReport = new   ReportRecord(new UnaryPattern(cannotStudyReporter, unit->getRace()));
		  unit->addReport( currentReport);
 		return INVALID;
    }

   int level;
   if(parameters.size() > 1)
      {
        IntegerData * par1       =  dynamic_cast<IntegerData *>(parameters[1]);
          assert(par1);
        level  =  par1->getValue();
        if (level > skill->getMaxLevel())
              level = skill->getMaxLevel();
      }
    else
      level = skill->getMaxLevel();


 TeachingOffer * teacher;
 PROCESSING_STATE  state = orderId->getProcessingState();
 ORDER_STATUS result;

  switch(state)
   {
     case NORMAL_STATE:
     // check if order may be executed
     result = preProcess_(unit,skill,level,orderId);
      if( (result == INVALID) ||( result == FAILURE))
      {
        unit->clearTeachingOffers();
        return result;
      }
      teacher = unit->findTeachingOffer(skill,level);

      if(teacher)
          {
            teacher->confirmTeachingOffer(unit);
            orderId->setProcessingState (RESUME); 
            unit->getLocation()->setStudentCounter(true);
            return SUSPENDED;
          }
          
      orderId->setProcessingState (SUSPEND);
      unit->getLocation()->setStudentCounter(true);
      return SUSPENDED;
      break;

     case SUSPEND:
        teacher = unit->findTeachingOffer(skill,level);
        if(teacher)
          {
            teacher->confirmTeachingOffer(unit);
            orderId->setProcessingState (RESUME);
            return SUSPENDED;
          }
         if(unit->getLocation()->getTeacherCounter() )
         {
            return SUSPENDED; // wait. There are other orders suspended and their 
                              // results may have effect on this order execution
         }
            orderId->setProcessingState (RESUME);
            return SUSPENDED;
        break;

     case RESUME:
        teacher = unit->findTeachingOffer(skill,level);
        unit->getLocation()->setStudentCounter(false);
        orderId->setProcessingState (NORMAL_STATE);
        unit->clearTeachingOffers();
        return doProcess_(unit,skill,level,teacher,orderId);
        break;

     default:
      cout << "Unknown state "<< state <<" in STUDY order\n";
   }
 		  return INVALID; 		     
}      


/** Checks if study order may be processed (except teaching condition)*/
ORDER_STATUS StudyOrder::preProcess_(UnitEntity * unit, SkillRule * skill, int level, Order * orderId)
{

 LEARNING_RESULT result = skill->mayStudy(unit);

 switch (result)
  {
    case LEARNING_OK:
    {
      break;
    }
    case CANNOT_STUDY_FAILURE:
    {
	    UnaryPattern * Message = new UnaryPattern(cannotStudyReporter, unit->getRace());
	    ReportRecord * currentReport = new   ReportRecord(Message, orderId);
		  unit->addReport( currentReport);
 		  return INVALID;
      break;
    }
    case RACE_FAILURE:
    {
		  unit->addReport( new   ReportRecord(new BinaryPattern(raceErrorReporter, unit->getRace(),skill)));
 		  return INVALID;
      break;
    }
    case REQUIREMENT_FAILURE:
    {
		  unit->addReport(new   ReportRecord(new BinaryPattern(requirementErrorReporter, unit, skill)) );
 		  return FAILURE;
      break;
    }
    case FOLLOWER_CANNOT_STUDY_LEVEL_FAILURE:
    case MAX_LEVEL_FAILURE:
    {
		  unit->addReport(new   ReportRecord(new BinaryPattern(maxLevelErrorReporter, unit, skill)) );
 		  return INVALID;
      break;
    }
    case FOLLOWER_CANNOT_STUDY_SECOND_BASIC_SKILL_FAILURE:
    {
		  unit->addReport(new   ReportRecord(new UnaryPattern(followerSkillLimitReporter, unit)) );
 		  return INVALID;
      break;
    }
    default:
    {
      cout << "Unexpected result ("<< result<<") of "<< skill->printName() <<" learning for " << unit->printTag() <<endl;
 		  return FAILURE;
    }
  }
    // level limit, specified in the order reached
      if(unit->getSkillLevel(skill) >= level )
        {
//         cout << " Order Level is " << level_ << " exp is " << skill_->getLevelExperience()<<" unit's level is "<< unit_->getSkillLevel(skill_)<<endl;
	       BinaryPattern * Message = new BinaryPattern(maxLevelErrorReporter, unit, skill);
	       ReportRecord * currentReport = new   ReportRecord(Message, orderId);
		     unit->addReport( currentReport);
 		     return INVALID;
        }

   // money check
    int cost = skill->getStudyCost(unit) * unit->getFiguresNumber();
    if (!unit->pay(cost) )
     {
	    BinaryPattern * Message = new BinaryPattern(paymentErrorReporter, unit, skill);
	    ReportRecord * currentReport = new   ReportRecord(Message, orderId);
		  unit->addReport( currentReport);
 		  return FAILURE;
    }
 		  return SUSPENDED;

}


/** Checks teaching condition and processes STUDY if possible*/
ORDER_STATUS StudyOrder::doProcess_(UnitEntity * unit, SkillRule * skill, int level, TeachingOffer * teacher, Order * orderId)
{
    // Check teacher
    bool needsTeacher = skill->teacherRequired(unit);
    if( needsTeacher && (teacher == 0))
    {
		  unit->addReport(new   ReportRecord(new BinaryPattern(teachingErrorReporter, unit, skill)) );
 		  return FAILURE;
    }

   // if this order is not the order that was processed last day we may refrain from reporting
   if(unit->getLastOrder() != orderId)
     {
     unit->addReport(new  BinaryPattern(learningStartedReporter,unit,
                        new SkillLevelElementData(new SkillLevelElement
                        (skill,unit->getSkillLevel(skill) +1))));
    }

  int newExp = skill->calculateLearningExperience(unit, teacher); // Learn-specific
  skill->addLearningExperience(unit,newExp); // Recursive, Learn-specific


   int newLevel = unit->getSkillLevel(skill);

   // Is it level specified in the order?
       if ( newLevel >= level )
 		      return SUCCESS;

 		  return IN_PROGRESS;


}
