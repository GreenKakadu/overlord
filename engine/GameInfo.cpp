/***************************************************************************
                          GameInfo.cpp  -  description
                             -------------------
    begin                : Wed Nov 20 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "stdio.h"
#include <time.h>
#include "GameInfo.h"
#include "FileParser.h"
#include "FactionEntity.h"
#include "EntitiesCollection.h"
extern void longtostr(unsigned long u, char *out);
extern EntitiesCollection <FactionEntity>   factions;

GameInfo::GameInfo()
{
	unitsFile_ = string("units");
	locationsFile_ = string("locations");
	factionsFile_ = string("factions");
  newEntityPrefix_ = string("new");
  daysInMonth = 30;
	runMode = NORMAL;
}

GameInfo::~GameInfo()
{
}

/** initializes Game with data from the file.
Returns false if faileres to open file */
void GameInfo::init(const char * filename)
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
  	if (parser->matchKeyword("VERSION"))
    		{
      		version_ = parser->getText();
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
 char buffer [10];
 longtostr(turn,buffer);
// sprintf(buffer,"%d",turn);
 turnString_ = buffer;
}



STATUS GameInfo::save()
{
  time_t  rawtime;
  cout << "Saving game info  "  <<endl;
  ofstream outfile ((filename_ + ".new").c_str());
  outfile << "# Overlord game info " <<endl;
  time ( &rawtime );
  outfile << "#  Version"<<  version_ << " " <<ctime(&rawtime) <<endl;

  if(!game_.empty()) outfile         << "GAME  " << game_ << endl;
  if(!name_.empty()) outfile         << "NAME  " << name_ << endl;
  if(!description_.empty()) outfile  << "DESCRIPTION  " << description_ << endl;
  if(turn != 0) outfile << "TURN  "  << turn +1 << endl;
  if(!version_.empty()) outfile      << "VERSION  " << version_ << endl;
  if(!serverEMail_.empty()) outfile  << "EMAIL  " << serverEMail_ << endl;

  if(daysInMonth) outfile  << "DAYS_IN_TURN  " << daysInMonth << endl;
  if(runMode != NORMAL) outfile  << "RUN_MODE  " << runMode << endl;
   
  if(!unitsFile_.empty()) outfile     << "UNITSFILE  " << unitsFile_ << endl;
  if(!locationsFile_.empty()) outfile << "LOCATIONSFILE  " << locationsFile_ << endl;
  if(!factionsFile_.empty()) outfile  << "FACTIONSFILE  " << factionsFile_ << endl;
  if(!buildingsFile_.empty()) outfile << "BUILDINGSFILE  " << buildingsFile_ << endl;

  if(!deadline_.empty()) outfile      << "DEADLINE  " << deadline_ << endl;
  if(!gameDir_.empty()) outfile       << "GAMEDIR  " << gameDir_ << endl;
  if(!playersDir_.empty()) outfile    << "PLAYERSDIR  " << playersDir_ << endl;
  if(!newEntityPrefix_.empty()) outfile << "NEW_ENTITY_PREFIX " << newEntityPrefix_ << endl;
  for (vector <string>::iterator iter = npcFactions_.begin(); 
          iter !=npcFactions_.end(); ++iter)
          {
            outfile << "NPC " << (*iter)<<endl;
          }

	 outfile.close();
	return OK;
}
/** Returns relevant order file name  */
string GameInfo::getOrdersFileName(FactionEntity * faction)
{
return (playersDir_ +  "/" + faction -> getTag() +   "/"  +"orders." + turnString_);
}


string GameInfo::getGameId() const{
return game_;
}
/** No descriptions */
string GameInfo::getReportFileName(FactionEntity * faction)
{
return (playersDir_ +  "/" + faction -> getTag() +   "/"  +"report." + turnString_);
}
/** No descriptions */
string GameInfo::getServer() const{
return serverEMail_;
}
string GameInfo::getServerName() const{
return name_;
}


/** No descriptions */
//const char * GameInfo::getUnitsFile() const{
//return unitsFile_.c_str();
//}
//const char * GameInfo::getFactionsFile() const{
//return factionsFile_.c_str();
//}
// const char * GameInfo::getLocationsFile() const{
//return locationsFile_.c_str();
//}
string * GameInfo::getUnitsFile() {
return &unitsFile_;
}
string * GameInfo::getFactionsFile() {
return &factionsFile_;
}
string * GameInfo::getLocationsFile() {
return &locationsFile_;
}

string * GameInfo::getBuildingsFile()
{
return &buildingsFile_;
}


bool GameInfo::isNewEntityName(const string &tag, FactionEntity * faction)
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
//     cout <<  (factions[1])->printName()<<endl;
//  if(factions.isValidTag(tempTag) )
  if(factions.isValidTag(tag.substr(0,factionTagSize)) )
  
    return true;
//    cout << "Faction "<<tag.substr(0,factionTagSize)<< " not found\n";
  return false;
}



bool GameInfo::isNPCFaction(FactionEntity * faction)
{
  for (vector <string>::iterator iter = npcFactions_.begin(); 
          iter !=npcFactions_.end(); ++iter)
          {
           if ((*iter) == faction->getTag())
            return true;
          }
  return false;
}

