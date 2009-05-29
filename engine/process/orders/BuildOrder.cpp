/***************************************************************************
                          BuildOrder.cpp
                             -------------------
    begin                : Sat Dec 2 2006
    copyright            : (C) 2006 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "BuildOrder.h"
#include "Entity.h"
#include "SkillRule.h"
#include "UnitEntity.h"
#include "ConstructionRule.h"
#include "ConstructionEntity.h"
#include "UnaryMessage.h"
#include "EntitiesCollection.h"
#include "RulesCollection.h"
#include "IntegerData.h"
#include "TargetOrder.h"
#include "StringData.h"
#include "BinaryMessage.h"
#include "UnaryMessage.h"
#include "LocationEntity.h"

extern RulesCollection <SkillRule>      skills;
extern ReportPattern * unusableSkillReporter;
extern ReportPattern * unusableBuildingSkillReporter;
const UINT BuildOrder::NO_RESOURCE_REPORT_FLAG = 0x01;

BuildOrder * instantiateBuildOrder = new BuildOrder();
extern ReportPattern *	missingParameterReporter;

BuildOrder::BuildOrder()
{

	keyword_ = "build";
	registerOrder_();
        description = string ( "BUILD [construction-tag] [structure-tag or placeholder name] [skill] [use counter]\n" ) +
	              "Full-day.  This order executes if the unit has skill required for building\n" +
	              "of designated construction, and the requisite for building that construction.\n";
// How BUILD should work:
// 
// Starting new building:
// 
//     BUILD construction [new structure-tag]
//     example: BUILD mine f06nB01
//     (this will require some skills, for instance building mine or farm will require 1-st building)
// 
// Continue to work on building, that already exists:
// 
//     BUILD [structure-tag] [skill] [N]
//     structure-tag - tag of structure to build. If empty will work on target.
//     skill - skill to use if unit has more than one skill applicable to this construction. If empty, first one will be selected
//     N - use counter. (Use no more than N times)
//        example BUILD f06nB01 carp 5
        
        
	fullDayOrder_= true;
	orderType_   = DAY_LONG_ORDER;
}



STATUS BuildOrder::loadParameters ( Parser * parser,
                                    ParameterList &parameters, Entity * entity )
{
	if ( !entityIsUnit ( entity ) )
		return IO_ERROR;
        string tag1 = parser->getWord();
        string tag2;
        if (tag1.size() == 0)
        {
          entity->addReport(new BinaryMessage(missingParameterReporter, new StringData(keyword_),
                            new StringData("constructoin type  or construction id")));
          return IO_ERROR;
        }
        if(checkRuleParameterTag( entity,  tag1, constructions, parameters ))
        {
          tag2 = parser->getWord();
          if (tag2.size() != 0)
          {
            checkParameterTag( entity,  tag2, buildingsAndShips, parameters );
          }
          return OK;
        }
        
        else if(checkParameterTag( entity,  tag1, buildingsAndShips, parameters ))
        {
          tag2 = parser->getWord();
          if (tag2.size() != 0)
          {
            checkRuleParameterTag(entity,  tag2, skills, parameters);
          }
          return OK;
        }
        else return IO_ERROR;
	//parseGameDataParameter ( entity,  parser, constructions, "construction tag", parameters );

        //parseGameDataParameter ( entity,  parser, buildingsAndShips, "Building or ship tag", parameters );

        //parseGameDataParameter(entity,  parser, skills, "skill tag", parameters);
        //parseOptionalGameDataParameter ( entity,  parser, constructions, parameters );

        //parseOptionalGameDataParameter ( entity,  parser, buildingsAndShips,  parameters );

        //parseOptionalGameDataParameter(entity,  parser, skills,  parameters);

        //parseIntegerParameter(parser, parameters);

	return OK;
}



ORDER_STATUS BuildOrder::process ( Entity * entity, ParameterList &parameters )
{
  int useCounter = 0;
  bool isFirstUse = false; 
  bool shouldAllreadyExist = false;
  ConstructionEntity * buildingOrShip =0; 
  ConstructionRule * construction;
  NewEntityPlaceholder * placeholder =0;
  SkillRule * skill;
	unsigned int parameterOffset = 0;
	IntegerData *  par1 = 0;

	UnitEntity * unit = dynamic_cast<UnitEntity *> ( entity );
	assert ( unit );

        construction = dynamic_cast<ConstructionRule *> ( parameters[0] );
        if ( construction != 0 )//Start building
        {
          //look for placeholder.
          if(parameters.size() >1 )
          {
            placeholder = dynamic_cast<NewEntityPlaceholder *>(parameters[1]);
            if (placeholder != 0)
            {
              parameterOffset = 1;
              Entity * realEntity = placeholder->getRealEntity();
              if(realEntity != 0)
              {
                buildingOrShip = dynamic_cast<ConstructionEntity *>(realEntity);
              }              
            }
            
          }                

          return startNewConstruction(construction,unit,buildingOrShip,unit->getCurrentOrder(),placeholder);
        }
        else// Continue building
        {
          //look for placeholder.
          buildingOrShip   =  dynamic_cast<ConstructionEntity *>(parameters[0]); 
          if(!buildingOrShip)
          {
            NewEntityPlaceholder * placeholder = dynamic_cast<NewEntityPlaceholder *>(parameters[0]);
        //    cout <<"placeholder=  "<< (int) placeholder <<endl;
              if(placeholder != 0)  // this is  placeholder.
              {
                GameData* realEntity = placeholder->getRealEntity();
                if(realEntity) // We can get real entity id from placeholder
                {
                  buildingOrShip   =  dynamic_cast<ConstructionEntity *>(realEntity); 
                }

              }
          }
          if(buildingOrShip)
          {
            parameterOffset = 1;
          }
          else
          {
            buildingOrShip = dynamic_cast<ConstructionEntity *>(unit->getTarget());
            if(buildingOrShip==0)//wrong target
            {
              return FAILURE;
            }
          }
            skill = dynamic_cast<SkillRule *> ( parameters[0 + parameterOffset] );
            if(skill) 
            {
              return buildExistingConstruction(buildingOrShip,unit,skill,unit->getCurrentOrder());
            }
               
        }

}

extern ReportPattern * newBuidingStartedReporter;
extern ReportPattern * buidingFinishedReporter;
extern ReportPattern * constructionStartedReporter;

ORDER_STATUS
BuildOrder::completeOrderProcessing ( Entity * entity, ParameterList &parameters, int result )
{
	UnitEntity * unit = dynamic_cast<UnitEntity *> ( entity );
	assert ( unit );
	unsigned int parameterOffset = 0;

	if ( parameters.size() >1 )
	{
		StringData * target       =  dynamic_cast<StringData *> ( parameters[1] );
		if ( target )
		{
			parameterOffset = 1;
		}
	}

	if ( parameters.size() > 1 + parameterOffset )
	{
		IntegerData * par1  = dynamic_cast<IntegerData *> ( parameters[1] );
		assert ( par1 );
		int amount = par1->getValue();

		if ( amount > result )
		{
			par1->setValue ( amount - result );
			entity->updateOrderResults ( FAILURE );
//  cout << "Saving order for "<< unit->print() <<"=[ ";
//  orderId->save(cout);
			return FAILURE;
		}
	}
	entity->updateOrderResults ( SUCCESS );
//  cout << "Order completed for "<< unit->print() <<".\n";
	return SUCCESS;
}
// Takes no time
ORDER_STATUS BuildOrder::startNewConstruction( ConstructionRule * construction, UnitEntity * unit,ConstructionEntity *buildingOrShip, OrderLine * order, AbstractData * placeholder)
{

  SkillRule * skill = construction->getBuildingSkill()->getSkill();
  USING_RESULT result =  construction->startNewConstruction(unit,buildingOrShip,placeholder);
  
  switch ( result )
  {
    case USING_IN_PROGRESS:
    {
      int newExp = skill->calculateUsingExperience ( unit ); // Use-specific
      skill->addUsingExperience ( unit,newExp ); // Recursive, Use-specific
      return IN_PROGRESS;
      break;
    }
    case USING_COMPLETED:
    {
      order->setCompletionFlag(true);
      int newExp = skill->calculateUsingExperience ( unit ); // Use-specific
      skill->addUsingExperience ( unit,newExp ); // Recursive, Use-specific
      return SUCCESS;
      break;
    }
    case UNUSABLE:
    {
      return FAILURE;
      break;
    }
    case CANNOT_USE:
    {
      return FAILURE;
      break;
    }
    case USING_OK:
    case NO_RESOURCES:
    case NO_MANA:
    case CONDITION_FAILURE:
    case WRONG_TARGET:
    case NO_TARGET:
    case TARGET_NOT_EXIST:
    case USING_NOT_ALLOWED:
    default:
      cout << "ILLEGAL USING_RESULT (" << result<<") for use of "<<skill->print() <<"\n";
      return FAILURE;
  }

}

ORDER_STATUS BuildOrder::buildExistingConstruction( ConstructionEntity * construction, UnitEntity * unit, SkillRule * skill, OrderLine * order)
{
      USING_RESULT result =  skill->useBuild(unit,construction);
      switch ( result )
      {
        case USING_IN_PROGRESS:
        {
          int newExp = skill->calculateUsingExperience ( unit ); // Use-specific
          skill->addUsingExperience ( unit,newExp ); // Recursive, Use-specific
          return IN_PROGRESS;
          break;
        }
        case USING_COMPLETED:
        {
          order->setCompletionFlag(true);
          int newExp = skill->calculateUsingExperience ( unit ); // Use-specific
          skill->addUsingExperience ( unit,newExp ); // Recursive, Use-specific
          return SUCCESS;
          break;
        }
        case UNUSABLE:
        {
          return FAILURE;
          break;
        }
        case CANNOT_USE:
        {
          return FAILURE;
          break;
        }
        case USING_OK:
        case NO_RESOURCES:
        case NO_MANA:
        case CONDITION_FAILURE:
        case WRONG_TARGET:
        case NO_TARGET:
        case TARGET_NOT_EXIST:
        case USING_NOT_ALLOWED:
        default:
          cout << "ILLEGAL USING_RESULT (" << result<<") for use of "<<skill->print() <<"\n";
          return FAILURE;
      }

}