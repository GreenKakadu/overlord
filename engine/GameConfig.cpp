/***************************************************************************
                          GameConfig.cpp  -  description
                             -------------------
    begin                : Wed Nov 20 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "stdio.h"
#include <time.h>
#include "GameConfig.h"
#include "FileParser.h"
#include "FactionEntity.h"
#include "EntitiesCollection.h"
extern string longtostr(long u);
extern EntitiesCollection <FactionEntity>   factions;

GameConfig::GameConfig()
{
	unitsFile_ = string("units.dat");
	locationsFile_ = string("locations.dat");
	factionsFile_ = string("factions.dat");
   buildingsFile_ = string("buildings.dat");
  effectsFile_ = string("effects.dat");
  newEntityPrefix_ = string("new");
  daysInMonth = 30;
	runMode = NORMAL;
	randomSeed_ = 0;
}

GameConfig::~GameConfig()
{
}

/** initializes Game with data from the file.
Returns false if faileres to open file */
void GameConfig::init(const char * filename)
{
	cout <<" Game file is " << filename << endl;
    FileParser * parser = new FileParser ( filename );
	if( parser->status != OK)
		{
		cout << "Exiting..." << endl;
		exit(1);
		}
	else
    filename_ =string( filename);
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
  	if (parser->matchKeyword("FILE"))
    		{
      		 factionsFile_ = parser->getParameter();
      		continue;
    		}
 	if (parser->matchKeyword("DAYS_IN_TURN"))
    		{
      		daysInMonth = parser->getInteger();
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

    } while (! parser ->eof() );
 delete parser;

 unitsFile_.insert(0, gameDir_+ "/");
 locationsFile_.insert(0, gameDir_+ "/");
 factionsFile_.insert(0, gameDir_+ "/");
 buildingsFile_.insert(0, gameDir_+ "/");
 effectsFile_.insert(0, gameDir_+ "/");
// Now initialize dynamic game data

	parser = new FileParser ( game_ + ".dat" );
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
//					cerr << "Got random seed " << randomSeed_<<endl;
      		continue;
    		}
    } while (! parser ->eof() );
 delete parser;
 turnString_ = longtostr(turn);

// cerr << "random seed is " << randomSeed_<<endl;
 if(randomSeed_ == 0)
 		randomSeed_ = time( NULL );
 srand( randomSeed_ ); // init random

}



STATUS GameConfig::save()
{
  time_t  rawtime;
  cout << "Saving game info  "  <<endl;
  ofstream outfile ((game_ + ".new").c_str());
  outfile << "# Overlord game info " <<endl;
  time ( &rawtime );
  outfile << "#  Version "<<  version << " " <<ctime(&rawtime) <<endl;

  if(!game_.empty()) outfile         << "GAME  " << game_ << endl;
  if(!name_.empty()) outfile         << "NAME  " << name_ << endl;
  if(!description_.empty()) outfile  << "DESCRIPTION  " << description_ << endl;
  if(turn != 0) outfile << "TURN  "  << turn +1 << endl;
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


bool GameConfig::isNewEntityName(const string &tag, FactionEntity * faction)
{
  // New Entity name should be [fxxx][Px][nnn]
  // fxxx - faction tag
  // Px New Unit prefix
  // nnn - number
  //// faction tag may be omited for your's own units. In this case
 // cout << "["<<tag<<"] is tested for being new unit placeholder\n";
  string::size_type factionTagSize = tag.find(newEntityPrefix_,0); //
  if(factionTagSize == string::npos)
   {
//     cout <<  "["<<newEntityPrefix_<<"] not found in tag string\n";
    return false;
    }
//     string tempTag = tag.substr(0,factionTagSize);
//     cout << "TempTag= "<<tempTag<<endl;
//     cout <<  (factions[1])->print()<<endl;
//  if(factions.isValidTag(tempTag) )
  if(factions.isValidTag(tag.substr(0,factionTagSize)) )

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

