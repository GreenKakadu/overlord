/***************************************************************************
                          Global.h  -  Contains declarations of all global variables.
                             -------------------
    begin                : Wed Nov 27 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef GLOBAL_H
#define GLOBAL_H
// PrototypeManager  definition
#include "GameData.h"
#include "PrototypeManager.h"
// Rules
#include "RulesCollection.h"
#include "BasicCondition.h"
#include "ObservationCondition.h"
#include "SkillCondition.h"
#include "TitleRule.h"
#include "SkillRule.h"
#include "ItemRule.h"
#include "RaceRule.h"
#include "LeaderRaceRule.h"
#include "FollowerRaceRule.h"
#include "CreatureRaceRule.h"
#include "EquipmentSlotVariety.h"
#include "BasicLearningStrategy.h"
#include "MagicLearningStrategy.h"
#include "NormalLearningStrategy.h"
#include "CreatureLearningStrategy.h"
#include "BasicUsingStrategy.h"
#include "HarvestUsingStrategy.h"
#include "CraftUsingStrategy.h"
#include "CombatUsingStrategy.h"
#include "SummonUsingStrategy.h"
#include "BuildUsingStrategy.h"
#include "MarketStrategy.h"

#include "TerrainRule.h"
#include "RuleIndex.h"
// Entities
#include "EntitiesCollection.h"
#include "UnitEntity.h"
#include "PhysicalEntity.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
// #include "GameEntity.h"
#include "GameInfo.h"
// Varieties
#include "VarietiesCollection.h"
#include "MovementVariety.h"
#include "DirectionVariety.h"
#include "StanceVariety.h"
// Order processing

#include "OrderPrototype.h"
#include "OrderPrototypesCollection.h"
#include "ProcessingMode.h"
#include "TravelElement.h"

#include "IntegerData.h" 
#include "ItemElementData.h"
#include "RaceElementData.h"
#include "Rational.h"
#include "SkillLevelElementData.h"   


extern GameInfo game;
extern int currentDay;
extern bool testMode;
//extern const unsigned SCALE_FACTOR;
//extern const int BLOCK_SIZE;

// Samples of objects
extern GameData       sampleGameData;
extern BasicCondition  sampleBasicCondition;
extern ObservationCondition  sampleObservationCondition;
extern SkillCondition  sampleSkillCondition;
extern TerrainRule    sampleTerrain;
extern TitleRule      sampleTitle;
extern RaceRule       sampleRace;
extern ItemRule       sampleItem;
extern SkillRule      sampleSkill;
extern EquipmentSlotVariety      sampleEquipmentSlot;
extern Entity         sampleEntity;
extern PhysicalEntity         samplePhysicalEntity;
extern UnitEntity     sampleUnit;
extern FactionEntity  sampleFaction;
extern MovementVariety  sampleMode;
extern StanceVariety     sampleStance;
// Sample Strategys
extern BasicLearningStrategy    sampleLearning;
extern MagicLearningStrategy    sampleMagicLearning;
extern NormalLearningStrategy   sampleNormalLearning;
extern CreatureLearningStrategy sampleCreatureLearning;
extern BasicUsingStrategy         sampleUsing;
extern HarvestUsingStrategy     sampleHarvestUsing;
extern CraftUsingStrategy       sampleCraftUsing;
extern SummonUsingStrategy      sampleSummonUsing;
extern CombatUsingStrategy      sampleCombatUsing;
extern BuildUsingStrategy       sampleBuildUsing;
// Rule Collections
extern RulesCollection <TerrainRule>   terrains;
extern RulesCollection <TitleRule>     titles;
extern RulesCollection <ItemRule>      items;
extern RulesCollection <SkillRule>     skills;
extern RulesCollection <RaceRule>      races;
// Entities Collections
extern EntitiesCollection <UnitEntity>      units;
extern EntitiesCollection <FactionEntity>   factions;
extern EntitiesCollection <LocationEntity>  locations;
// Varieties Collections
extern VarietiesCollection <MovementVariety> movementModes;
extern VarietiesCollection <DirectionVariety>     directions;
extern VarietiesCollection <StanceVariety>    stances;
extern VarietiesCollection <EquipmentSlotVariety>      equipments;
// Strategies
//extern RulesCollection <LearningRule>      learning;
// Order processing
extern OrderPrototypesCollection orderPrototypesCollection;
extern ProcessingMode   immediateOrders;
extern ProcessingMode   stackOrders;
extern ProcessingMode 	dayOrders;
// Functions
extern bool ciStringCompare(const string& s1,const string& s2);
extern bool matchItemElement(Parser * parser);
#endif
