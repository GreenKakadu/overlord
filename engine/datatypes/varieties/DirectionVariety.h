/***************************************************************************
                          DirectionVariety.h  -  description
                             -------------------
    begin                : Sun Jan 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef DIR_H
#define DIR_H

#include "Variety.h"

class DirectionVariety : public Variety  {
public: 
      DirectionVariety ( const string & keyword, GameData * parent): Variety(keyword, parent){}
      DirectionVariety ( const DirectionVariety * prototype ) : Variety(prototype){}
//			DirectionVariety(){};
		 ~DirectionVariety(){};

      virtual STATUS     initialize      ( Parser *parser);
      GameData * createInstanceOfSelf();
    protected:
    private:
};

#endif
