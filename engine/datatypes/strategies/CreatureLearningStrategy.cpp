/***************************************************************************
                          CreatureLearningStrategy.cpp 
                             -------------------
    begin                : Tue Feb 18 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "CreatureLearningStrategy.h"
CreatureLearningStrategy  sampleCreatureLearning  ("LEARNING_CREATURE",  &sampleLearning);
CreatureLearningStrategy::CreatureLearningStrategy( const CreatureLearningStrategy * prototype ):
           BasicLearningStrategy(prototype)
{
  name_ = "creature skill";
}



GameData * CreatureLearningStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<CreatureLearningStrategy> (this);
}



STATUS
CreatureLearningStrategy::initialize        ( Parser *parser )
{
      return BasicLearningStrategy::initialize(parser);
}

void CreatureLearningStrategy::save(ostream &out)
{
    BasicLearningStrategy::save(out);
}


BasicLearningStrategy * CreatureLearningStrategy::cloneSelf()
{
 CreatureLearningStrategy * copyOfSelf = new CreatureLearningStrategy(keyword_,parent_);
 *copyOfSelf = *this;
 return copyOfSelf;
}
