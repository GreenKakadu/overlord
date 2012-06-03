/*
 * File:   TurnReport.cpp
 * Author: alex
 *
 * Created on April 13, 2010, 2:44 PM
 */

#include "TurnReport.h"
#include <time.h>
#include "GameConfig.h"
#include "FileParser.h"
#include "SkillLevelElement.h"
#include "Entity.h"
#include "FactionEntity.h"
#include "Rule.h"
#include "Event.h"
#include "datatypes/GameData.h"
#include "datatypes/entities/Entity.h"
#include "TokenEntity.h"
#include "UnitEntity.h"
#include "ConstructionEntity.h"
#include "FactionEntity.h"
#include "EffectEntity.h"
#include "LocationEntity.h"
#include "DataManipulator.h"

//extern DataManipulator * dataManipulatorPtr;
TurnReport::TurnReport()
{
    faction_ = 0;
    section = REPORT_SECTION_END;
    currentFaction_ = 0;
    currentLocation_ = 0;
    currentUnit_ = 0;
    currentConstruction_ = 0;
    currentEffect_ = 0;
    currentKnowledge_ = 0;
    currentSkillKnowledge_ = 0;
    currentEvent_ = 0;
    currentDiscoveredLocation_=0;
}


TurnReport::~TurnReport()
{
}
/*
Initialization of turn report itself
*/
void TurnReport::init(FactionEntity * faction)
{
    turn_ =gameFacade->getGameTurn() ;
    game_ = gameFacade->getGameConfig()->getGameId();
    version_ = GameConfig::version;
    faction_ = faction;
    if(faction_)
    {
        filename_ =faction->getTag() + "report."+ longtostr(turn_);
    }
    else
    {
        filename_ ="report."+ longtostr(turn_);
    }

}



STATUS TurnReport::loadHeader()
{
  STATUS         status;
  string tag;

  parser_ = new FileParser(filename_.c_str());
  int n=0;
  if (parser_->status != OK)
  {
      status = IO_ERROR;
      return status;
  }
  status = OK;
  do
  {
    parser_->getLine();

       if (parser_->matchKeyword ("GAME") )
              {
              game_ = parser_->getText();
              continue;
              }

      if (parser_->matchKeyword("TURN"))
              {
              turn_ = parser_->getInteger();
              continue;
              }
      if (parser_->matchKeyword("REPORT"))
              {
              string factionTag = parser_->getWord();
              // In client it may happen that we have no data yet.
              // Special check for this case
              gameFacade->factions.getIndex(factionTag,false);
              if(gameFacade->factions.getStatus() != OK)
              {
                  FactionEntity * faction = new FactionEntity(&sampleFaction);
                  faction->setTag(factionTag);
                  gameFacade->factions.add(faction);
              }
      else
      {
              faction_ = gameFacade->factions[factionTag];
             if(faction_==0)
              {
                  FactionEntity * faction = new FactionEntity(&sampleFaction);
                  faction->setTag(factionTag);
                  gameFacade->factions.add(faction);
              }
      }
              continue;
              }
  }while (! parser_ ->eof() );
  delete parser_;
         return status;

}

// Only loads data placeholders to database in order to enable 
// cross-references in initialization process
// Adds new data to DB


STATUS TurnReport::load()
{
    STATUS         status;
    string tag;

    parser_ = new FileParser(filename_.c_str());
    int n=0;
    if (parser_->status != OK)
    {
        status = IO_ERROR;
        return status;
    }
    status = OK;
    do
    {
        n++;
        if(n>10000)
        {
            cout<<"Loop in TurnReport::load"<<endl;
            break;
        }
      parser_->getLine();
//      cout<<n<<" " <<parser_->report() <<endl;
//        if(n%100==99)
//        {
//            getchar();
//        }
	 if (parser_->matchKeyword ("GAME") )
    		{
      		game_ = parser_->getText();
      		continue;
    		}

 	if (parser_->matchKeyword("TURN"))
    		{
      		turn_ = parser_->getInteger();
      		continue;
    		}
 	if (parser_->matchKeyword("REPORT"))
    		{
                string factionTag = parser_->getWord();
                // In client it may happen that we have no data yet.
                // Special check for this case
                gameFacade->factions.getIndex(factionTag,false);
                if(gameFacade->factions.getStatus() != OK)
                {
                    FactionEntity * faction = new FactionEntity(&sampleFaction);
                    faction->setTag(factionTag);
                    gameFacade->factions.add(faction);
                }
        else
        {
      		faction_ = gameFacade->factions[factionTag];
               if(faction_==0)
                {
                    FactionEntity * faction = new FactionEntity(&sampleFaction);
                    faction->setTag(factionTag);
                    gameFacade->factions.add(faction);
                }
        }
      		continue;
    		}

	if (parser_->matchKeyword("SECTION"))
    		{
                section =  TurnReport::getSection();
      		continue;
        }

        switch (section) {
            case FACTIONS_SECTION:
            {
                // Load factions data
                if (parser_->matchKeyword("FACTION"))
                {
                    currentFaction_ = new FactionEntity(&sampleFaction);
                    tag = parser_->getWord();
                    currentFaction_->setTag(tag);
                    knownFactions_[tag]= currentFaction_;
                    gameFacade->factions.add(currentFaction_,false);
                   continue;
                }
                break;
            }

            case LOCATIONS_SECTION:
            {
                if (parser_->matchKeyword("LOCATION"))
                {
                    currentLocation_ = new LocationEntity(&sampleLocation);
                   tag = parser_->getWord();
                   currentLocation_->setTag(tag);
                    visitedLocations_[tag]= currentLocation_;
                    gameFacade->locations.add(currentLocation_,false);
                    continue;
                }
                break;
          }
            case DISCOVERED_LOCATIONS_SECTION:
            {
                if (parser_->matchKeyword("LOCATION"))
                {
                    currentDiscoveredLocation_ = new LocationEntity(&sampleLocation);
                    tag = parser_->getWord();
                    currentDiscoveredLocation_->setTag(tag);
                      discoveredLocations_[tag]= currentDiscoveredLocation_;
                     gameFacade->locations.add(currentLocation_,false);
                   continue;
                }
                break;
          }
        case ENTITIES_SECTION: // Not implemented
          {
              break;
          }
        case UNITS_SECTION:
          {
                if (parser_->matchKeyword("UNIT"))
                {

                    currentUnit_ = new UnitEntity(&sampleUnit);
                    tag = parser_->getWord();
                    currentUnit_->setTag(tag);
                    observedUnits_[tag] =currentUnit_;
                    gameFacade->units.add(currentUnit_,false);
                    continue;
                }
             break;
          }
          case CONSTRUCTIONS_SECTION:
          {
               if (parser_->matchKeyword("BUILDING"))
                {
                   currentConstruction_ = new ConstructionEntity(&sampleConstructionEntity);
                   tag = parser_->getWord();
                    currentConstruction_->setTag(tag);
                    observedConstructions_[tag] = currentConstruction_;
                    gameFacade->buildingsAndShips.add(currentConstruction_,false);
                   continue;
                }
              break;
          }
          case EFFECTS_SECTION:
          {
               if (parser_->matchKeyword("EFFECT"))
                {
                   currentEffect_ = new EffectEntity(&sampleEffectEntity);
                   tag = parser_->getWord();
                    currentEffect_->setTag(tag);
                    observedEffects_[tag] = currentEffect_;
                    gameFacade->effects.add(currentEffect_,false);
                    continue;
                }
              break;
          }
          case RULES_KNOWLEDGE_SECTION:
          {
               if (parser_->matchKeyword("RULES_KNOWLEDGE"))
                {
                BasicRulesCollection  * collection =  gameFacade->getDataManipulator()->findRuleCollection(parser_->getWord());
                if(collection)
                {
                        string tag = parser_->getWord();
                      GameData * data = collection->findByTag(tag,false);
//                      if(tag=="man")
//                      {
//                          cout<<"<==================== creating "<<tag <<" ..."<<endl;
//                      }
                  //GameData * data = collection->findByTag(parser_->getWord(),false);
                  // Check if this rule (parser_->getWord())  already exist in collection
                  // if yes - do nothing
                  // if not - create new rule and add it to collection
                  if(data==0)
                  {
                      STATUS status;
                      data = DataStorageHandler::loadGameData(parser_,tag,collection->getCollectionKeyword(),&status);
                      collection->add(data);
//                      if(tag=="hero")
//                      {
//                          cout<<"<==================== ";
//                          if(data)
//                          {
//                              cout <<data->getTag();
//                          }
//                          cout<<" added to " <<collection->getCollectionKeyword() <<endl;
//                      }
                  }
                      Rule * rule = dynamic_cast<Rule *>(data);
                      if(rule)
                      {
                      rules_.push_back(rule);
                      //cout<<rule->getTag()<< " added to knowledge"<<endl;
                      }
                }

               }

              break;
        }
        case SKILL_KNOWLEDGE_SECTION:
        {
            if (parser_->matchKeyword("SKILL_ELEMENT"))
            {
                int level;
                string skillTag = parser_->matchWord();
                if (skillTag.empty())
                    break;
//                if(skillTag=="frai")
//                {
//                    cout<<"<== loading skill "<<skillTag <<endl;
//                }
                SkillRule * skill = dynamic_cast<SkillRule *>(gameFacade->skills.findByTag(skillTag,false));
                if (skill == 0)
                {
                    // add new skill to collection
                    skill = new SkillRule(&sampleSkill);
                    skill->setTag(skillTag);
                    gameFacade->skills.add(skill);
                }

                parser_->getWord();

                if (parser_->matchInteger())
                    level = parser_->getInteger();
                else
                    level = 1;

                currentSkillKnowledge_ = new SkillLevelElement(skill, level);

                skillKnowledge_.push_back(currentSkillKnowledge_);
            }
            continue;
            break;
        }
          case EVENTS_SECTION:
          {
              if (parser_->matchKeyword("EVENT"))
              {
                  string tag = parser_->matchWord();
            	  Event * newEvent = new Event(&sampleEvent);
            	  newEvent->setTag(tag);
            	  observedEvents_[tag] = newEvent;
            	  gameFacade->events.addEvent(newEvent);
              }
              break;
          }
          case COMBAT_SECTION:// Not implemented
          {
              break;
          }
          case MESSAGES_SECTION:// Not implemented
          {
              break;
          }
          default:
              continue;

      }
 
    } while (! parser_ ->eof() );
 delete parser_;
        return status;

}
/*
Initialization of data contained in  turn report
*/

STATUS TurnReport::initialize()
{
    STATUS         status;
    string tag;
    parser_ = new FileParser(filename_.c_str());
    int n=0;
    if (parser_->status != OK)
    {
        status = IO_ERROR;
        return status;
    }
    status = OK;
    do
    {
        n++;
        if(n>10000)
        {
            cout<<"Loop in TurnReport::initialize"<<endl;
            break;
        }
      parser_->getLine();
      //cout<<n<<" " <<endl;


	if (parser_->matchKeyword("SECTION"))
    		{
                section =  TurnReport::getSection();
      		continue;
        }

        switch (section) {
            case FACTIONS_SECTION:
            {
                // Load factions data
                if (parser_->matchKeyword("FACTION"))
                {
                    //<<================================ HERE
                    tag =parser_->getWord();
                    map<string, FactionEntity *>::iterator iter = knownFactions_.find(tag);
                    if(iter != knownFactions_.end())
                    {
                       currentFaction_ = iter->second;
                    }

                   continue;
                }
                if (currentFaction_)
                {
                    currentFaction_->initialize(parser_);
                }
                break;
            }

            case LOCATIONS_SECTION:
            {
                if (parser_->matchKeyword("LOCATION"))
                {
                    tag =parser_->getWord();
                    map<string, LocationEntity *>::iterator iter = visitedLocations_.find(tag);
                      if(iter != visitedLocations_.end())
                    {
                       currentLocation_ = iter->second;
                    }
                    continue;
               }
                if (currentLocation_)
                {
                    currentLocation_->initialize(parser_);
                }
                break;
          }
            case DISCOVERED_LOCATIONS_SECTION:
            {
                if (parser_->matchKeyword("LOCATION"))
                {
                    tag =parser_->getWord();
                    map<string, LocationEntity *>::iterator iter = discoveredLocations_.find(tag);
                      if(iter != discoveredLocations_.end())
                    {
                       currentDiscoveredLocation_ = iter->second;
                    }
                    continue;
                }
               if (currentDiscoveredLocation_)
               {
                    currentDiscoveredLocation_->initialize(parser_);
                }
                // Load locations data
                break;
          }
        case ENTITIES_SECTION: // Not implemented
          {
              break;
          }
        case UNITS_SECTION:
          {
                if (parser_->matchKeyword("UNIT"))
                {

                    currentUnit_ = new UnitEntity(&sampleUnit); // Why create? It should be taken from already loaded observedUnits_
                    tag =parser_->getWord();
                    currentUnit_->setTag(tag);
                    map<string, UnitEntity *>::iterator iter = observedUnits_.find(tag);
                      if(iter != observedUnits_.end())
                    {
                       currentUnit_ = iter->second;
                    }
                    continue;
                }
                if (currentUnit_)
                {
                    currentUnit_->initialize(parser_);
                }
             break;
          }
          case CONSTRUCTIONS_SECTION:
          {
               if (parser_->matchKeyword("BUILDING"))
                {
                   currentConstruction_ = new ConstructionEntity(&sampleConstructionEntity);
                   tag =parser_->getWord();
                    currentConstruction_->setTag(tag);
                    map<string,ConstructionEntity  *>::iterator iter =observedConstructions_.find(tag);
                      if(iter != observedConstructions_.end())
                    {
                      currentConstruction_  = iter->second;
                    }
                   continue;
                }
                if (currentConstruction_)
                {
                    currentConstruction_->initialize(parser_);
                }
              break;
          }
          case EFFECTS_SECTION:
          {
               if (parser_->matchKeyword("EFFECT"))
                {
                   currentEffect_ = new EffectEntity(&sampleEffectEntity);
                   tag =parser_->getWord();
                    currentEffect_->setTag(tag);
                   map<string, EffectEntity *>::iterator iter = observedEffects_.find(tag);
                      if(iter != observedEffects_.end())
                    {
                       currentEffect_ = iter->second;
                    }
                    continue;
                }
                if (currentEffect_)
                {
                    currentEffect_->initialize(parser_);
                }
              break;
          }
          case RULES_KNOWLEDGE_SECTION:
          {
               if (parser_->matchKeyword("RULES_KNOWLEDGE"))
                {
//                    string tag = parser_->getWord();
//                  if(tag=="hrse")
//                  {
//                      cout<<"<==================== Horse init"<<endl;
//                  }


                BasicRulesCollection  * collection =  gameFacade->getDataManipulator()->findRuleCollection(parser_->getWord());
                if(collection)
                {
                      string tag = parser_->getWord();
                      currentKnowledge_ = dynamic_cast<Rule *>(collection->findByTag(tag,false));
//                      if(tag=="hero")
//                      {
//                          cout<<"<==================== initializing hero..."<<endl;
//                      }
 //                 currentKnowledge_ = dynamic_cast<Rule *>(collection->findByTag(parser_->getWord(),false));

               }
               }
                if(currentKnowledge_)
                {
                    currentKnowledge_->initialize(parser_);
                }

              break;
        }
        case SKILL_KNOWLEDGE_SECTION:
        {
            if (parser_->matchKeyword("SKILL_ELEMENT"))
            {
                currentSkillKnowledge_ = SkillLevelElement::readElement(parser_);
            }
            if (currentSkillKnowledge_)
            {
                int currentLevel = currentSkillKnowledge_->getLevel();
                SkillRule * skill = currentSkillKnowledge_->getSkill();
//                if(skill==skills["frai"])
//                {
//                    cout<<"<== reading skill element"<<skill->getTag()<<" "<<currentLevel<<endl;
//                }
               // skill->initLevel(currentLevel);// TEmp. <---------------------------TEMP
                if (parser_->matchKeyword("LEVEL"))
                {
                    if (currentLevel >SkillRule::getMaxSkillLevel())
                    {
                        cerr << "Error : In Report for " << skill->print() << " Skill Level " << currentLevel
                                << " is higher than maximum(" << SkillRule::getMaxSkillLevel() << ")\n";
                        currentSkillKnowledge_ = 0;
                        break;
                    }
                    skill->setExpForLevel(currentLevel,parser_->getInteger());

                }
                skill->initializeLevel(parser_, currentLevel);

            }
            break;
        }
          case EVENTS_SECTION:
          {
            if (parser_->matchKeyword("EVENT"))
            {
                tag =parser_->getWord();
                currentEvent_ = gameFacade->events.findEvent(tag);
                continue;
            }
            if (currentEvent_)
            {
                currentEvent_->initialize(parser_);
            }

              break;
          }
          case COMBAT_SECTION:// Not implemented
          {
              break;
          }
          case MESSAGES_SECTION:// Not implemented
          {
              break;
          }
          default:
              continue;

      }

    } while (! parser_ ->eof() );
 delete parser_;
        return status;


    return OK;
}



REPORT_SECTION TurnReport::getSection()
//void TurnReport::sectionInit()
{
    //cout<<"getSection() "<<endl;
	if (parser_->matchKeyword("FACTIONS"))
        {
            return FACTIONS_SECTION;
        }
	if (parser_->matchKeyword("LOCATIONS"))
        {
            return LOCATIONS_SECTION;
        }
	if (parser_->matchKeyword("ENTITIES"))
        {
            return ENTITIES_SECTION;
        }
	if (parser_->matchKeyword("UNITS"))
        {
            return UNITS_SECTION;
        }
	if (parser_->matchKeyword("CONSTRUCTIONS"))
        {
            return CONSTRUCTIONS_SECTION;
        }
	if (parser_->matchKeyword("EFFECTS"))
        {
            return EFFECTS_SECTION;
        }
	if (parser_->matchKeyword("RULES_KNOWLEDGE"))
        {
            return RULES_KNOWLEDGE_SECTION;
        }
	if (parser_->matchKeyword("SKILL_KNOWLEDGE"))
        {
            return SKILL_KNOWLEDGE_SECTION;
        }
	if (parser_->matchKeyword("EVENTS"))
        {
            return EVENTS_SECTION;
        }
	if (parser_->matchKeyword("COMBAT"))
        {
            return COMBAT_SECTION;
        }
	if (parser_->matchKeyword("MESSAGES"))
        {
            return MESSAGES_SECTION;
        }
	if (parser_->matchKeyword("DISCOVERED_LOCATIONS"))
        {
            return DISCOVERED_LOCATIONS_SECTION;
        }
        
return REPORT_SECTION_END;
}


//// We know that initialization of some
//void TurnReport::postLoad()
//{
//    if(currentFaction_)
//    {
//      knownFactions_.push_back(currentFaction_);
//      currentFaction_=0;
//    }
//                 if (currentLocation_) // Initialization of the current object is finished
//                    {
//                      visitedLocations_.push_back(currentLocation_);
//                      currentLocation_ =0;
//                    }
//
//                  if (currentDiscoveredLocation_) // Initialization of the current object is finished
//                    {
//                      discoveredLocations_.push_back(currentDiscoveredLocation_);
//                      currentDiscoveredLocation_ =0;
//                    }
//
//                  if (currentUnit_) // Initialization of the current object is finished
//                    {
//                       observedUnits_.push_back(currentUnit_);
//                       currentUnit_ =0;
//                    }
//
//                   if (currentConstruction_) // Initialization of the current object is finished
//                    {
//                       observedConstructions_.push_back(currentConstruction_);
//                       currentConstruction_ =0;
//                    }
//
//   if (currentEffect_) // Initialization of the current object is finished
//    {
//       observedEffects_.push_back(currentEffect_);
//       currentEffect_ =0;
//    }
//
//    //currentKnowledge_
//    //currentSkillKnowledge_
//    //currentEvent_
//
//
//}

void TurnReport::save()
{
    time_t rawtime;
  ofstream outfile (filename_.c_str());
  outfile << "# Overlord turn "<<gameFacade->getGameTurn()<<" report for "<<faction_->print() <<endl;
  time ( &rawtime );
  outfile << "# Ver " <<  GameConfig::version <<" " <<ctime(&rawtime) <<endl;
  cout << "Writing " <<filename_.c_str()<< endl;

    outfile <<"GAME "<< gameFacade->getGameConfig()->getGameId()<<endl;
    outfile <<"TURN "<< gameFacade->getGameTurn()<<endl;
    outfile <<"REPORT "<< getFaction()->getTag()<<endl;
    outfile <<"SECTION "<<"FACTIONS "<<endl;
    // First report own faction:
    //getFaction()->save(outfile);
//    for(map<string, FactionEntity *>::iterator iter =knownFactions_.begin(); iter != knownFactions_.end();++iter )
    for(auto iter =knownFactions_.begin(); iter != knownFactions_.end();++iter )// Example of using C++0x
      {
            iter->second->save(outfile);
      }

      // Save Entities Data
 outfile <<"SECTION "<<"LOCATIONS "<<endl;
 for(map<string, LocationEntity *>::iterator iter =visitedLocations_.begin(); iter != visitedLocations_.end();++iter )
  {
        iter->second->save(outfile);
  }
 for(map<string, LocationEntity *>::iterator iter = discoveredLocations_.begin(); iter != discoveredLocations_.end();++iter )
 {
       iter->second->save(outfile);
 }
// for(auto x:discoveredLocations_)         x.second->save(outfile);

outfile <<"SECTION "<<"ENTITIES "<<endl;
outfile <<"SECTION "<<"UNITS "<<endl;
// First report own units:

for(vector<UnitEntity *>::iterator iter = getFaction()->getLoyalUnits().begin();
		iter != getFaction()->getLoyalUnits().end();
		++iter )
 {
       (*iter)->save(outfile);
 }
// Report collected images:

 for(map<string, UnitEntity *>::iterator iter = observedUnits_.begin(); iter != observedUnits_.end();++iter )
 {
      if(!getFaction()->isLoyalUnit(iter->second))
     {
          iter->second->save(outfile);
      }
 }
 outfile <<"SECTION "<<"CONSTRUCTIONS "<<endl;
 // First report own units:

 for(vector<ConstructionEntity *>::iterator iter = getFaction()->getLoyalConstructions().begin();
 		iter != getFaction()->getLoyalConstructions().end();
 		++iter )
  {
        (*iter)->save(outfile);
  }
 //  report collected images:

  for(map<string, ConstructionEntity *>::iterator iter = observedConstructions_.begin(); iter != observedConstructions_.end();++iter )
  {
      if(!getFaction()->isLoyalConstruction(iter->second))
     {
        iter->second->save(outfile);
    }
  }
  outfile <<"SECTION "<<"EFFECTS "<<endl;
  for(map<string, EffectEntity *>::iterator iter = observedEffects_.begin(); iter != observedEffects_.end();++iter )
  {
        iter->second->save(outfile);
  }
    // Save Rules Data
   outfile <<"SECTION "<<"RULES_KNOWLEDGE"<<endl;
 for(vector<Rule *>::iterator iter = rules_.begin(); iter != rules_.end();++iter )
 {
   outfile <<endl<<"RULES_KNOWLEDGE"<<" "; // Prefix for turn reports.
       (*iter)->save(outfile);
 }
   outfile <<"SECTION "<<"SKILL_KNOWLEDGE"<<endl;
 for(vector<SkillLevelElement *>::iterator iter = skillKnowledge_.begin(); iter != skillKnowledge_.end();++iter )
 {
//     if(getFaction() ==factions["f02"])
//     {
//     cout<<"Saving SkillLevelElement "<<(*iter)->print()<<endl;
//     }
       (*iter)->saveLevelElement(outfile);
 }

    // Save Events Data
 outfile <<"SECTION "<<"EVENTS "<<endl;
 for(map<string, Event *>::iterator iter = observedEvents_.begin(); iter != observedEvents_.end();++iter )
  {
        iter->second->save(outfile);
  }
// for(map<string, Event *>::iterator iter = globalEvents_.begin(); iter != globalEvents_.end();++iter )
//  {
//        iter->second->save(outfile);
//  }

    // Save Combat Data
 outfile <<"SECTION "<<"COMBAT "<<endl;
 //TBD CombatReport:
 // list of participants, their locations, their moves,
 // events, result loot survivers
   // Save Messages
 outfile <<"SECTION "<<"MESSAGES "<<endl;
 //TBD
}

void TurnReport::addSkillKnowledge(SkillRule * knowledge, int level)
{
//    if (getFaction() == factions["f08"])
//    {
//        cout << " ======> adding " << knowledge->getTag() << " " << level << endl;
//    }
    for (SkillLevelIterator iter = skillKnowledge_.begin(); iter != skillKnowledge_.end(); ++iter)
    {
        if ((*iter)->getSkill() == knowledge)
        {
            if ((*iter)->getLevel() >= level)
            {
                return;
            }
            else
            {
                (*iter)->setLevel(level);
                return;
            }
        }
    }
//    if (getFaction() == factions["f02"])
//    {
//        cout << " ======> " << knowledge->getTag() << " " << level << " was added" << endl;
//    }
    skillKnowledge_.push_back(new SkillLevelElement(knowledge, level));
}

// This function is not used
//void TurnReport::addEntityImage(Entity * image)
//{
//  for(vector<Entity *>::iterator iter = entities_.begin(); iter != entities_.end(); ++iter)
//  {
//    if((*iter)->getTag()==image->getTag())
//    {
//       (*iter)->updateImage(image);
//       return;
//    }
//    else
//    {
//      entities_.push_back(image);
//    }
//  }
//
//}

Entity *
TurnReport::findEntityImage(Entity * data)
{

  UnitEntity * entity = dynamic_cast<UnitEntity *> (data);
  if (entity != 0)
    {
      for (auto iter = getFaction()->getLoyalUnits().begin(); iter
          != getFaction()->getLoyalUnits().end(); ++iter)
        {
          if ((*iter)->getTag() == entity->getTag())
            return (*iter);
        }
      auto iter = observedUnits_.find(entity->getTag());
      if (iter != observedUnits_.end())
        {
          return (*iter).second;
        }
      return gameFacade->unknownUnit;
    }

    {// Block defined in order to reuse the same names for variables
      ConstructionEntity * entity = dynamic_cast<ConstructionEntity *> (data);
      if (entity != 0)
        {
          for (auto iter = getFaction()->getLoyalConstructions().begin(); iter
              != getFaction()->getLoyalConstructions().end(); ++iter)
            {
              if ((*iter)->getTag() == entity->getTag())
                return (*iter);
            }
          auto iter = observedConstructions_.find(entity->getTag());
          if (iter != observedConstructions_.end())
            {
              return (*iter).second;
            }
          return gameFacade->unknownConstruction;
        }
    }//Block end

    {// Block defined in order to reuse the same names for variables
      LocationEntity * entity = dynamic_cast<LocationEntity *> (data);
      if (entity != 0)
        {

          auto iter = visitedLocations_.find(entity->getTag());
          if (iter != visitedLocations_.end())
            {
              return (*iter).second;
            }
          {// Block defined in order to reuse the same names for variables
          auto iter = discoveredLocations_.find(entity->getTag());
          if (iter != discoveredLocations_.end())
            {
              return (*iter).second;
            }
          return gameFacade->unknownLocation;
          }//Block end
        }
    }//Block end
    {// Block defined in order to reuse the same names for variables
      EffectEntity * entity = dynamic_cast<EffectEntity *> (data);
      if (entity != 0)
        {
          auto iter = observedEffects_.find(entity->getTag());
           if (iter != observedEffects_.end())
             {
               return (*iter).second;
             }
          return gameFacade->unknownEffect;
        }
    }//Block end

  return 0;
}


// add LocationImage to faction. replace if older exists.
void TurnReport::addLocationImage(LocationEntity * locationToAdd)
{
    if (locationToAdd)
    {
        int observation = locationToAdd->getFactionalObservation(getFaction());
        string tag = locationToAdd->getTag();
        LocationEntity * imageToAdd = locationToAdd->createVisitedLocationImage(getFaction(), observation);
        imageToAdd->extractAndAddKnowledge(getFaction());
        map<string, LocationEntity *>::iterator iter = visitedLocations_.find(tag);
        if (iter != visitedLocations_.end())
        {
            iter->second->updateImage(imageToAdd);
        }
        else // not there yet
        {
//            if(getFaction() ==factions["f02"])
//            {
//        cerr << "Adding data for " << imageToAdd->print() << " for "
//			<< getFaction()->print();
//	cerr << "  ...adding as visited." << endl;
//            }
           visitedLocations_[tag] = imageToAdd;
            locationToAdd->addNeighbours(this);
            discoveredLocations_.erase(tag);// Remove from discovered if any.

        }
    }
}

 /*
  * Add location to the list of discovered locations
  * if it is not yet in this list
  * These are locations, connected to visited locations
  */
 void TurnReport::addDiscoveredLocation(LocationEntity * location)
{
    if (location)
    {
       //cerr << "Adding data for " << location->print() << " for "
	//		<< getFaction()->print() << endl;
        string tag = location->getTag();
        if (visitedLocations_.find(tag) != visitedLocations_.end())
        {
            return;
        }

        if (discoveredLocations_.find(tag) != discoveredLocations_.end())
        {
            return;
        }
//            if(getFaction() ==factions["f02"])
//            {
//      cerr << "Adding data for " << location->print() << " for "
//			<< getFaction()->print();
//	cerr << " ...adding as discovered." << endl;
//            }
        LocationEntity * image = location->createBasicImage(getFaction());
        image->extractAndAddKnowledge(getFaction());
	discoveredLocations_[tag]= image;
    }
}

UnitEntity * TurnReport::addUnitImage(UnitEntity * unitToAdd, int observation)
{
    if (unitToAdd)
    {
//        if (unitToAdd->getFaction() == getFaction())
//        {
//            return 0;
//        }
        string tag = unitToAdd->getTag();
        addFactionImage(unitToAdd->getFaction());
        map<string, UnitEntity *>::iterator iter = observedUnits_.find(tag);
        if (iter != observedUnits_.end())
        {
            iter->second->updateImage(unitToAdd);
            return unitToAdd;
        }
//        if(tag=="U807" && getFaction()->getTag()=="f08")
//        {
//            cout<<"Creating image of "<<tag<<" for "<<getFaction()->getTag()<<endl;
//        }
//

        UnitEntity * imageToAdd = unitToAdd->createUnitImage(getFaction(),
                                                             observation);
        if (imageToAdd)
        {
            imageToAdd->extractAndAddKnowledge(getFaction());
            observedUnits_[tag] = imageToAdd;
        }
        return imageToAdd;
    }
    return 0;
}

ConstructionEntity * TurnReport::addConstructionImage(
                                                      ConstructionEntity * constructionToAdd, int observation)
{
    if (constructionToAdd)
    {
//        if (constructionToAdd->getFaction() == getFaction())
//        {
//            return 0;
//        }
//         if(getFaction() == factions["f02"]) cout<<"===} Adding "<<constructionToAdd->getTag()<<" ["<<constructionToAdd->getFaction()->getTag() <<"] to "<<getFaction()->getTag()<<endl;
       addFactionImage(constructionToAdd->getFaction());
        string tag = constructionToAdd->getTag();
        map<string, ConstructionEntity *>::iterator iter = observedConstructions_.find(tag);
        if (iter != observedConstructions_.end())
        {
            iter->second->updateImage(constructionToAdd);
            return constructionToAdd;
        }


        ConstructionEntity * imageToAdd =
                constructionToAdd->createConstructionImage(getFaction(),
                                                           observation);
        if (imageToAdd)
        {
            imageToAdd->extractAndAddKnowledge(getFaction());
            observedConstructions_[tag] = imageToAdd;
        }
        return imageToAdd;
    }

    return 0;
}

EffectEntity * TurnReport::addEffectImage(EffectEntity * effectToAdd, int observation)
{
    if (effectToAdd)
    {
//        if (effectToAdd->getFaction() == getFaction())
//        {
//            return 0;
//        }
        addFactionImage(effectToAdd->getFaction());

        string tag = effectToAdd->getTag();
        map<string, EffectEntity *>::iterator iter = observedEffects_.find(tag);
        if (iter != observedEffects_.end())
        {
            iter->second->updateImage(effectToAdd);
            return iter->second;
        }


        EffectEntity * imageToAdd = effectToAdd->createEffectImage(getFaction(), observation);
        if (imageToAdd)
        {
            imageToAdd->extractAndAddKnowledge(getFaction());
            observedEffects_[tag]= imageToAdd;
        }
        return imageToAdd;
    }
    return 0;
}



// This function extracts faction image from image
void TurnReport::addFactionImage(FactionEntity * factionToAdd)
{
    //FactionEntity * factionToAdd = image->getFaction();
    if (factionToAdd == 0)
    {
        return;
    }
    if(factionToAdd == getFaction())
    {
    	return;
    }
    //cerr<< "Adding image of "<<factionToAdd->print()<<" to TurnReport for "<<getFaction()->print()<<" ";
       string tag = factionToAdd->getTag();
       map<string, FactionEntity *>::iterator iter = knownFactions_.find(tag);
        if (iter != knownFactions_.end())
        {
            iter->second->updateImage(factionToAdd);
            return;
        }

//    for(map<string, FactionEntity *>::iterator iter = knownFactions_.begin();
//            iter != knownFactions_.end(); ++iter)
//    {
//        if (iter->second->getTag() == factionToAdd->getTag())
//        {
//            iter->second->updateImage(factionToAdd);
//            //cerr<<factionToAdd->print()<<" already recorded."<< endl;
//           return;
//        }
//
//    }
//    FactionEntity * imageToAdd = factionToAdd->createFactionImage(getFaction());
   //cerr<<imageToAdd->print()<<" added"<< endl;
        knownFactions_[tag]=factionToAdd->createFactionImage(getFaction());
}



//void TurnReport::addGlobalEvent(Event * event)
//{
//	string tag = event->getTag();
//	if(globalEvents_.find(tag) == globalEvents_.end())
//	{
//		globalEvents_[tag] = event;
//	}
//
//}
Event * TurnReport::addEventImage(Event * event)
{
	string tag = event->getTag();
	if(observedEvents_.find(tag) == observedEvents_.end())
	{
		observedEvents_[tag] = event;
		return event;
	}
return 0;
}
