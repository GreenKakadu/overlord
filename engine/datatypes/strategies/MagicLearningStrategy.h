/***************************************************************************
                          MagicLearningStrategy.h 
                             -------------------
    begin                : Tue Feb 18 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
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
  MagicLearningStrategy( const MagicLearningStrategy * prototype );                                      
	~MagicLearningStrategy(){}
  LEARNING_RESULT mayStudy(TokenEntity * unit, SkillRule * skill);
  GameData * createInstanceOfSelf();
	BasicLearningStrategy * cloneSelf();
  STATUS initialize        ( Parser *parser );
      virtual void save(ostream &out);
};
extern MagicLearningStrategy    sampleMagicLearning;

#endif
