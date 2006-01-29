/***************************************************************************
                          NormalLearningStrategy.cpp 
                             -------------------
    begin                : Tue Feb 18 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "NormalLearningStrategy.h"
NormalLearningStrategy    sampleNormalLearning    ("LEARNING_NORMAL",    &sampleLearning);

NormalLearningStrategy::NormalLearningStrategy( const NormalLearningStrategy * prototype ):
           BasicLearningStrategy(prototype)
{
  name_ = "creature skill";
}



GameData * NormalLearningStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<NormalLearningStrategy> (this);
}



STATUS
NormalLearningStrategy::initialize        ( Parser *parser )
{
      return BasicLearningStrategy::initialize(parser);
}
