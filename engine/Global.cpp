/***************************************************************************
                          Global.cpp  -  Global variables
                             -------------------
    begin                : Wed Nov 27 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "Global.h"
#include "DataStorageHandler.h"


int currentDay = 1;
const int BLOCK_SIZE = 1024;
//const unsigned SCALE_FACTOR = 100;

template class EntitiesCollection<UnitEntity>;
template class RulesCollection<TerrainRule>;
template class VarietiesCollection<MovementVariety>;
PrototypeManager * GameData::prototypeManager= new PrototypeManager;

GameInfo game;
GameData      sampleGameData  ("GameData",0);
BasicCondition  sampleBasicCondition ("CONDITION",      &sampleGameData);
ObservationCondition  sampleObservationCondition ("OBSERVATION_CONDITION", &sampleGameData);
SkillCondition  sampleSkillCondition ("SKILL_CONDITION", &sampleGameData);
// Varieties
Variety          sampleVariety  ("VARIETY",  &sampleGameData);
MovementVariety  sampleMode  ("MOVEMENT",  &sampleVariety);
DirectionVariety sampleDirection ("DIR",      &sampleGameData);
StanceVariety    sampleStance    ("STANCE",   &sampleGameData);
EquipmentSlotVariety sampleEquipmentSlot     ("EQUIPMENT_SLOT", &sampleGameData);

TerrainRule   sampleTerrain   ("TERRAIN",  &sampleGameData);
TitleRule     sampleTitle     ("TITLE",    &sampleGameData);
ItemRule      sampleItem      ("ITEM",     &sampleGameData);
SkillRule     sampleSkill     ("SKILL",    &sampleGameData);
RaceRule      sampleRace      ("RACE",     &sampleGameData);
// Derived Rules
MajorTitleRule     sampleMajorTitleRule =     MajorTitleRule("MAJOR", &sampleTitle);
MinorTitleRule     sampleMinorTitleRule =     MinorTitleRule("MINOR", &sampleTitle);
StandardTitleRule  sampleStandardTitleRule =  StandardTitleRule ("STANDARD", &sampleTitle );
OverlordTitleRule  sampleOverlordTitleRule =  OverlordTitleRule ("OVERLORD", &sampleTitle);
LeaderRaceRule     sampleLeaderRaceRule =     LeaderRaceRule("LEADER", &sampleRace);
FollowerRaceRule   sampleFollowerRaceRule =   FollowerRaceRule("FOLLOWER", &sampleRace);
CreatureRaceRule   sampleCreatureRaceRule =   CreatureRaceRule("CREATURE", &sampleRace);
//RaceRule   sampleRaceRule =   RaceRule("", &sampleRace);
// Entities
Entity         sampleEntity  ("ENTITY",  &sampleGameData);
PhysicalEntity samplePhysicalEntity  ("PHYSICAL_ENTITY",  &sampleEntity);
UnitEntity     sampleUnit    ("UNIT",    &samplePhysicalEntity);
FactionEntity  sampleFaction ("FACTION", &sampleEntity);
LocationEntity sampleLocation("LOCATION",&samplePhysicalEntity);
//Entity * RIPplaceholder = new  Entity(sampleEntity);
// Strategys
BasicLearningStrategy    sampleLearning         ("LEARNING",          &sampleGameData);
MagicLearningStrategy    sampleMagicLearning    ("LEARNING_MAGIC",    &sampleLearning);
NormalLearningStrategy   sampleNormalLearning   ("LEARNING_NORMAL",   &sampleLearning);
CreatureLearningStrategy sampleCreatureLearning ("LEARNING_CREATURE", &sampleLearning);
BasicUsingStrategy       sampleUsing            ("USING",             &sampleGameData);
HarvestUsingStrategy     sampleHarvestUsing     ("USING_HARVEST",     &sampleUsing);
CraftUsingStrategy       sampleCraftUsing       ("USING_CRAFT",       &sampleUsing);
SummonUsingStrategy      sampleSummonUsing      ("USING_SUMMON",      &sampleUsing);
CombatUsingStrategy      sampleCombatUsing      ("USING_COMBAT",      &sampleUsing);
BuildUsingStrategy       sampleBuildUsing       ("USING_BUILD",       &sampleUsing);
MarketStrategy           sampleMarket           ("MARKET",            &sampleGameData);

// Varieties Collections
VarietiesCollection <MovementVariety> movementModes(new DataStorageHandler("movements"));
VarietiesCollection <DirectionVariety>     directions(new DataStorageHandler("directions"));
VarietiesCollection <StanceVariety>    stances(new DataStorageHandler("stances"));
VarietiesCollection <EquipmentSlotVariety>      equipments(new DataStorageHandler("equipments"));

// Rules Collections

RulesCollection <TerrainRule>   terrains(new DataStorageHandler("terrains"));
RulesCollection <TitleRule>     titles(new DataStorageHandler("titles"));
RulesCollection <ItemRule>      items(new DataStorageHandler("items"));
RulesCollection <SkillRule>     skills(new DataStorageHandler("skills"));
RulesCollection <RaceRule>      races(new DataStorageHandler("races"));
RuleIndex ruleIndex;

EntitiesCollection <UnitEntity>   units(new DataStorageHandler(game.getUnitsFile()));
EntitiesCollection <FactionEntity>   factions(new DataStorageHandler(game.getFactionsFile() ));
EntitiesCollection <LocationEntity>   locations(new DataStorageHandler(game.getLocationsFile() ));


OrderPrototypesCollection orderPrototypesCollection;

bool testMode = false;
ProcessingMode   immediateOrders  (IMMEDIATE_ORDER);
ProcessingMode  stackOrders  (STACK_ORDER);
ProcessingMode 	 dayOrders  (DAY_LONG_ORDER);
// Samples: All these shouldn't be here but for some reason
// I can't call methods of some uninstantiated objects
#include "ResourceCompetitiveRequest.h"
#include "LocalMarketRequest.h"
#include "MarketConflict.h"
#include "RecruitRequest.h"
#include "NewRecruitRequest.h"
#include "LocalRecruitOffer.h"
TravelElement sample(0,0,0,0,0);
IntegerData  IntegerDataSample(0);
ItemElementData ItemElementDataSample(0,0);
RaceElementData RaceElementDataSample(0,0);
SkillLevelElementData SkillLevelElementDataSample(new SkillLevelElement(0,0));
Rational rationalSample(0);
ResourceCompetitiveRequest rcrSample(0, 0, 0, rationalSample);
LocalMarketRequest lmrSample(0,0,0,BUY);
RecruitRequest rrSample(0,0,0,0,0,0);
NewRecruitRequest nrrSample(0,0,0,0,0,0);
LocalRecruitOffer lrrSample(0,0,0);
MarketConflict marketConflictSample(0);
// special values
ItemRule * cash; 
MovementVariety * walkingMode;
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
#include "Element.h"
#include "ItemElement.h"
#include "RaceElement.h"
#include "ReportElement.h"
#include "ResourceElement.h"
#include "MovementElement.h"
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
#else
             BasicSkillElement * BasicSkillElement::headOfFreeList;
             BasicItemElement * BasicItemElement::headOfFreeList;
             Element2<RaceRule, int > * Element2<RaceRule, int >::headOfFreeList;
             BasicInventoryElement * BasicInventoryElement::headOfFreeList;
             BasicReportElement  * BasicReportElement::headOfFreeList;
             StanceElement * StanceElement::headOfFreeList;
             BasicResourceElement * BasicResourceElement::headOfFreeList;
             BasicMovementElement * BasicMovementElement::headOfFreeList;
#endif
