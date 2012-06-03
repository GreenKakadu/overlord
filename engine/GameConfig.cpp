/***************************************************************************
                          GameConfig.cpp  -  description
                             -------------------
    begin                : Wed Nov 20 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "stdio.h"
#include <time.h>
#include "GameConfig.h"
#include "FileParser.h"
#include "FactionEntity.h"
#include "EntitiesCollection.h"
extern string longtostr(long u);

GameConfig::GameConfig()
{
		unitsFile_          = string("units.dat");
		locationsFile_      = string("locations.dat");
		factionsFile_       = string("factions.dat");
        buildingsFile_      = string("buildings.dat");
        effectsFile_        = string("effects.dat");
//Vars 
        combat_filesFile_   = string("combat_files.var");
        combat_movesFile_   = string("combat_moves.var");
        combat_ranksFile_   = string("combat_ranks.var");
        combat_stancesFile_ = string("combat_stances.var");
        combat_targetFile_  = string("combat_target.var");
        construction_worksFile_  = string("construction_works.var");
        damage_typeFile_    = string("damage_type.var");
        directionsFile_     = string("directions.var");
        equipmentsFile_     = string("equipments.var");
        movementsFile_      = string("movements.var");
        stancesFile_        = string("stances.var");
//Rules 
        constructionsFile_  = string("constructions.rules");
        enchantmentsFile_   = string("enchantments.rules");
        fx_actionsFile_     = string("fx_actions.rules");
        itemsFile_          = string("items.rules");
        racesFile_          = string("races.rules");
        effectsRuleFile_    = string("effects.rules");
        eventsRuleFile_     = string("event_types.rules");
        seasonsFile_        = string("seasons.rules");
        skillsFile_         = string("skills.rules");
        terrainsFile_       = string("terrains.rules");
        titlesFile_         = string("titles.rules");
        weathersFile_       = string("weathers.rules");
        
 // Events
        eventsFile_ = string(".events");

        gameFile_           = string("game.dat");
        turnString_         = string("0");

        newEntityPrefix_    = string("new");
        daysInMonth = 30;
	runMode = NORMAL;
	randomSeed_ = 0;
        maxCombatRounds = 50;
        isInitialized_ = true;
}

GameConfig::~GameConfig()
{
}

/** initializes Game with data from the file.
Returns false if fails to open file */
bool GameConfig::init(const string filename)
{
	cout <<" Configuration file is " << filename << endl;
    FileParser * parser = new FileParser ( filename );
	if( parser->status != OK)
		{
                    return false;
		}
	else
    filename_ = filename;
  do
    {
      parser->getLine();
	 if (parser->matchKeyword ("NAME") )
    		{
      		name_ = parser->getText();
      		continue;
    		}
  	if (parser->matchKeyword("DESCRIPTION"))
    		{
      		description_ = parser->getText();
      		continue;
    		}
 	if (parser->matchKeyword("GAME"))
    		{
      		game_ = parser->getWord();
      		continue;
    		}
 	if (parser->matchKeyword("TURN"))
    		{
      		turn = parser->getInteger();
      		continue;
    		}
 	if (parser->matchKeyword("BLANKSTART"))
    		{
                isInitialized_ = false;
      		continue;
    		}
 	if (parser->matchKeyword("DEADLINE"))
    		{
      		deadline_ = parser->getText();
      		continue;
    		}

	if (parser->matchKeyword("GAMEDIR"))
    		{
      		 gameDir_= parser->getText();
      		continue;
    		}
 	if (parser->matchKeyword("PLAYERSDIR"))
    		{
      		 playersDir_= parser->getParameter();
      		continue;
    		}
 	if (parser->matchKeyword("EMAIL"))
    		{
      		 serverEMail_= parser->getText();
      		continue;
    		}
 	if (parser->matchKeyword("UNITSFILE"))
    		{
      		 unitsFile_ = parser->getParameter();
      		continue;
    		}
 	if (parser->matchKeyword("LOCATIONSFILE"))
    		{
      		 locationsFile_ = parser->getParameter();
      		continue;
    		}
 	if (parser->matchKeyword("FACTIONSFILE"))
    		{
      		 factionsFile_ = parser->getParameter();
      		continue;
    		}
 	if (parser->matchKeyword("BUILDINGSFILE"))
    		{
      		 buildingsFile_ = parser->getParameter();
      		continue;
    		}
 	if (parser->matchKeyword("EFFECTSFILE"))
    		{
      		 effectsFile_ = parser->getParameter();
      		continue;
    		}
        
        if (parser->matchKeyword("COMBAT_FILES_FILE")) { combat_filesFile_ = parser->getParameter(); continue;}
        if (parser->matchKeyword("COMBAY_MOVES_FILE")) { combat_movesFile_ = parser->getParameter(); continue;}
        if (parser->matchKeyword("COMBAT_RANKS_FILE")) { combat_ranksFile_ = parser->getParameter(); continue;}
        if (parser->matchKeyword("COMBAT_STANCES_FILE")) { combat_stancesFile_ = parser->getParameter(); continue;}
        if (parser->matchKeyword("COMBAT_TARGETFILE")) { combat_targetFile_ = parser->getParameter(); continue;}
        if (parser->matchKeyword("CONSTRUCTION_WORKS_FILE")) { construction_worksFile_ = parser->getParameter(); continue;}
        if (parser->matchKeyword("DAMAGE_TYPE_FILE")) { damage_typeFile_ = parser->getParameter(); continue;}
        if (parser->matchKeyword("DIRECTIONS_FILE")) { directionsFile_ = parser->getParameter(); continue;}
        if (parser->matchKeyword("EQUIPMENTS_FILE")) { equipmentsFile_ = parser->getParameter(); continue;}
        if (parser->matchKeyword("MOVEMENTS_FILE")) { movementsFile_ = parser->getParameter(); continue;}
        if (parser->matchKeyword("STANCES_FILE")) { stancesFile_ = parser->getParameter(); continue;}

        if (parser->matchKeyword("CONSTRUSCTIONS_FILE")) { constructionsFile_ = parser->getParameter(); continue;}
        if (parser->matchKeyword("ENCHANTMENTS_FILE")) { enchantmentsFile_ = parser->getParameter(); continue;}
        if (parser->matchKeyword("FX_ACTIONS_FILE")) { fx_actionsFile_ = parser->getParameter(); continue;}
        if (parser->matchKeyword("ITEMS_FILE")) { itemsFile_ = parser->getParameter(); continue;}
        if (parser->matchKeyword("RACES_FILE")) { racesFile_ = parser->getParameter(); continue;}
        if (parser->matchKeyword("EFFECTS_RULE_FILE")) { effectsRuleFile_ = parser->getParameter(); continue;}
        if (parser->matchKeyword("EVENTS_RULE_FILE")) { eventsRuleFile_ = parser->getParameter(); continue;}
        if (parser->matchKeyword("SEASONS_FILE")) { seasonsFile_ = parser->getParameter(); continue;}
        if (parser->matchKeyword("SKILLS_FILE")) { skillsFile_ = parser->getParameter(); continue;}
        if (parser->matchKeyword("TERRAINS_FILE")) {  terrainsFile_ = parser->getParameter(); continue;}
        if (parser->matchKeyword("TITLES_FILE")) { titlesFile_ = parser->getParameter(); continue;}
        if (parser->matchKeyword("WEATHERS_FILE")) { weathersFile_ = parser->getParameter(); continue;}

        if (parser->matchKeyword("GAMEFILE"))
    		{
      		 gameFile_ = parser->getParameter();
      		continue;
    		}
 	if (parser->matchKeyword("DAYS_IN_TURN"))
    		{
      		daysInMonth = parser->getInteger();
      		continue;
    		}
 	if (parser->matchKeyword("COMBAT_ROUNDS"))
    		{
                maxCombatRounds = parser->getInteger();
      		continue;
    		}
 	if (parser->matchKeyword("NEW_ENTITY_PREFIX"))
    		{
      		newEntityPrefix_ = parser->getWord();
      		continue;
    		}
 	if (parser->matchKeyword("RUN_MODE"))
    		{
      		 runMode = static_cast<RUNMODE> (parser->getInteger());
      		continue;
    		}
 	if (parser->matchKeyword("NPC"))
    		{
      		npcFactions_.push_back(parser->getWord());
      		continue;
    		}
 	if (parser->matchKeyword("PREFIX"))
    		{

      		continue;
    		}

    } while (! parser ->eof() );
 delete parser;

 unitsFile_.insert(0, gameDir_+ "/");
 locationsFile_.insert(0, gameDir_+ "/");
 factionsFile_.insert(0, gameDir_+ "/");
 buildingsFile_.insert(0, gameDir_+ "/");
 effectsFile_.insert(0, gameDir_+ "/");

 combat_filesFile_.insert(0, gameDir_+ "/");
 combat_movesFile_.insert(0, gameDir_+ "/");
 combat_ranksFile_.insert(0, gameDir_+ "/");
 combat_stancesFile_.insert(0, gameDir_+ "/");
 combat_targetFile_.insert(0, gameDir_+ "/");
 construction_worksFile_.insert(0, gameDir_+ "/");
 damage_typeFile_.insert(0, gameDir_+ "/");
 directionsFile_.insert(0, gameDir_+ "/");
 equipmentsFile_.insert(0, gameDir_+ "/");
 movementsFile_.insert(0, gameDir_+ "/");
 stancesFile_.insert(0, gameDir_+ "/");

 constructionsFile_.insert(0, gameDir_+ "/");
 enchantmentsFile_.insert(0, gameDir_+ "/");
 fx_actionsFile_.insert(0, gameDir_+ "/");
 itemsFile_.insert(0, gameDir_+ "/");
 racesFile_.insert(0, gameDir_+ "/");
 effectsRuleFile_.insert(0, gameDir_+ "/");
 eventsRuleFile_.insert(0, gameDir_+ "/");
 seasonsFile_.insert(0, gameDir_+ "/");
 skillsFile_.insert(0, gameDir_+ "/");
 terrainsFile_.insert(0, gameDir_+ "/");
 titlesFile_.insert(0, gameDir_+ "/");
 weathersFile_.insert(0, gameDir_+ "/");
    


 if(game_.empty())
 {
 	gameFile_ = game_ + ".dat";
 }
 gameFile_.insert(0, gameDir_+ "/");

// Now initialize dynamic game data
    if(isInitialized_)
    {
	parser = new FileParser ( gameFile_ );
	if( parser->status != OK)
		{
		cout << "Failed to open file "<<game_<<".dat\t\t Exiting..." << endl;
		exit(1);
		}

  do
    {
      parser->getLine();
 	if (parser->matchKeyword("TURN"))
    		{
      		turn = parser->getInteger();
      		continue;
    		}
 	if (parser->matchKeyword("DEADLINE"))
    		{
      		deadline_ = parser->getText();
      		continue;
    		}
 	if (parser->matchKeyword("SEED"))
    		{
      		randomSeed_ = parser->getLongInteger();
		cerr << "Got random seed " << randomSeed_<<endl;
      		continue;
    		}
    } while (! parser ->eof() );
 delete parser;
 turnString_ = longtostr(turn);
 eventsFile_.insert(0, gameDir_+ "/" + "turn" + turnString_);


// cerr << "random seed is " << randomSeed_<<endl;
 if(randomSeed_ == 0)
 		randomSeed_ = (unsigned long)time( NULL );
 srand( randomSeed_ ); // init random
    }
 return true;

}



STATUS GameConfig::save()
{
  time_t  rawtime;
  cout << "Saving game info  "  <<endl;
  ofstream outfile ((gameFile_ + ".new").c_str());
  outfile << "# Overlord game info " <<endl;
  time ( &rawtime );
  outfile << "#  Version "<<  version << " " <<ctime(&rawtime) <<endl;

  if(!game_.empty()) outfile         << "GAME  " << game_ << endl;
  if(!name_.empty()) outfile         << "NAME  " << name_ << endl;
  if(!description_.empty()) outfile  << "DESCRIPTION  " << description_ << endl;
  /*if(turn != 0) */outfile << "TURN  "  << turn +1 << endl;
	outfile << "#Random seed used: " << randomSeed_ <<endl; // seed is reported
	                                                        //but not saved! 
//  if(!version_.empty()) outfile      << "VERSION  " << version_ << endl;
//  if(!serverEMail_.empty()) outfile  << "EMAIL  " << serverEMail_ << endl;

//  if(daysInMonth) outfile  << "DAYS_IN_TURN  " << daysInMonth << endl;
//  if(runMode != NORMAL) outfile  << "RUN_MODE  " << runMode << endl;

//  if(!unitsFile_.empty()) outfile     << "UNITSFILE  " << unitsFile_ << endl;
// if(!locationsFile_.empty()) outfile << "LOCATIONSFILE  " << locationsFile_ << endl;
// if(!factionsFile_.empty()) outfile  << "FACTIONSFILE  " << factionsFile_ << endl;
//  if(!buildingsFile_.empty()) outfile << "BUILDINGSFILE  " << buildingsFile_ << endl;

  if(!deadline_.empty()) outfile      << "DEADLINE  " << deadline_ << endl;
//  if(!gameDir_.empty()) outfile       << "GAMEDIR  " << gameDir_ << endl;
//  if(!playersDir_.empty()) outfile    << "PLAYERSDIR  " << playersDir_ << endl;
//  if(!newEntityPrefix_.empty()) outfile << "NEW_ENTITY_PREFIX " << newEntityPrefix_ << endl;
//  for (vector <string>::iterator iter = npcFactions_.begin();
//          iter !=npcFactions_.end(); ++iter)
//          {
//            outfile << "NPC " << (*iter)<<endl;
//          }

	 outfile.close();
	return OK;
}
/** Returns relevant order file name  */
string GameConfig::getOrdersFileName(FactionEntity * faction)
{
return (playersDir_ +  "/" + faction -> getTag() +   "/"  +"orders." + turnString_ );
}


string GameConfig::getGameId() const{
return game_;
}
/** No descriptions */
string GameConfig::getReportFileName(FactionEntity * faction)
{
return (playersDir_ +  "/" + faction -> getTag() +   "/"  +"report." + turnString_ );
}
/** No descriptions */
string GameConfig::getCReportFileName(FactionEntity * faction)
{
return (playersDir_ +  "/" + faction -> getTag() +   "/"  +"creport." + turnString_ );
}
/** No descriptions */
string GameConfig::getServer() const{
return serverEMail_;
}
string GameConfig::getServerName() const{
return name_;
}



string * GameConfig::getUnitsFile() {
return &unitsFile_;
}
string * GameConfig::getFactionsFile() {
return &factionsFile_;
}
string * GameConfig::getLocationsFile() {
return &locationsFile_;
}

string * GameConfig::getBuildingsFile()
{
return &buildingsFile_;
}


string * GameConfig::getEffectsFile()
{
return &effectsFile_;
}

string * GameConfig::getGameFile()
{
return &gameFile_;
}


bool GameConfig::isNewEntityName(const string &tag, FactionEntity * faction)
{
  // New Entity name should be [fxxx][Px][X][nnn]
  // fxxx - faction tag
  // Px New Entity prefix
	// X - prefix defining type of Entity (Unit, Construction) - same as collection prefix
  // nnn - number
  //// faction tag may be omited for your's own units. In this case
// cout << "["<<tag<<"] is tested for being new unit placeholder\n";
  string::size_type factionTagSize = tag.find(newEntityPrefix_,0);
  if(factionTagSize ==0)
	{
    return true;
	}
	//
  if(factionTagSize == string::npos)
   {
//     cerr <<  "["<<newEntityPrefix_<<"] not found in tag string\n";
    return false;
    }
     string tempTag = tag.substr(0,factionTagSize);
     faction = dynamic_cast<FactionEntity *>(gameFacade->factions.findByTag(tempTag,false));
     if(gameFacade->factions.isValidTag(tempTag) )
   		 return true;
//    cout << "Faction "<<tag.substr(0,factionTagSize)<< " not found\n";
  return false;
}



bool GameConfig::isNPCFaction(FactionEntity * faction)
{
  for (vector <string>::iterator iter = npcFactions_.begin();
          iter !=npcFactions_.end(); ++iter)
          {
           if ((*iter) == faction->getTag())
            return true;
          }
  return false;
}


// Returns next letter after newEntityPrefix
char GameConfig::getEntityTypePrefix(const string &tag)
{
  string::size_type factionTagSize = tag.find(newEntityPrefix_,0);
	//
  if(factionTagSize == string::npos)
   {
     cerr <<  "["<<newEntityPrefix_<<"] not found in tag string\n";
    return 0;
   }
  if(factionTagSize + newEntityPrefix_.length() > tag.length())
	{
     cerr <<  "["<<newEntityPrefix_<<"] contains no entity identifier\n";
		return 0;
	}
   char prefix = tag[factionTagSize + newEntityPrefix_.length()];

	 if(isalpha(prefix))
	 {
	 	return prefix;
	 }
	 return 0;
}

