/***************************************************************************
                          Variety.h 
                             -------------------
    begin                : Sun Mar 16 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef VARIETY_H
#define VARIETY_H

#include "GameData.h"
#include "OverlordTypes.h"


/**
  *@author Alex Dribin
  */

class Variety : public GameData  {
public: 
      Variety ( const Variety * prototype ): GameData(prototype){}
      Variety ( const string & keyword, GameData * parent): GameData(keyword, parent){}
	virtual ~Variety(){}
protected:
};

#endif
