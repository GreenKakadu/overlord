/***************************************************************************
                          Global.cpp  -  Global variables
                             -------------------
    begin                : Wed Nov 27 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "Global.h"
#include "general/DataStorageHandler.h"


int currentDay = 1;
const int BLOCK_SIZE = 1024;

template class EntitiesCollection<UnitEntity>;
template class RulesCollection<TerrainRule>;
template class VarietiesCollection<MovementVariety>;
PrototypeManager * GameData::prototypeManager= new PrototypeManager;

GameInfo game;
GameData      sampleGameData  ("GameData",0);
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
UnitEntity     sampleUnit    ("UNIT",    &sampleEntity);
FactionEntity  sampleFaction ("FACTION", &sampleEntity);
LocationEntity sampleLocation("LOCATION",&sampleEntity);
// Letters
BasicLearningLetter    sampleLearning         ("LEARNING",          &sampleGameData);
MagicLearningLetter    sampleMagicLearning    ("LEARNING_MAGIC",    &sampleLearning);
NormalLearningLetter   sampleNormalLearning   ("LEARNING_NORMAL",   &sampleLearning);
CreatureLearningLetter sampleCreatureLearning ("LEARNING_CREATURE", &sampleLearning);
BasicUseLetter         sampleUsing            ("USING",             &sampleGameData);
HarvestUsingLetter     sampleHarvestUsing     ("USING_HARVEST",     &sampleUsing);
CraftUsingLetter       sampleCraftUsing       ("USING_CRAFT",       &sampleUsing);
SummonUsingLetter      sampleSummonUsing      ("USING_SUMMON",      &sampleUsing);
CombatUsingLetter      sampleCombatUsing      ("USING_COMBAT",      &sampleUsing);
BuildUsingLetter       sampleBuildUsing       ("USING_BUILD",       &sampleUsing);
BasicStudentLetter     sampleStudent          ("STUDENT",           &sampleGameData);
LeaderStudentLetter    sampleLeaderStudent    ("STUDENT_LEADER",    &sampleStudent);
FollowerStudentLetter  sampleFollowerStudent  ("STUDENT_FOLLOWER",  &sampleStudent);
CreatureStudentLetter  sampleCreatureStudent  ("STUDENT_CREATURE",  &sampleStudent);

// Varieties Collections
VarietiesCollection <MovementVariety> movementModes(new DataStorageHandler("movements"));
VarietiesCollection <DirectionVariety>     directions(new DataStorageHandler("directions"));
VarietiesCollection <StanceVariety>    stances(new DataStorageHandler("stances"));
VarietiesCollection <EquipmentSlotVariety>      equipments(new DataStorageHandler("equipments"));
//MovementVariety * walkingMode = movementModes["walk"];

// Rules Collections

RulesCollection <TerrainRule>   terrains(new DataStorageHandler("terrains"));
RulesCollection <TitleRule>     titles(new DataStorageHandler("titles"));
RulesCollection <ItemRule>      items(new DataStorageHandler("items"));
RulesCollection <SkillRule>     skills(new DataStorageHandler("skills"));
RulesCollection <RaceRule>      races(new DataStorageHandler("races"));

EntitiesCollection <UnitEntity>   units(new DataStorageHandler(game.getUnitsFile()));
EntitiesCollection <FactionEntity>   factions(new DataStorageHandler(game.getFactionsFile() ));
EntitiesCollection <LocationEntity>   locations(new DataStorageHandler(game.getLocationsFile() ));


OrderPrototypesCollection orderPrototypesCollection;

bool testMode = false;
ProcessingMode   immediateOrders  (IMMEDIATE_ORDER);
ProcessingMode  stackOrders  (STACK_ORDER);
ProcessingMode 	 dayOrders  (DAY_LONG_ORDER);
TravelElement sample(0,0,0,0,0);


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
#include "elements/Element.h"
#include "elements/ItemElement.h"
#include "elements/RaceElement.h"
#include "elements/ReportElement.h"
#include "elements/RecruitElement.h"
#include "elements/MovementElement.h"
// Template instantiation
#ifdef BCC
template  <> BasicSkillElement * BasicSkillElement::headOfFreeList;
template  <> BasicItemElement * BasicItemElement::headOfFreeList;
template  <> BasicRaceElement * BasicRaceElement::headOfFreeList;
template  <> BasicInventoryElement * BasicInventoryElement::headOfFreeList;
template  <> BasicReportElement  * BasicReportElement::headOfFreeList;
template  <> StanceElement * StanceElement::headOfFreeList;
template  <> BasicRecruitElement * BasicRecruitElement::headOfFreeList;
template  <> BasicMovementElement * BasicMovementElement::headOfFreeList;
#else
             BasicSkillElement * BasicSkillElement::headOfFreeList;
             BasicItemElement * BasicItemElement::headOfFreeList;
             Element2<RaceRule, int > * Element2<RaceRule, int >::headOfFreeList;
             BasicInventoryElement * BasicInventoryElement::headOfFreeList;
             BasicReportElement  * BasicReportElement::headOfFreeList;
             StanceElement * StanceElement::headOfFreeList;
             BasicRecruitElement * BasicRecruitElement::headOfFreeList;
             BasicMovementElement * BasicMovementElement::headOfFreeList;
#endif
