/***************************************************************************
                          TerrainRule.h  

                             -------------------
    begin                : Mon Nov 12 13:52:00 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef TERRAIN_H
#define TERRAIN_H
#include "GameData.h"



class  TerrainRule : public GameData
{
public:
          TerrainRule ( const string & keyword, GameData * parent);
          TerrainRule ( const TerrainRule * prototype );
          TerrainRule ();
  virtual int     initialize      ( Parser *parser);
  GameData * createInstanceOfSelf();
  void     print();


protected:
         int _optimalPopulation;
	 
private:
};
  
#endif
