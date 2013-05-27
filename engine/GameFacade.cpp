/* 
 * File:   GameFacade.cpp
 * Author: alex
 * 
 * Created on February 22, 2011, 12:44 PM
 */

#include "GameFacade.h"
#include <iostream>
#include "DataStorageHandler.h"
#include "datatypes/collections/BasicCollection.h"
#include "datatypes/collections/BasicEntitiesCollection.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "ConstructionEntity.h"
#include "EffectEntity.h"
GameFacade::GameFacade()
{

}

GameFacade::GameFacade(const GameFacade& orig)
{
}

GameFacade::~GameFacade()
{
}

bool GameFacade::loadConfiguration(string gameGonfigFile)
{
  bool result =  gameConfig_.init(gameGonfigFile);
  if(!result)
  {
      cout<<"Can't open config file "<<gameGonfigFile<<endl;
      return result;
  }
  cout << "Game " <<        ( gameConfig_.getGameId() ) << endl;
  cout << "Game data taken from " <<      *  ( gameConfig_.getGameFile() ) << endl;
  cout << "Units data taken from " <<     * ( gameConfig_.getUnitsFile() ) << endl;
  cout << "Factions data taken from " <<  * ( gameConfig_.getFactionsFile() ) << endl;
  cout << "Locations data taken from " << * ( gameConfig_.getLocationsFile() ) << endl;

    factions.bindHandler(new DataStorageHandler(gameConfig_.getFactionsFile()));
    units.bindHandler(new DataStorageHandler(gameConfig_.getUnitsFile()));
    locations.bindHandler(new DataStorageHandler(gameConfig_.getLocationsFile()));
    effects.bindHandler(new DataStorageHandler(gameConfig_.getEffectsFile()));
    buildingsAndShips.bindHandler(new DataStorageHandler(gameConfig_.getBuildingsFile()));
    factions.setEntityTagPrefix('f');
// Rules
     fx_actions.bindHandler(new DataStorageHandler(gameConfig_.getFx_actionsFile()));
     terrains.bindHandler(new DataStorageHandler(gameConfig_.getTerrainsFile()));
     titles.bindHandler(new DataStorageHandler(gameConfig_.getTitlesFile()));
     skills.bindHandler(new DataStorageHandler(gameConfig_.getSkillsFile()));
     items.bindHandler(new DataStorageHandler(gameConfig_.getItemsFile()));
     races.bindHandler(new DataStorageHandler(gameConfig_.getRacesFile()));
     eventRules.bindHandler(new DataStorageHandler(gameConfig_.getEventsRuleFile()));
     effectRules.bindHandler(new DataStorageHandler(gameConfig_.getEffectsRuleFile()));
     enchantments.bindHandler(new DataStorageHandler(gameConfig_.getEnchantmentsFile()));
     constructions.bindHandler(new DataStorageHandler(gameConfig_.getConstructionsFile()));
     weathers.bindHandler(new DataStorageHandler(gameConfig_.getWeathersFile()));
     seasons.bindHandler(new DataStorageHandler(gameConfig_.getSeasonsFile()));
// Varieties
     movementModes.bindHandler(new DataStorageHandler(gameConfig_.getMovementsFile()));
     directions.bindHandler(new DataStorageHandler(gameConfig_.getDirectionsFile()));
     stances.bindHandler(new DataStorageHandler(gameConfig_.getStancesFile()));
     combatStances.bindHandler(new DataStorageHandler(gameConfig_.getCombat_stancesFile()));
     combatMoves.bindHandler(new DataStorageHandler(gameConfig_.getCombat_movesFile()));
     combatRanks.bindHandler(new DataStorageHandler(gameConfig_.getCombat_ranksFile()));
     combatFiles.bindHandler(new DataStorageHandler(gameConfig_.getCombat_filesFile()));
     equipments.bindHandler(new DataStorageHandler(gameConfig_.getEquipmentsFile()));
     construction_works.bindHandler(new DataStorageHandler(gameConfig_.getConstruction_worksFile()));
     combatTargets.bindHandler(new DataStorageHandler(gameConfig_.getCombat_targetFile()));
     gameEntity_ = new Entity(sampleEntity);
     // Events
     events.bindHandler(new DataStorageHandler(gameConfig_.getEventsFile()));
 getDataManipulator()->init();
  return result;
}
int GameFacade::getGameTurn()
{
    return gameConfig_.turn;
}

bool GameFacade::prepareGame()
{
    getDataManipulator()->turnPreProcessing();
  unknownUnit = units["U0"];
  unknownConstruction = buildingsAndShips["B0"];
  unknownLocation = locations["L0"];
  unknownEffect = effects["E0"];

    return true;
}


// Create a new copy of all Rules, Varieties and data files with extention .new
// 
bool GameFacade::saveData(char * suffix)
{
 if(getDataManipulator()->save(suffix) == OK)
 {
return true;
 }
 else
     return false;
}






bool GameFacade::isUnknownParameter(AbstractData * data)
{
  Entity * entity = dynamic_cast<Entity *> (data);
  if (entity == 0)
  {
      return false;
  }
  {// Block defined in order to reuse the same names for variables
  UnitEntity * entity = dynamic_cast<UnitEntity *> (data);
  if (entity != 0)
    {
     if(entity == gameFacade->unknownUnit)
       return true;
     else
       return false;
    }
  }
  {// Block defined in order to reuse the same names for variables
    ConstructionEntity * entity = dynamic_cast<ConstructionEntity *> (data);
    if (entity != 0)
      {
       if(entity == gameFacade->unknownConstruction)
         return true;
       else
         return false;
      }
  }
  {// Block defined in order to reuse the same names for variables
    LocationEntity * entity = dynamic_cast<LocationEntity *> (data);
    if (entity != 0)
      {
       if(entity == gameFacade->unknownLocation)
         return true;
       else
         return false;
      }
  }
  {// Block defined in order to reuse the same names for variables
    EffectEntity * entity = dynamic_cast<EffectEntity *> (data);
    if (entity != 0)
      {
       if(entity == gameFacade->unknownEffect)
         return true;
       else
         return false;
      }
  }
  return false;
}
