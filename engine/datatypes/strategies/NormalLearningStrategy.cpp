/***************************************************************************
                          NormalLearningStrategy.cpp 
                             -------------------
    begin                : Tue Feb 18 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "NormalLearningStrategy.h"
GameData * NormalLearningStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<NormalLearningStrategy> (this);
}
STATUS
NormalLearningStrategy::initialize        ( Parser *parser )
{
  if (parser->matchKeyword ("SPECIALIST") )
    {
      return OK;
    }
      return OK;
}
