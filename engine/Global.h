/***************************************************************************
                          Global.h  -  Contains declarations of all global variables.
                             -------------------
    begin                : Wed Nov 27 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
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
#include "ActionRule.h"
#include "EnchantmentRule.h"
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
#include "EnchantmentUsingStrategy.h"
#include "SummonUsingStrategy.h"
#include "BuildUsingStrategy.h"
#include "ConstructionUsingStrategy.h"
#include "MarketStrategy.h"
#include "ConstructionRule.h"
#include "ActionUsingStrategy.h"
//#include "EffectUsingStrategy.h"

#include "TerrainRule.h"
#include "RuleIndex.h"
// Entities
#include "EntitiesCollection.h"
#include "UnitEntity.h"
#include "TokenEntity.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
#include "ConstructionEntity.h"
#include "EffectEntity.h"
//#include "GameEntity.h"
//#include "GameConfig.h"

// Varieties
#include "VarietiesCollection.h"
#include "MovementVariety.h"
#include "DirectionVariety.h"
#include "StanceVariety.h"
#include "CombatStanceVariety.h"
#include "CombatMoveVariety.h"
#include "CombatRankVariety.h"
#include "CombatFileVariety.h"
#include "ConstructionWorksVariety.h"

// Order processing

#include "OrderPrototype.h"
#include "OrderPrototypesCollection.h"
#include "ProcessingMode.h"
#include "TravelElement.h"

#include "IntegerData.h"
#include "ItemElement.h"
#include "RationalNumber.h"
#include "BasicExit.h"


extern bool testMode;
// Functions
extern bool ciStringCompare(const string& s1,const string& s2);
extern bool matchItemElement(Parser * parser);
extern bool isFileExist(string filename);
extern string makeRandomName(int length=6);
extern void notifyAndExit(string filename);

#endif
