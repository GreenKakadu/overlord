/***************************************************************************
                             SkillOrder.cpp 
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "SkillOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "SkillRule.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "TertiaryPattern.h"
#include "EntitiesCollection.h"
extern RulesCollection <SkillRule>      skills;
extern Reporter *	invalidParameterReporter;
extern Reporter *	missingParameterReporter;

SkillOrder * instantiateSkillOrder = new SkillOrder();

SkillOrder::SkillOrder(){
  keyword_ = "Skill";
  registerOrder_();
  description = string("SKILL skill-id number [ LEVEL | DAYS ] \n") +
  "Immediate, condition.  This order executes when the unit has either the\n" +
  "specified level, or at least the number of days of experience in the skill.\n" +
  "If neither LEVEL nor DAYS is specified, LEVEL is assumed.\n";
  orderType_   = IMMEDIATE_ORDER;
  mayInterrupt_ = true;
}

STATUS SkillOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, skills, "skill tag", parameters))
            return IO_ERROR;
    if(!parseIntegerParameter(parser, parameters))
    {
      entity->addReport(new TertiaryPattern(invalidParameterReporter, new StringData(keyword_), new StringData(""), new StringData("skill level or days number")));
      return IO_ERROR;
    }
  if(parser ->matchKeyword("DAYS"))
	{
      parameters.push_back( new StringData ("DAYS"));
	}
	else
      parameters.push_back( new StringData ("LEVEL"));

    
  return OK;

}



ORDER_STATUS SkillOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
  SkillRule * skill = dynamic_cast<SkillRule *>(parameters[0]);

   if ( skill == 0)
    {
 		return FAILURE;
    }
  int parameter = getIntegerParameter(parameters,1);
  
  if (parameters.size() > 2)
    {
      string par = parameters[2]->print();
      if(par == string("DAYS"))
        {
        	if(unit->hasSkill(skill,parameter))
        		return SUCCESS;
        	else
        		return FAILURE;	
          }
    }     

    if(unit->hasSkillLevel(skill,parameter))
        	return SUCCESS;
    else
        	return FAILURE;	

  
	return FAILURE;
}

