/***************************************************************************
                          TerrainRule.cpp  
                             -------------------
    begin                : Mon Nov 12 13:34:00 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "TerrainRule.h"
#include "SkillRule.h"
#include "RulesCollection.h"
extern RulesCollection <SkillRule>      skills;

TerrainRule::TerrainRule ( const TerrainRule * prototype ) : Rule(prototype)
{
  optimalPopulation_ =0;
  landWalk_ =0;
  }


GameData * TerrainRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<TerrainRule> (this);
}


STATUS 
TerrainRule::initialize        ( Parser *parser )
{

	
  if (parser->matchKeyword ("NAME") )
    {
      setName(parser->getText());
      return OK;
    }
  if (parser->matchKeyword("DESCRIPTION"))
    {
      setDescription(parser->getText());
      return OK;
    }
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
  if (parser->matchKeyword ("DAYS") )
    {
      MovementVariety * movementMode = movementModes[parser->getWord()];
       if(movementMode)
            _movementTime[movementMode] = parser->getInteger();
      return OK;
    }
      return OK;

 }

void TerrainRule::print()
{
    cout  << getName();
    cout << " [" << getTag()  << "] ";
 if( optimalPopulation_)
   cout << "Optima "<< optimalPopulation_ <<endl;
 else
   cout << endl;
   
      
}



int TerrainRule::getTravelTime(MovementVariety * mode)
{

  return   _movementTime[mode];

}
//bool TerrainRule::mayMove(MovementVariety * mode, UnitEntity * unit)
//{return true;}
