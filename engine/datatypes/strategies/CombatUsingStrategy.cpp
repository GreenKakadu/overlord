/***************************************************************************
                          CombatUsingStrategy.cpp 
                             -------------------
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "CombatUsingStrategy.h"
#include "TokenEntity.h"
CombatUsingStrategy       sampleCombatUsing       ("USING_COMBAT",       &sampleUsing);
GameData * CombatUsingStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<CombatUsingStrategy> (this);
}



STATUS
CombatUsingStrategy::initialize        ( Parser *parser )
{
  if (parser->matchKeyword ("PRODUCES") )
    {
      return OK;
    }
      return OK;
}



USING_RESULT CombatUsingStrategy::use(TokenEntity * tokenEntity, SkillRule * skill, 
			int &useCounter,OrderLine * order)
{
  return UNUSABLE;
}




