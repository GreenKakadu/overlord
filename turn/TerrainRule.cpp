/***************************************************************************
                          TerrainRule.cpp  
                             -------------------
    begin                : Mon Nov 12 13:34:00 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#include "TerrainRule.h"

TerrainRule::TerrainRule()
{
}

TerrainRule::TerrainRule(const string & keyword, GameData * parent)
{
    keyword_   = keyword;               
    parent_   = parent;               
    prototypeManager->addToRegistry(this);
}

TerrainRule::TerrainRule(const TerrainRule * prototype)
{
      parent_ = prototype->getParent();
      keyword_ = prototype->getKeyword();
}

GameData * TerrainRule::createInstanceOfSelf()
{
  return CREATE_INSTANCE<TerrainRule> (this);
}


int 
TerrainRule::initialize        ( Parser *parser )
{

	
  if (parser->matchKeyword ("NAME") )
    {
      setName(parser->getText());
      return 0;
    }
  if (parser->matchKeyword("DESCRIPTION"))
    {
      setDescription(parser->getText());
      return 0;
    }
  if (parser->matchKeyword ("OPTIMA") )
    {
      _optimalPopulation = parser->getInteger();
      return 0;
    }
      return 0;

 }

void TerrainRule::print()
{
    cout  << getName();
    cout << " [" << getTag()  << "] ";
 if( _optimalPopulation)
   cout << "Optima "<< _optimalPopulation <<endl;
 else
   cout << endl;
   
      
}
