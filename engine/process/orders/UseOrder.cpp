/***************************************************************************
                          UseOrder.cpp
                             -------------------
    begin                : Mon May 5 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "UseOrder.h"
#include "Entity.h"
#include "SkillRule.h"
#include "UnitEntity.h"
#include "UnaryMessage.h"
#include "EntitiesCollection.h"
#include "RulesCollection.h"
#include "IntegerData.h"
#include "TargetOrder.h"
#include "StringData.h"

extern RulesCollection <SkillRule>      skills;
//extern EntitiesCollection <UnitEntity>      units;
//extern EntitiesCollection <LocationEntity>      locations;
extern ReportPattern * unusableSkillReporter;
const UINT UseOrder::NO_RESOURCE_REPORT_FLAG = 0x01;

UseOrder * instantiateUseOrder = new UseOrder();

UseOrder::UseOrder(){

  keyword_ = "use";
  registerOrder_();
  description = string("USE skill-tag [target] [products] \n") +
  "Full-day.  This order executes as soon as the unit has the designated skill\n" +
  "at 1st level or better, and the requisite for the use of the skill are\n" +
  "fulfilled, notably for a production skill.  The order completes when the\n" +
  "specified number of products are obtained, for a production skill.  For a\n" +
  "spell casting, products specifies how many castings are done.\n";

    fullDayOrder_= true;
  orderType_   = DAY_LONG_ORDER;
}



STATUS UseOrder::loadParameters(Parser * parser,
                            ParameterList &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, skills, "skill tag", parameters))
            return IO_ERROR;

   if( ! parser->matchInteger()) // non-integer parameter is target
   {
    string tag = parser->getWord();
    if (tag.size() != 0)
        {
          parameters.push_back(TargetOrder::findTarget(tag));
        }
    }
    parseIntegerParameter(parser, parameters);

  return OK;
}



ORDER_STATUS UseOrder::process (Entity * entity, ParameterList &parameters)
{
  int useCounter = 0;
  unsigned int parameterOffset = 0;
  IntegerData *  par1 = 0;

  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);

  SkillRule * skill = dynamic_cast<SkillRule *>(parameters[0]);
 if ( skill == 0)
    {
       unit->addReport( new UnaryMessage(unusableSkillReporter, parameters[0]));
 		  return INVALID;
    }

 if(parameters.size() >1)
    {
      par1       =  dynamic_cast<IntegerData *>(parameters[1]);
      if(!par1)
      {
        parameterOffset = 1;
        unit->setTarget(parameters[1]);
      }
      else
        useCounter = par1->getValue();
    }

  if(parameters.size() >2)
    {
      par1       =  dynamic_cast<IntegerData *>(parameters[ 1+ parameterOffset]);
      assert(par1);
      useCounter = par1->getValue();
//      if (useCounter == 0 )
//         useCounter = 1;
    }
   // check that entity may use skill (has enough resources ETC.)
 USING_RESULT result = skill->mayBeUsedBy(unit);

 switch (result)
  {
    case USING_OK:
    {
      break;
    }
    case UNUSABLE:
    {
       unit->addReport( new UnaryMessage(unusableSkillReporter, parameters[0]));
 		  return INVALID;
      break;
    }
    case NO_RESOURCES:
    {
     if(!unit->getCurrentOrder()->getReportingFlag(NO_RESOURCE_REPORT_FLAG ))
      {
        skill->reportUse(result, unit);
        unit->getCurrentOrder()->setReportingFlag(NO_RESOURCE_REPORT_FLAG );
      }
      return FAILURE;
      break;
    }
    case CONDITION_FAILURE:
    {
//       unit->addReport( new UnaryMessage(unusableSkillReporter, parameters[0]));
 		  return FAILURE;
      break;
    }
    case CANNOT_USE:
    {
//       unit->addReport( new UnaryMessage(unusableSkillReporter, parameters[0]));
 		  return INVALID;
      break;
    }
    case WRONG_TARGET:
    {
//       unit->addReport( new UnaryMessage(unusableSkillReporter, parameters[0]));
 		  return INVALID;
      break;
    }
    case NO_TARGET:
    {
//       unit->addReport( new UnaryMessage(unusableSkillReporter, parameters[0]));
 		  return INVALID;
      break;
    }
    case USING_NOT_ALLOWED:
    {
//      unit->addReport(new BinaryMessage (harvestingNotPermittedReporter, //resourceType_, unit->getLocation()),0,0);
 		  return FAILURE;
      break;
    }
    case TARGET_NOT_EXIST:
    {
//       unit->addReport( new UnaryMessage(unusableSkillReporter, parameters[0]));
 		  return FAILURE;
      break;
    }
    default:
      cout << "ILLEGAL USING_RESULT (" << result<<") for evaluation of use of "<<skill->print()<<"\n";
      return FAILURE;
  }
  if(parameterOffset)
  {
    (parameters[1])->clean();
     parameters[1] = unit->getTarget();
  }
  result = skill->use(unit,useCounter);

  if(parameters.size() > 1+ parameterOffset)
    {
      par1->setValue(useCounter);
    }

 switch (result)
  {
    case USING_IN_PROGRESS:
      {
      int newExp = skill->calculateUsingExperience(unit); // Use-specific
      skill->addUsingExperience(unit,newExp); // Recursive, Use-specific
      return IN_PROGRESS;
      break;
      }
    case USING_COMPLETED:
      {
      int newExp = skill->calculateUsingExperience(unit); // Use-specific
      skill->addUsingExperience(unit,newExp); // Recursive, Use-specific
      return SUCCESS;
      break;
      }
    case UNUSABLE:
    case CANNOT_USE:
    case USING_OK:
    case NO_RESOURCES:
    case CONDITION_FAILURE:
    case WRONG_TARGET:
    case NO_TARGET:
    case TARGET_NOT_EXIST:
    case USING_NOT_ALLOWED:
    default:
      cout << "ILLEGAL USING_RESULT (" << result<<") for use of "<<skill->print()<<"\n";
      return FAILURE;
  }
}



ORDER_STATUS
UseOrder::completeOrderProcessing (Entity * entity, ParameterList &parameters, int result)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
  unsigned int parameterOffset = 0;

 if(parameters.size() >1)
    {
     StringData * target       =  dynamic_cast<StringData *>(parameters[1]);
     if(target)
      {
          parameterOffset = 1;
      }
     }

  if(parameters.size() > 1 + parameterOffset)
  {
    IntegerData * par1  = dynamic_cast<IntegerData *>(parameters[1]);
    assert(par1);
    int amount = par1->getValue();

    if ( amount > result)
    {
      par1->setValue(amount - result);
      entity->updateOrderResults(FAILURE);
//  cout << "Saving order for "<< unit->print() <<"=[ ";
//  orderId->save(cout);
      return FAILURE;
    }
  }
  entity->updateOrderResults(SUCCESS);
//  cout << "Order completed for "<< unit->print() <<".\n";
  return SUCCESS;
}

