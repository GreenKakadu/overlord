/***************************************************************************
                          TerrainRule.cpp  
                             -------------------
    begin                : Mon Nov 12 13:34:00 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "TerrainRule.h"
#include "SkillRule.h"

//TerrainRule   sampleTerrain   ("TERRAIN",  &sampleGameData);
BasicRulesCollection  terrains(new DataStorageHandler("terrains"));
extern RulesCollection <SkillRule>      skills;



TerrainRule * findTerrainByTag(const string &tag)
{
 return GET_FROM_COLLECTION<TerrainRule>(&terrains,tag);
}

TerrainRule::TerrainRule ( const TerrainRule * prototype ) : Rule(prototype)
{
  optimalPopulation_ =0;
  landWalk_ =0;
  buildEnabled_ = true;
  }


GameData * TerrainRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<TerrainRule> (this);
}


STATUS 
TerrainRule::initialize        ( Parser *parser )
{
  GameData::initialize(parser);
	
  if (parser->matchKeyword ("OPTIMA") )
    {
      optimalPopulation_ = parser->getInteger();
      return OK;
    }
  if (parser->matchKeyword ("LANDWALK") )
    {
      landWalk_ = skills[parser->getWord()];
      return OK;
    }
  if (parser->matchKeyword ("NOBUILD") )
    {
      buildEnabled_ = false;
      return OK;
    }
  if (parser->matchKeyword ("DAYS") )
    {
      MovementVariety * movementMode = movementModes[parser->getWord()];
       if(movementMode)
            _movementTime[movementMode] = parser->getInteger();
      return OK;
    }
      return OK;

 }




int TerrainRule::getTravelTime(MovementVariety * mode)
{

  return   _movementTime[mode];

}
//bool TerrainRule::mayMove(MovementVariety * mode, UnitEntity * unit)
//{return true;}



void TerrainRule::printDescription(ReportPrinter & out)
{
    out << print()<< ": "<< getDescription()<<".";    
    if(! buildEnabled_) out << " Construction of buildings  is not allowed here.";
}
