/***************************************************************************
                          Global.h  -  Contains declarations of all global variables.
                             -------------------
    begin                : Wed Nov 27 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef GLOBAL_H
#define GLOBAL_H
//#ifdef HAVE_CONFIG_H
#include "config.h"
//#endif
// PrototypeManager  definition
#include "general/GameData.h"
#include "general/PrototypeManager.h"
// Rules
#include "collections/RulesCollection.h"
#include "rules/TitleRule.h"
#include "rules/SkillRule.h"
#include "rules/ItemRule.h"
#include "rules/RaceRule.h"
#include "rules/LeaderRaceRule.h"
#include "rules/FollowerRaceRule.h"
#include "rules/CreatureRaceRule.h"
#include "varieties/EquipmentSlotVariety.h"
#include "letters/BasicLearningLetter.h"
#include "letters/MagicLearningLetter.h"
#include "letters/NormalLearningLetter.h"
#include "letters/CreatureLearningLetter.h"
#include "letters/BasicUseLetter.h"
#include "letters/HarvestUsingLetter.h"
#include "letters/CraftUsingLetter.h"
#include "letters/CombatUsingLetter.h"
#include "letters/SummonUsingLetter.h"
#include "letters/BuildUsingLetter.h"
#include "letters/BasicStudentLetter.h"
#include "letters/LeaderStudentLetter.h"
#include "letters/FollowerStudentLetter.h"
#include "letters/CreatureStudentLetter.h"

#include "rules/TerrainRule.h"
// Entities
#include "collections/EntitiesCollection.h"
#include "entities/UnitEntity.h"
#include "entities/FactionEntity.h"
#include "entities/LocationEntity.h"
// #include "GameEntity.h"
#include "general/GameInfo.h"
// Varieties
#include "collections/VarietiesCollection.h"
#include "varieties/MovementVariety.h"
#include "varieties/DirectionVariety.h"
#include "varieties/StanceVariety.h"
// Order processing

#include "general/OrderPrototype.h"
#include "collections/OrderPrototypesCollection.h"
#include "general/ProcessingMode.h"
#include "elements/TravelElement.h"
extern GameInfo game;
extern int currentDay;
extern bool testMode;
//extern const int BLOCK_SIZE;

// Samples of objects
extern GameData       sampleGameData;
extern TerrainRule    sampleTerrain;
extern TitleRule      sampleTitle;
extern RaceRule       sampleRace;
extern ItemRule       sampleItem;
extern SkillRule      sampleSkill;
extern EquipmentSlotVariety      sampleEquipmentSlot;
extern Entity         sampleEntity;
extern UnitEntity     sampleUnit;
extern FactionEntity  sampleFaction;
extern MovementVariety  sampleMode;
extern StanceVariety     sampleStance;
// Sample Letters
extern BasicLearningLetter    sampleLearning;
extern MagicLearningLetter    sampleMagicLearning;
extern NormalLearningLetter   sampleNormalLearning;
extern CreatureLearningLetter sampleCreatureLearning;
extern BasicUseLetter         sampleUsing;
extern HarvestUsingLetter     sampleHarvestUsing;
extern CraftUsingLetter       sampleCraftUsing;
extern SummonUsingLetter      sampleSummonUsing;
extern CombatUsingLetter      sampleCombatUsing;
extern BuildUsingLetter       sampleBuildUsing;
extern BasicStudentLetter     sampleStudent;
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
// Letters
//extern RulesCollection <LearningRule>      learning;
// Order processing
extern OrderPrototypesCollection orderPrototypesCollection;
extern ProcessingMode   immediateOrders;
extern ProcessingMode   stackOrders;
extern ProcessingMode 	dayOrders;
// Functions
//extern bool mayInterract(Entity * entity1, Entity * entity2);
extern bool ciStringCompare(const string& s1,const string& s2);
extern bool matchItemElement(Parser * parser);
#endif
