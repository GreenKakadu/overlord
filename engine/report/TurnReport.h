/* 
 * File:   TurnReport.h
 * Author: alex
 *
 * Created on April 13, 2010, 2:44 PM
 */

#ifndef _TURNREPORT_H
#define	_TURNREPORT_H
#include <vector>
#include <map>
#include <string>
#include "OverlordTypes.h"
class Parser;
class Entity;
class Rule;
class SkillRule;
class SkillLevelElement;

class TokenEntity;
class UnitEntity;
class ConstructionEntity;
class FactionEntity;
class EffectEntity;
class Event;
class LocationEntity;
using namespace std;
enum report_section {
    FACTIONS_SECTION = 0,
    LOCATIONS_SECTION = 1,
    ENTITIES_SECTION = 2,
    UNITS_SECTION = 3,
    CONSTRUCTIONS_SECTION = 4,
    EFFECTS_SECTION = 5,
    RULES_KNOWLEDGE_SECTION = 6,
    SKILL_KNOWLEDGE_SECTION = 7,
    EVENTS_SECTION = 8,
    COMBAT_SECTION = 9,
    MESSAGES_SECTION = 10,
    DISCOVERED_LOCATIONS_SECTION = 11,
    REPORT_SECTION_END = 99
};
typedef enum report_section REPORT_SECTION;

class TurnReport {
public:
    TurnReport();

    virtual ~TurnReport();
    // Initialize turn report with game data
    void init(FactionEntity * faction);
    //void sectionInit();
    // Load only header of turnReport from file
    STATUS loadHeader();
    // Load turnReport from file
    STATUS load();
    // Save  turnReport to file
    void save();
    // Second stage of loading  turnReport to file
    STATUS initialize();
    //void setFaction(FactionEntity * faction){faction_ = faction;}
    UnitEntity * addUnitImage(UnitEntity * unitToAdd, int observation);
    ConstructionEntity * addConstructionImage(ConstructionEntity * constructionToAdd, int observation);
    EffectEntity * addEffectImage(EffectEntity * effectToAdd, int observation);
    Event * addEventImage(Event * event);
    Entity * findEntityImage(Entity * data);
    void addLocationImage(LocationEntity * locationToAdd);
    void addFactionImage(FactionEntity * factionToAdd);
 //   void addGlobalEvent(Event * event);
    void addRule(Rule * rule){rules_.push_back(rule);}
    void addSkillKnowledge(SkillRule * knowledge, int level);
    void addDiscoveredLocation(LocationEntity * location);
    //QQQvoid addEntity(Entity * entity){entities_.push_back(entity);}
    FactionEntity * getFaction(){return faction_;}

    vector<Rule *> getAllRules(){return rules_;}
    vector<SkillLevelElement *> getAllSkillKnowledge(){return skillKnowledge_;}
    inline string getGame() {return game_;}
    REPORT_SECTION getSection();
    void setReportName(string str){filename_ =str;}
    //void postLoad();
    // add rules?
private:
    Parser * parser_;
    string filename_;
    int turn_;
    string game_;
    string version_;
    FactionEntity * faction_;

    // Store here Images of Entities i.e. Entities as Faction sees them
    //vector<Entity * > entities_;
    vector<Rule * > rules_;
    vector<SkillLevelElement *> skillKnowledge_;
       REPORT_SECTION section;

     map<string, LocationEntity *> discoveredLocations_;
     map<string, LocationEntity *> visitedLocations_;
     map<string, UnitEntity *> observedUnits_;
     map<string, ConstructionEntity *> observedConstructions_;
     map<string, EffectEntity *> observedEffects_;
     map<string, FactionEntity *> knownFactions_;
     map<string, Event *> observedEvents_;
    // map<string, Event *> globalEvents_;

     FactionEntity * currentFaction_;
     LocationEntity * currentLocation_;
     UnitEntity * currentUnit_;
     ConstructionEntity * currentConstruction_;
     EffectEntity * currentEffect_;
     Rule * currentKnowledge_;
     SkillLevelElement * currentSkillKnowledge_;
     Event * currentEvent_;
     LocationEntity * currentDiscoveredLocation_;
   };

#endif	/* _TURNREPORT_H */

