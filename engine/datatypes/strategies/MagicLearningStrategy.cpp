/***************************************************************************
                          MagicLearningStrategy.cpp 
                             -------------------
    begin                : Tue Feb 18 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "MagicLearningStrategy.h"
MagicLearningStrategy     sampleMagicLearning     ("LEARNING_MAGIC",     &sampleLearning);

MagicLearningStrategy::MagicLearningStrategy( const MagicLearningStrategy * prototype ):
           BasicLearningStrategy(prototype)
{
  name_ = "magic skill";
}



GameData * MagicLearningStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<MagicLearningStrategy> (this);
}



STATUS
MagicLearningStrategy::initialize        ( Parser *parser )
{
      return BasicLearningStrategy::initialize(parser);
}

