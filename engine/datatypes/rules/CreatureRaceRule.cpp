/***************************************************************************
                          CreatureRaceRule.cpp 
                             -------------------
    begin                : Thu Apr 17 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "CreatureRaceRule.h"
CreatureRaceRule::CreatureRaceRule ( const CreatureRaceRule * prototype ) : RaceRule(prototype)
{
  hiringCost_ = 500;
  hiringProbability_ = 100;
  hiringMax_ = 100;
}

GameData * CreatureRaceRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<CreatureRaceRule> (this);
}

STATUS
CreatureRaceRule::initialize        ( Parser *parser )
{
  if (parser->matchKeyword ("SPECIALIST") )
    {
//			special_ = true;
      return OK;
    }
      return RaceRule::initialize(parser);
}

bool CreatureRaceRule::mayWork()
{
  return false;
}
