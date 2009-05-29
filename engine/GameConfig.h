/***************************************************************************
                          GameConfig.h  -  description
                             -------------------
    begin                : Wed Nov 20 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#ifndef GAMECONFIG_H
#define GAMECONFIG_H
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
enum run_mode {
	NORMAL = 0,
	TIME_TEST = 1,
	MEMORY_TEST =2,
	DAILY_MEMORY_TEST = 3,
	COMBAT_SIMULATOR = 4,
	LOCATION_INITIALIZATION = 5,
	STARTING_TURN = 6,
	LAST_MODE = 99
	};
typedef enum run_mode RUNMODE;

class GameConfig {
public:
	GameConfig();
	~GameConfig();
  void init(const char * filename);
  STATUS save();
  string getOrdersFileName(FactionEntity * faction);
  string getGameId() const;
  /** No descriptions */
  string getReportFileName(FactionEntity * faction);
  /** No descriptions */
  string getServer() const;
//==    inline string getVersion() const{ return version_;}
   string getServerName() const;
  /** No descriptions */

  static string version;
// Entities
  string * getUnitsFile() ;
  string * getFactionsFile() ;
  string * getLocationsFile() ;
  string * getBuildingsFile() ;
  string * getEffectsFile() ;
  string * getGameFile() ;
// Vars
  string * getCombat_filesFile(){return &combat_filesFile_;}  
  string * getCombat_ranksFile()  {return &combat_ranksFile_;}   
  string * getCombat_targetFile()  {return &combat_targetFile_;}      
  string * getDamage_typeFile()  {return &damage_typeFile_;} 
  string * getEquipmentsFile() {return &equipmentsFile_;} 
  string * getStancesFile(){return &stancesFile_;}
  string * getCombat_movesFile(){return &combat_movesFile_;} 
  string * getCombat_stancesFile(){return &combat_stancesFile_;}   
  string * getConstruction_worksFile() {return &construction_worksFile_;} 
  string * getDirectionsFile() {return &directionsFile_;}   
  string * getMovementsFile(){return &movementsFile_;} 
// Rules
  string  * getConstructionsFile(){return &constructionsFile_;} 
  string  * getEnchantmentsFile(){return &enchantmentsFile_;} 
  string  * getFx_actionsFile(){return &fx_actionsFile_;} 
  string  * getItemsFile(){return &itemsFile_;} 
  string  * getRacesFile(){return &racesFile_;} 
  string  * getSeasonsFile(){return &seasonsFile_;} 
  string  * getSkillsFile(){return &skillsFile_;} 
  string  * getTerrainsFile(){return &terrainsFile_;} 
  string  * getTitlesFile(){return &titlesFile_;} 
  string  * getWeathersFile(){return &weathersFile_;} 

  string  getDeadline() {return deadline_;}
        bool isNPCFaction(FactionEntity * faction);
        char getEntityTypePrefix(const string &tag);
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
  string effectsFile_;
  
  string   combat_filesFile_;  
  string   combat_movesFile_;  
  string   combat_ranksFile_;  
  string   combat_stancesFile_;  
  string   combat_targetFile_;  
  string   construction_worksFile_;  
  string   damage_typeFile_;  
  string   directionsFile_;  
  string   equipmentsFile_;  
  string   movementsFile_;  
  string   stancesFile_;
  
  string   constructionsFile_;
  string   enchantmentsFile_;
  string   fx_actionsFile_;
  string   itemsFile_;
  string   racesFile_;
  string   seasonsFile_;
  string   skillsFile_;
  string   terrainsFile_;
  string   titlesFile_;
  string   weathersFile_;
 
  
  string gameFile_;
  vector <string> npcFactions_;
	unsigned long int randomSeed_;
};
extern GameConfig gameConfig;
 
#endif
