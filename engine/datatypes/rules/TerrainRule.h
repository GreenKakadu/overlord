/***************************************************************************
                          TerrainRule.h  

                             -------------------
    begin                : Mon Nov 12 13:52:00 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef TERRAIN_H
#define TERRAIN_H
#include "Rule.h"
#include "MovementMode.h"
#include "RulesCollection.h"
#include "DataStorageHandler.h"
//class UnitEntity;
class SkillRule;

class  TerrainRule : public Rule
{
    public:
      TerrainRule ( const string & keyword, GameData * parent) : Rule(keyword, parent){}
      TerrainRule ( const TerrainRule * prototype );// : Rule(prototype)	{_optimalPopulation =0;landWalk_ =0;}
      ~TerrainRule (){}
      virtual STATUS     initialize      ( Parser *parser);
      GameData * createInstanceOfSelf();
      void printDescription(ReportPrinter & out);
      virtual  int getTravelTime(MovementVariety * mode);
//    virtual  bool mayMove(MovementVariety * mode, UnitEntity * unit); // For special terrains
      SkillRule * getLandWalk() const {return landWalk_;}
      inline bool mayBuild() const {return buildEnabled_;}
    protected:
      int optimalPopulation_;
      MovementMode<int> _movementTime;
      SkillRule * landWalk_;
      bool buildEnabled_;
    private:
};

extern TerrainRule    sampleTerrain;
extern BasicRulesCollection   terrains;
#endif
