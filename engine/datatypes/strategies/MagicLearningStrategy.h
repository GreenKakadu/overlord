/***************************************************************************
                          MagicLearningStrategy.h 
                             -------------------
    begin                : Tue Feb 18 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef MAGIC_LEARNING_H
#define MAGIC_LEARNING_H

#include "BasicLearningStrategy.h"

/**
  *@author Alex Dribin
  */

class MagicLearningStrategy : public BasicLearningStrategy  {
public: 
	MagicLearningStrategy( const string & keyword, GameData * parent):
                                         BasicLearningStrategy(keyword, parent){}
  MagicLearningStrategy( const MagicLearningStrategy * prototype ): BasicLearningStrategy(prototype){}                                       
	~MagicLearningStrategy(){}
  GameData * createInstanceOfSelf();
  STATUS initialize        ( Parser *parser );
};

#endif
