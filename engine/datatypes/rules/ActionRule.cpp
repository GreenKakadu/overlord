/***************************************************************************
                          ActionRule.cpp 
                             -------------------
    begin                : Sun Jan 18 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "ActionRule.h"
#include "PrototypeManager.h"
#include "GameConfig.h"

//RulesCollection <ActionRule>    fx_actions(new DataStorageHandler(gameConfig.getFx_actionsFile()));
RulesCollection <ActionRule>    fx_actions(new DataStorageHandler("fx_actions.rules"));
ActionRule    sampleAction    ("FX_ACTION",&sampleGameData);

ActionRule::ActionRule ( const ActionRule * prototype ) : Rule(prototype)
{
  targetType_ =  0;
}


GameData * ActionRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<ActionRule> (this);
}


STATUS
ActionRule::initialize        ( Parser *parser )
{
  GameData::initialize(parser);

  targetType_ =  prototypeManager->findInRegistry("UNIT");


  if (parser->matchKeyword("TARGET"))
    {
      string targetName = parser->getText();
      targetType_ =  prototypeManager->findInRegistry(targetName);
			if( targetType_== 0)
				{
					cerr << "Unknown target type " <<  targetName << " for action " << print()<< endl;
				}

      return OK;
    }
  return OK;
}



void ActionRule::printDescription(ReportPrinter & out)
{
    out << print()<< ": "<< getDescription()<<".";
}



ACTION_RESULT ActionRule::carryOut(Entity * entity)
{
  return     ACTION_FAILURE;

}
