/***************************************************************************
                          Strategy.h 
                             -------------------
    begin                : Sun Mar 16 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef LETTER_H
#define LETTER_H

#include "GameData.h"

#include "OverlordTypes.h"
class Parser;

/**
  *@author Alex Dribin
  */

class Strategy : public GameData  {
public: 
	Strategy(const string & keyword, GameData * parent): GameData(keyword, parent){}
	Strategy(const GameData * prototype): GameData(prototype){}
	~Strategy(){}
 virtual STATUS     initialize      ( Parser *);
  GameData * createInstanceOfSelf();
};
extern Strategy sampleStrategy;
#endif
