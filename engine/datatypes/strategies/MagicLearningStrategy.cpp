/***************************************************************************
                          MagicLearningStrategy.cpp 
                             -------------------
    begin                : Tue Feb 18 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#include "MagicLearningStrategy.h"
GameData * MagicLearningStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<MagicLearningStrategy> (this);
}
STATUS
MagicLearningStrategy::initialize        ( Parser *parser )
{
  if (parser->matchKeyword ("SPECIALIST") )
    {
      return OK;
    }
      return OK;
}

