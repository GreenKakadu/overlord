/***************************************************************************
                          CreatureLearningStrategy.cpp 
                             -------------------
    begin                : Tue Feb 18 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "CreatureLearningStrategy.h"
GameData * CreatureLearningStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<CreatureLearningStrategy> (this);
}
STATUS
CreatureLearningStrategy::initialize        ( Parser *parser )
{
  if (parser->matchKeyword ("SPECIALIST") )
    {
      return OK;
    }
      return OK;
}
