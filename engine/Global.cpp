/***************************************************************************
                          Global.cpp  -  Global variables
                             -------------------
    begin                : Wed Nov 27 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 This file contains Overlord Engine definitions   
 ***************************************************************************/
#include "Global.h"
#include "DataStorageHandler.h"


int currentDay = 1;
const int BLOCK_SIZE = 1024;
//const unsigned SCALE_FACTOR = 100;

template class EntitiesCollection<UnitEntity>;
template class RulesCollection<TerrainRule>;
template class VarietiesCollection<MovementVariety>;
//PrototypeManager * GameData::prototypeManager= new PrototypeManager;

PrototypeManager * prototypeManager = 0; // will be created on first call
BasicCondition  sampleBasicCondition ("CONDITION",      &sampleGameData);
ObservationCondition  sampleObservationCondition ("OBSERVATION_CONDITION", &sampleGameData);
SkillCondition  sampleSkillCondition ("SKILL_CONDITION", &sampleGameData);
GameData  targetTypeSelf("SELF", &sampleGameData);
GameInfo game;
// Varieties
Variety          sampleVariety  ("VARIETY",  &sampleGameData);
MovementVariety  sampleMode  ("MOVEMENT",  &sampleVariety);
DirectionVariety sampleDirection ("DIR",      &sampleGameData);
StanceVariety    sampleStance    ("STANCE",   &sampleGameData);
EquipmentSlotVariety sampleEquipmentSlot     ("EQUIPMENT_SLOT", &sampleGameData);
ConstructionWorksVariety   sampleConstructionWork  ("CONSTRUCTION_WORK", &sampleGameData);

TerrainRule   sampleTerrain   ("TERRAIN",  &sampleGameData);
TitleRule     sampleTitle     ("TITLE",    &sampleGameData);
ItemRule      sampleItem      ("ITEM",     &sampleGameData);
SkillRule     sampleSkill     ("SKILL",    &sampleGameData);
RaceRule      sampleRace      ("RACE",     &sampleGameData);
ActionRule    sampleAction    ("FX_ACTION",&sampleGameData);
EnchantmentRule    sampleEnchantment    ("FX_EFFECT",&sampleGameData);
ConstructionRule   sampleConstructionRule =   ConstructionRule("CONSTRUCTION", &sampleGameData);// Derived Rules

// Derived Rules
MerchantPrinceTitleRule     sampleMerchantPrinceTitleRule =     MerchantPrinceTitleRule("PRINCE", &sampleTitle);
OverlordTitleRule  sampleOverlordTitleRule =  OverlordTitleRule ("OVERLORD", &sampleTitle);
LeaderRaceRule     sampleLeaderRaceRule =     LeaderRaceRule("LEADER", &sampleRace);
FollowerRaceRule   sampleFollowerRaceRule =   FollowerRaceRule("FOLLOWER", &sampleRace);
CreatureRaceRule   sampleCreatureRaceRule =   CreatureRaceRule("CREATURE", &sampleRace);// Entities

// Entities
Entity         sampleEntity  ("ENTITY",  &sampleGameData);
TokenEntity sampleTokenEntity  ("PHYSICAL_ENTITY",  &sampleEntity);
UnitEntity     sampleUnit    ("UNIT",    &sampleTokenEntity);
FactionEntity  sampleFaction ("FACTION", &sampleEntity);
LocationEntity sampleLocation("LOCATION",&sampleTokenEntity);
EffectEntity         sampleEffectEntity  ("EFFECT",  &sampleTokenEntity);
ConstructionEntity   sampleConstructionEntity =   ConstructionEntity("BUILDING", &sampleTokenEntity);
//Entity * RIPplaceholder = new  Entity(sampleEntity);
// Strategys
BasicLearningStrategy     sampleLearning          ("LEARNING",           &sampleGameData);
MagicLearningStrategy     sampleMagicLearning     ("LEARNING_MAGIC",     &sampleLearning);
NormalLearningStrategy    sampleNormalLearning    ("LEARNING_NORMAL",    &sampleLearning);
CreatureLearningStrategy  sampleCreatureLearning  ("LEARNING_CREATURE",  &sampleLearning);
BasicUsingStrategy        sampleUsing             ("USING",              &sampleGameData);
HarvestUsingStrategy      sampleHarvestUsing      ("USING_HARVEST",      &sampleUsing);
CraftUsingStrategy        sampleCraftUsing        ("USING_CRAFT",        &sampleUsing);
EnchantmentUsingStrategy  sampleEnchantmentUsing  ("USING_ENCHANT",      &sampleUsing);
SummonUsingStrategy       sampleSummonUsing       ("USING_SUMMON",       &sampleUsing);
ActionUsingStrategy       sampleActionUsing       ("USING_ACTION",       &sampleUsing);
//EffectUsingStrategy       sampleEffectUsing       ("USING_EFFECT",       &sampleUsing);
CombatUsingStrategy       sampleCombatUsing       ("USING_COMBAT",       &sampleUsing);
BuildUsingStrategy        sampleBuildUsing        ("USING_BUILD",        &sampleUsing);
ConstructionUsingStrategy sampleConstructionUsing ("USING_CONSTRUCTION", &sampleUsing);
MarketStrategy            sampleMarket            ("MARKET",             &sampleGameData);

// Varieties Collections
VarietiesCollection <MovementVariety> movementModes(new DataStorageHandler("movements"));
VarietiesCollection <DirectionVariety>     directions(new DataStorageHandler("directions"));
VarietiesCollection <StanceVariety>    stances(new DataStorageHandler("stances"));
VarietiesCollection <EquipmentSlotVariety>      equipments(new DataStorageHandler("equipments"));
VarietiesCollection <ConstructionWorksVariety>  construction_works(new DataStorageHandler("construction_works"));

// Rules Collections

//RulesCollection <TerrainRule>   terrains(new DataStorageHandler("terrains"));
RulesCollection <TitleRule>     titles(new DataStorageHandler("titles"));
RulesCollection <ItemRule>      items(new DataStorageHandler("items"));
RulesCollection <SkillRule>     skills(new DataStorageHandler("skills"));
RulesCollection <RaceRule>      races(new DataStorageHandler("races"));
RulesCollection <ActionRule>    fx_actions(new DataStorageHandler("fx_actions"));
RulesCollection <EnchantmentRule>    enchantments(new DataStorageHandler("enchantments"));
RulesCollection <ConstructionRule>      constructions(new DataStorageHandler("constructions"));
RuleIndex ruleIndex;

EntitiesCollection <UnitEntity>   units(new DataStorageHandler(game.getUnitsFile()));
EntitiesCollection <FactionEntity>   factions(new DataStorageHandler(game.getFactionsFile() ));
EntitiesCollection <LocationEntity>   locations(new DataStorageHandler(game.getLocationsFile() ));
EntitiesCollection <ConstructionEntity>   buildingsAndShips(new DataStorageHandler(game.getBuildingsFile() ));
EntitiesCollection <EffectEntity>   effects(new DataStorageHandler(game.getEffectsFile()));


bool testMode = false;
ProcessingMode   immediateOrders  (IMMEDIATE_ORDER);
ProcessingMode  stackOrders  (STACK_ORDER);
ProcessingMode 	 dayOrders  (DAY_LONG_ORDER);

// Samples: All these are here temporary to provide
// symbol loading from libraries.
#include "libraryWorkaround.h"
// special values
ItemRule * cash;
ItemRule * food;
MovementVariety * walkingMode;
MovementVariety * swimingMode;
MovementVariety * flyingMode;
StanceVariety *  allied;

// Case-insensitive string comparison ( Scott Meyers)
#include <cctype>
#include <string>
bool ciCharCompare(char c1, char c2)
{
	return tolower(static_cast<unsigned char>(c1))  != tolower(static_cast<unsigned char>(c2));
}


bool ciStringCompare(const std::string& s1,const std::string& s2)
{
  if(s1.size() != s2.size())
    return true;
  if (lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(),ciCharCompare))
  return true;
	else
	return false;
}
bool ciStringCompareN(const std::string& s1,const std::string& s2, const int N)
{
  int i;
  for (i = 0; i< N ; i++)
  {
    if (ciCharCompare(s1[i], s2[i]))
    return true;
  }  	
	return false ;
}
/* Converts a long int to a string.
   'out' must be long enough to take the largest number.
 */
void longtostr(unsigned long u, char *out)
{
        char *p=out,*r;
	/* Process the number, output to the string in reverse */
        r = p;
        do {
		/* Turn the digit into a character */
                *p = '0' + u%10;
                u=u/10;
                p++;
        } while(u != 0);

        *p=0;
        p--;
        /* Reverse The String. */
        while(r < p) {
                *p ^= *r ^= *p ^= *r; /* A Crazy Swap */
                p--; r++;
        }
}



string longtostr(unsigned long u)
{
        char buffer[12];
        char *p=buffer, *r;
	/* Process the number, output to the string in reverse */
        r = p;
        do {
		/* Turn the digit into a character */
                *p = '0' + u%10;
                u=u/10;
                p++;
        } while(u != 0);

        *p=0;
        p--;
        /* Reverse The String. */
        while(r < p) {
                *p ^= *r ^= *p ^= *r; /* A Crazy Swap */
                p--; r++;
        }
        return string(buffer);
}
#include "Element.h"
#include "ItemElement.h"
#include "RaceElement.h"
#include "ReportElement.h"
#include "ResourceElement.h"
#include "MovementElement.h"
#include "ConstructionWorksElement.h"
#include "EnchantmentElement.h"

// Template instantiation
#ifdef BCC
template  <> BasicSkillElement * BasicSkillElement::headOfFreeList;
template  <> BasicItemElement * BasicItemElement::headOfFreeList;
template  <> BasicRaceElement * BasicRaceElement::headOfFreeList;
template  <> BasicInventoryElement * BasicInventoryElement::headOfFreeList;
template  <> BasicReportElement  * BasicReportElement::headOfFreeList;
template  <> StanceElement * StanceElement::headOfFreeList;
template  <> BasicResourceElement * BasicResourceElement::headOfFreeList;
template  <> BasicMovementElement * BasicMovementElement::headOfFreeList;
template  <> BasicSkillUseElement * BasicSkillUseElement::headOfFreeList;
template  <> BasicConstructionWorksElement * BasicConstructionWorksElement::headOfFreeList;
template  <> BSwapRequestElement * SwapRequestElement::headOfFreeList;
#else
             BasicSkillElement * BasicSkillElement::headOfFreeList;
             BasicEnchantmentElement * BasicEnchantmentElement::headOfFreeList;
             BasicItemElement * BasicItemElement::headOfFreeList;
             Element2<RaceRule, int > * Element2<RaceRule, int >::headOfFreeList;
             BasicInventoryElement * BasicInventoryElement::headOfFreeList;
             BasicReportElement  * BasicReportElement::headOfFreeList;
             StanceElement * StanceElement::headOfFreeList;
             BasicResourceElement * BasicResourceElement::headOfFreeList;
             BasicMovementElement * BasicMovementElement::headOfFreeList;
             BasicSkillUseElement * BasicSkillUseElement::headOfFreeList;
             BasicConstructionWorksElement * BasicConstructionWorksElement::headOfFreeList;
             SwapRequestElement * SwapRequestElement::headOfFreeList;
#endif
// Temporary: Implementation Definitions
//PickpocketActionRule     samplePickpocketActionRule =     PickpocketActionRule("PICKPOCKET_ACTION", &sampleAction);
