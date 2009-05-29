/***************************************************************************
                          TerrainRule.cpp  
                             -------------------
    begin                : Mon Nov 12 13:34:00 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "TerrainRule.h"
#include "SkillRule.h"
#include "GameConfig.h"

TerrainRule   sampleTerrain   ("TERRAIN",  &sampleGameData);
//BasicRulesCollection  terrains(new DataStorageHandler(gameConfig.getTerrainsFile()));
BasicRulesCollection  terrains(new DataStorageHandler("terrains.rules"));
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
  	colorR_ = 255;
	colorG_= 255;
	colorB_ = 255;
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
  if (parser->matchKeyword("RESOURCE"))
	{
        ResourceElement * newResource = ResourceElement::readElement(parser);
        if(newResource)
          potentialResources.push_back(newResource);
	  return OK;
	}
  if (parser->matchKeyword ("NOBUILD") )
    {
      buildEnabled_ = false;
      return OK;
    }
  if (parser->matchKeyword ("COLOR_R") )
    {
	  colorR_ =  parser->getInteger();
      return OK;
    }
  if (parser->matchKeyword ("COLOR_G") )
    {
	  colorG_ =  parser->getInteger();
      return OK;
    }
  if (parser->matchKeyword ("COLOR_B") )
    {
	  colorB_ =  parser->getInteger();
      return OK;
    }
  if (parser->matchKeyword ("DAYS") )
    {
      MovementVariety * movementMode = movementModes[parser->getWord()];
       if(movementMode)
            _movementTime[movementMode] = parser->getInteger();
      return OK;
    }
		skillBonuses_.initialize(parser);
		movementBonuses_.initialize(parser);
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



int TerrainRule::getProductionBonusValue(SkillRule * skill)
{
  return skillBonuses_.getProductionBonus(skill);
}



int TerrainRule::getLearningBonus(SkillRule * skill)
{
  return skillBonuses_.getLearningBonus(skill);
}



int TerrainRule::getStudyBonus(SkillRule * skill)
{
  return skillBonuses_.getStudyBonus(skill);
}



// Implementation-specific routine for determining aquatic locations
bool TerrainRule::isAquatic()
{
   return(this == terrains.findByTag("lake",false) ||
	 			this == terrains.findByTag( "ocea",false));

}
