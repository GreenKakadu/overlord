/* 
 * File:   GameFacade.h
 * Author: alex
 * This object provides facade to engine objects for use in client
 * Created on February 22, 2011, 12:44 PM
 */

#ifndef GAMEFACADE_H
#define	GAMEFACADE_H
#include <string>
#include "GameConfig.h"
#include "RuleIndex.h"
#include "DataManipulator.h"
#include "EntitiesCollection.h"
#include "RulesCollection.h"
#include "VarietiesCollection.h"
#include "BasicEventsCollection.h"
using namespace std;
class FactionEntity;
class LocationEntity;
class UnitEntity;
class EffectEntity;
class ConstructionEntity;
class ActionRule;
class TerrainRule;
class TitleRule;
class SkillRule;
class ItemRule;
class RaceRule;
class EffectRule;
class EnchantmentRule;
class EventRule;
class ConstructionRule;
class WeatherRule;
class SeasonRule;
class MovementVariety;
class DirectionVariety;
class StanceVariety;
class CombatStanceVariety;
class CombatMoveVariety;
class CombatRankVariety;
class CombatFileVariety;
class EquipmentSlotVariety;
class ConstructionWorksVariety;
class CombatTargetVariety;

class GameFacade {
public:
    GameFacade();
    GameFacade(const GameFacade& orig);
    virtual ~GameFacade();
    bool loadConfiguration(string gameGonfigFile);
    bool prepareGame();
    int getGameTurn();
    inline int getCurrentDay(){return currentDay_;}
    inline void setCurrentDay(int day){currentDay_ = day;}
    inline DataManipulator * getDataManipulator(){return &dataManipulator_;}
    inline GameConfig * getGameConfig(){return &gameConfig_;}
    inline Entity * getGameEntity(){return gameEntity_;}
   static bool isUnknownParameter(AbstractData * data);
// Entities
    EntitiesCollection <FactionEntity>   factions;
    EntitiesCollection <UnitEntity>   units;
    EntitiesCollection <LocationEntity>   locations;
    EntitiesCollection <EffectEntity>   effects;
    EntitiesCollection <ConstructionEntity>   buildingsAndShips;
// Rules
   RulesCollection <ActionRule>    fx_actions;
   RulesCollection <TerrainRule>    terrains;
   RulesCollection <TitleRule>    titles;
   RulesCollection <SkillRule>    skills;
   RulesCollection <ItemRule>    items;
   RulesCollection <RaceRule>    races;
   RulesCollection <EffectRule>    effectRules;
   RulesCollection <EnchantmentRule>    enchantments;
   RulesCollection <EventRule>    eventRules;
   RulesCollection <ConstructionRule>     constructions;
   RulesCollection <WeatherRule>    weathers;
   RulesCollection <SeasonRule>     seasons;
// Varieties
   VarietiesCollection <MovementVariety>   movementModes;
   VarietiesCollection <DirectionVariety>   directions;
   VarietiesCollection <StanceVariety>   stances;
   VarietiesCollection <CombatStanceVariety>   combatStances;
   VarietiesCollection <CombatMoveVariety>   combatMoves;
   VarietiesCollection <CombatRankVariety>   combatRanks;
   VarietiesCollection <CombatFileVariety>   combatFiles;
   VarietiesCollection <EquipmentSlotVariety>   equipments;
   VarietiesCollection <ConstructionWorksVariety>   construction_works;
   VarietiesCollection <CombatTargetVariety>   combatTargets;
// Events
   BasicEventsCollection    events;
// Important objects
   UnitEntity * unknownUnit;
   ConstructionEntity * unknownConstruction;
   LocationEntity * unknownLocation;
   EffectEntity * unknownEffect;
   RuleIndex ruleIndex;
private:
GameConfig  gameConfig_;
DataManipulator dataManipulator_;
int currentDay_;
Entity * gameEntity_;
};
extern GameFacade * gameFacade;
#endif	/* GAMEFACADE_H */

