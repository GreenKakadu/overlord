/***************************************************************************
                          BuildUsingStrategy.cpp 
                             -------------------
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "BuildUsingStrategy.h"
GameData * BuildUsingStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<BuildUsingStrategy> (this);
}
STATUS
BuildUsingStrategy::initialize        ( Parser *parser )
{
  if (parser->matchKeyword ("PRODUCES") )
    {
//      resourceHarvested_ = items[parser->getWord()];
//      days_ =  parser->getInteger();
      return OK;
    }
      return OK;
}



bool BuildUsingStrategy::use(UnitEntity * unit, Order * OrderId)
{
//  Rational request = getDailyProduction() * unit->getFiguresNumber();
//  unit->getLocation()->addDailyConflictRequest(new ResourceCompetitiveRequest(unit,getHarvestedResource(), request));
  return true;
}

