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
#include "UnaryPattern.h"
#include "EntitiesCollection.h"
#include "RulesCollection.h"
#include "IntegerData.h"
extern RulesCollection <SkillRule>      skills;
//extern EntitiesCollection <UnitEntity>      units;
//extern EntitiesCollection <LocationEntity>      locations;
extern Reporter * unusableSkillReporter;
const UINT UseOrder::NO_RESOURCE_REPORT_FLAG = 0x01;



UseOrder::UseOrder(){

  keyword_ = "use";
  description = string("USE skill-tag [products] \n") +
  "Full-day.  This order executes as soon as the unit has the designated skill\n" +
  "at 1st level or better, and the requisite for the use of the skill are\n" +
  "fulfilled, notably for a production skill.  The order completes when the\n" +
  "specified number of products are obtained, for a production skill.  For a\n" +
  "spell casting, products specifies how many castings are done.\n";

  orderType_   = DAY_LONG_ORDER;
}

STATUS UseOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, skills, "skill tag", parameters))
            return IO_ERROR;

    parseIntegerParameter(parser, parameters);

  return OK;
}

ORDER_STATUS UseOrder::process (Entity * entity, vector <AbstractData *>  &parameters, Order * orderId)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);

  SkillRule * skill = dynamic_cast<SkillRule *>(parameters[0]); 
 if ( skill == 0)
    {
       unit->addReport( new UnaryPattern(unusableSkillReporter, parameters[0]));
 		  return INVALID;
    }
   // check that entity has enough resources
 USING_RESULT result = skill->mayUse(unit);

 switch (result)
  {
    case USING_OK:
    {
      break;
    }
    case UNUSABLE:
    {
       unit->addReport( new UnaryPattern(unusableSkillReporter, parameters[0]));
 		  return INVALID;
      break;
    }
    case NO_RESOURCES:
    {
     if(!orderId->getReportingFlag(NO_RESOURCE_REPORT_FLAG ))
      {
        skill->reportUse(result, unit,orderId);
        orderId->setReportingFlag(NO_RESOURCE_REPORT_FLAG );
      }
      return FAILURE;
      break;
    }
  }
  if(skill->use(unit, orderId))
  {
    int newExp = skill->calculateUsingExperience(unit); // Use-specific
    skill->addUsingExperience(unit,newExp); // Recursive, Use-specific
    return IN_PROGRESS;
  }
   else
    return FAILURE;  
}
ORDER_STATUS
UseOrder::completeProcessing (Entity * entity, vector <AbstractData *>  &parameters, Order * orderId, int result)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);

  if(parameters.size() > 1)
  {
    IntegerData * par1  = dynamic_cast<IntegerData *>(parameters[1]);
    assert(par1);
    int amount = par1->getValue();

    if ( amount > result)
    {
      par1->setValue(amount - result);
      entity->updateOrderResults(FAILURE,orderId);
//  cout << "Saving order for "<< unit->printName() <<"=[ ";
//  orderId->save(cout);
      return FAILURE;
    }
  }
  entity->updateOrderResults(SUCCESS,orderId);
//  cout << "Order completed for "<< unit->printName() <<".\n";
  return SUCCESS;
}

