/***************************************************************************
                          TeachOrder.cpp
                             -------------------
    begin                : Thu Jul 17 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "TeachOrder.h"
#include "Entity.h"
#include "IntegerData.h"
#include "UnitEntity.h"
#include "SkillLevelElement.h"
#include "LocationEntity.h"
#include "FactionEntity.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
#include "TokenEntity.h"
#include "TeachingOffer.h"
extern ReportPattern * teachingReporter;
extern EntitiesCollection <UnitEntity>      units;

//TeachOrder instantiateTeachOrder;
TeachOrder * instantiateTeachOrder = new TeachOrder();

TeachOrder::TeachOrder()
{
   keyword_ = "teach";
  registerOrder_();
  description = string("TEACH skill-tag unit-id [unit-id...] \n") +
  "Full-day, leader-only.  This order executes as soon as the specified unit is\n" +
  "present and issues the full-day order STUDY skill-tag.  Spends some time\n" +
  "teaching another unit.  Teaching is successful only when the teacher has a\n" +
  "greater level in the particular skill than the target unit.  Teaching enables\n" +
  "a unit to study a particular skill, get to an higher level and learn faster.\n";

    fullDayOrder_= true;
    orderType_   = DAY_LONG_ORDER;
}



STATUS TeachOrder::loadParameters(Parser * parser,
                            ParameterList &parameters, Entity * entity )
{
    if(!parseGameDataParameter(entity,  parser, skills, "skill tag", parameters))
            return IO_ERROR;

    while (parseOptionalGameDataParameter(entity, parser, units, parameters))
          {}

  return OK;


}



ORDER_STATUS TeachOrder::process (Entity * entity, ParameterList &parameters)
{
  TokenEntity * teacher = dynamic_cast<TokenEntity *>(entity);
  assert(teacher);

  SkillRule * skill = dynamic_cast<SkillRule *>(parameters[0]);

   if ( skill == 0)
    {
//		  unit->addReport( new UnaryMessage(cannotStudyReporter, unit->getRace()));
 		return INVALID;
    }
//     if(teacher==units["U1200"])
//     {
//       cout<<"TeachOrder";
//     }
  vector <Entity * > students;
 PROCESSING_STATE  state = entity->getCurrentOrder()->getProcessingState();

  switch(state)
   {
     case NORMAL_STATE:
     {
        for (unsigned i=1; i< parameters.size(); ++i)
        {
          UnitEntity * newStudent = dynamic_cast< UnitEntity *>(parameters[i]);
          if(newStudent)
            {
              if (!teacher->mayInterract(newStudent)) // Not In the same place or can't see
	            continue;
              students.push_back(newStudent);
            }
        }
        TeachingOffer * offer = new TeachingOffer(teacher, skill/*, students*/);
        teacher->addTeachingDonorOffer(offer);
        vector <Entity *>::iterator iter;
	if(students.size() == 0)
	{
		teacher->getLocation()->addTeachingAcceptorOffer(offer);
	}
	else
	{
		for(iter = students.begin(); iter != students.end(); ++iter)
		{
			(*iter)->addTeachingAcceptorOffer(offer);
		}
	}

            if(teacher->isTraced())
            {
                cout<<teacher->print()<<" offers teaching of "<<skill->print()<< " to "<<students.size()<<" students." <<endl;
            }
            entity->getCurrentOrder()->setProcessingState (SUSPEND);
            teacher->getLocation()->setTeacherCounter(true);
            return SUSPENDED;
        break;
     }
     case SUSPEND:
     if(teacher->checkTeachingConfirmation())
     {
        // if this order is not the order that was processed last day
        // we may refrain from reporting
      if(teacher->getLastOrder() != entity->getCurrentOrder())
        {
//QQQ
          teacher->addReport(new UnaryMessage (teachingReporter,
                        new  SkillLevelElement
                        (skill,teacher->getSkillLevel(skill))));
//          teacher->addReport(new BinaryMessage (teachingReporter2, skill,
//                        new IntegerData(teacher->getSkillLevel(skill))));
        }
      entity->getCurrentOrder()->setProcessingState (RESUME);
            return SUSPENDED;
     }
     else
     {
      teacher->getLocation()->setTeacherCounter(false);
      entity->getCurrentOrder()->setProcessingState (NORMAL_STATE);
      teacher->cancelTeachingOffer();
// 		  return INVALID;
 		  return FAILURE;
     }

        break;

     case RESUME:
      teacher->cancelTeachingOffer();
      teacher->getLocation()->setTeacherCounter(false);
      entity->getCurrentOrder()->setProcessingState (NORMAL_STATE);
      return SUCCESS;
        break;

     default:
      cout << "Unknown state "<< state <<" in TEACH order\n";
   }
 		  return INVALID;


}



void TeachOrder::preProcess_(Entity * entity, ParameterList &parameters)
{

}



void TeachOrder::doProcess_(Entity * entity, ParameterList &parameters)
{

}
