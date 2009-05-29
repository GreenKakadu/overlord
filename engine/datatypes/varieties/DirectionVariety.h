/***************************************************************************
                          DirectionVariety.h  -  description
                             -------------------
    begin                : Sun Jan 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#ifndef DIR_H
#define DIR_H

#include "Variety.h"
#include "VarietiesCollection.h"

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

extern VarietiesCollection <DirectionVariety>     directions;
extern DirectionVariety sampleDirection;
#endif
