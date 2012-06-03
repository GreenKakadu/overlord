/***************************************************************************
                          NormalLearningStrategy.h 
                             -------------------
    begin                : Tue Feb 18 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#ifndef NORMAL_LEARNING_H
#define NORMAL_LEARNING_H

#include "BasicLearningStrategy.h"

/**
  *@author Alex Dribin
  */

class NormalLearningStrategy : public BasicLearningStrategy  {
public: 
	NormalLearningStrategy( const string & keyword, GameData * parent):
                                         BasicLearningStrategy(keyword, parent){}
  NormalLearningStrategy( const NormalLearningStrategy * prototype );
	~NormalLearningStrategy(){};
  GameData * createInstanceOfSelf();
  STATUS initialize        ( Parser *parser );
	BasicLearningStrategy * cloneSelf();
      virtual void save(ostream &out);
};
extern NormalLearningStrategy   sampleNormalLearning;

#endif
