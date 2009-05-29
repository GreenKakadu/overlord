/***************************************************************************
                             ForgetOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "ForgetOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
#include "SkillRule.h"
extern RulesCollection <SkillRule>      skills;
extern ReportPattern *	AtReporter;

ForgetOrder * instantiateForgetOrder = new ForgetOrder();

ForgetOrder::ForgetOrder(){
  keyword_ = "Forget";
  registerOrder_();
  description = string("FORGET skill-tag \n") +
  "Immediate, one-shot.  Executes if the unit has any experience in the specified\n" +
  "skill.  Causes the unit to forget the given skill, and all skills that depend\n" +
  "on it.  This order is useful for normal units who wish to learn a new skill,\n" +
  "but already knows a different skill.\n";

  orderType_   = IMMEDIATE_ORDER;
}

STATUS ForgetOrder::loadParameters(Parser * parser,
                            ParameterList &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, skills, "skill tag", parameters))
            return IO_ERROR;
  return OK;

}



ORDER_STATUS ForgetOrder::process (Entity * entity, ParameterList &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
  SkillRule * skill = dynamic_cast<SkillRule *>(parameters[0]);

   if ( skill == 0)
    {
 		return FAILURE;
    }

  if(!unit->hasSkill(skill,1))
	    return FAILURE;
	unit->forgetSkill(skill);
  return SUCCESS;
}

