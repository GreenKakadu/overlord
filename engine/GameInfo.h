/***************************************************************************
                          GameInfo.h  -  description
                             -------------------
    begin                : Wed Nov 20 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef GAMEINFO_H
#define GAMEINFO_H
#include "OverlordTypes.h"
#include "SystemTypes.h"
#include <string>
#include <vector>
using namespace std;
class FactionEntity;
/**This objects keeps both static and dynamic information about current game.
Static information includes such configuration information 
like directories, file names, server e-mail, special symbol definitions etc.
Dynamic information includes turn number, deadline date,  total number of all 
Entities. It should be saved after each turn and read before.
  *@author Alex Dribin
  */
enum run_mode {NORMAL = 0, TIME_TEST = 1, MEMORY_TEST =2, DAILY_MEMORY_TEST = 3};
typedef enum run_mode RUNMODE;

class GameInfo {
public: 
	GameInfo();
	~GameInfo();
  void init(const char * filename);
  STATUS save();
  string getOrdersFileName(FactionEntity * faction);
  string getGameId() const;
  /** No descriptions */
  string getReportFileName(FactionEntity * faction);
  /** No descriptions */
  string getServer() const;
  inline string getVersion() const{ return version_;}
   string getServerName() const;
  /** No descriptions */
//  const char * getUnitsFile() const;
//  const char * getFactionsFile() const;
//  const char * getLocationsFile() const;
  string * getUnitsFile() ;
  string * getFactionsFile() ;
  string * getLocationsFile() ;
  string * getBuildingsFile() ;
  bool isNPCFaction(FactionEntity * faction);
  
  inline string getNewEntityPrefix() const {return newEntityPrefix_;}
  inline int getNewEntityPrefixSize() const {return newEntityPrefix_.size();}
         bool isNewEntityName(const string &tag, FactionEntity * faction = 0);
   UINT turn;
   int daysInMonth;
  RUNMODE runMode;
  protected:
  private:
  string description_; // Game description
  string filename_;
  string  name_; // Game extended name
  string  game_; // Game id
  string deadline_;
  string version_;
  string serverEMail_;
  string gameDir_;
  string playersDir_;
  string turnString_;
  string newEntityPrefix_;
//  string newConstructionPrefix_;
// Entities:
	string unitsFile_;
	string locationsFile_;
	string factionsFile_;
  string buildingsFile_;
  vector <string> npcFactions_;
};
extern GameInfo game;

#endif
