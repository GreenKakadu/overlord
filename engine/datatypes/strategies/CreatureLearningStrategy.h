/***************************************************************************
                          CreatureLearningStrategy.h 
                             -------------------
    begin                : Tue Feb 18 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef CREATURE_LEARNING_H
#define CREATURE_LEARNING_H

#include "BasicLearningStrategy.h"

/**
  *@author Alex Dribin
  */

class CreatureLearningStrategy : public BasicLearningStrategy  {
public: 
	CreatureLearningStrategy( const string & keyword, GameData * parent):
                                         BasicLearningStrategy(keyword, parent){}
  CreatureLearningStrategy( const CreatureLearningStrategy * prototype );
	~CreatureLearningStrategy(){}
  GameData * createInstanceOfSelf();
  STATUS initialize        ( Parser *parser );
};
extern CreatureLearningStrategy sampleCreatureLearning;

#endif
