/***************************************************************************
                          NormalLearningStrategy.cpp 
                             -------------------
    begin                : Tue Feb 18 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "NormalLearningStrategy.h"
NormalLearningStrategy    sampleNormalLearning    ("LEARNING_NORMAL",    &sampleLearning);

NormalLearningStrategy::NormalLearningStrategy( const NormalLearningStrategy * prototype ):
           BasicLearningStrategy(prototype)
{
  //name_ = "a normal skill";
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


BasicLearningStrategy * NormalLearningStrategy::cloneSelf()
{
 NormalLearningStrategy * copyOfSelf = new NormalLearningStrategy(keyword_,parent_);
 *copyOfSelf = *this;
 return copyOfSelf;
}
