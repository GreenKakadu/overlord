/***************************************************************************
                      BattleField.h
               Physical location of battle
                          ------------------
    begin                : Mon Nov 01 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef BATTLE_UNIT_H
#define BATTLE_UNIT_H
#include <vector>
#include "CombatRankVariety.h"
#include "CombatFileVariety.h"
#include "ItemElement.h"
#include "MeleeAttackElement.h"
using namespace std;
class TokenEntity;
class BattleInstance;
class CombatEngine;
class CombatReport;
class TitlesAttribute;
class TitleElement;

enum  battle_direction{
  FORWARD      = 0,
  BACKWARD     = 1,
  LEFT         = 2,
  RIGHT        = 3,
	SELF         = 4
};
typedef enum battle_direction BATTLE_DIRECTION;
typedef vector<TokenEntity *> LocalInstantiations;

class BattleTargetElement
{
public:
BattleTargetElement(BattleInstance * instance, BATTLE_DIRECTION battleDirection,
										 int range)
	{instance_ = instance; battleDirection_ = battleDirection; range_ = range;}
~BattleTargetElement(){}
BattleInstance * instance_;
BATTLE_DIRECTION battleDirection_;
int range_;
};
typedef vector<BattleTargetElement> BattleTargets;

class BattleField
{
public:
  BattleField(CombatEngine * engine);
  ~BattleField();
	inline  CombatReport * getCombatReport()const {return report_;}
	inline  CombatEngine * getCombatEngine()const {return engine_;}
	void placeEntity(TokenEntity * entity, int rank = 0, int file = 0);
	bool moveEntity(TokenEntity * entity, BATTLE_DIRECTION battleDirection);
	bool calculateMovementCoordinates(TokenEntity * entity,
									BATTLE_DIRECTION battleDirection);
	void removeEntity(TokenEntity * entity);
  int getRelativeRank(BattleInstance * battleInstance,
															BATTLE_DIRECTION battleDirection, int range = 1);
  int getRelativeFile(BattleInstance * battleInstance,
															BATTLE_DIRECTION battleDirection, int range = 1);
  bool haveEnemies(BattleInstance * battleInstance,
															BATTLE_DIRECTION battleDirection, int range = 1);
bool haveEnemiesRelative(BattleInstance * battleInstance,
			int deltaRank, int deltaFile);
bool isBattlefieldRelative(BattleInstance * battleInstance,
			int deltaRank, int deltaFile);
  void addEnemies(BattleInstance * battleInstance,
									BattleTargets & potentialTargets,
									BATTLE_DIRECTION battleDirection);

  void addEnemies(BattleInstance * battleInstance,
									BattleTargets & potentialTargets,
									int rank, int file);
  void addAllEnemiesAtRange(BattleInstance * battleInstance,
									BattleTargets & potentialTargets,
									int range);

  void addFriends(BattleInstance * battleInstance,
									BattleTargets & potentialTargets,
									BATTLE_DIRECTION battleDirection);

  void addFriends(BattleInstance * battleInstance,
									BattleTargets & potentialTargets,
									int rank, int file);
  void addAllFriendsAtRange(BattleInstance * battleInstance,
									BattleTargets & potentialTargets,
									int range);

//  BATTLE_DIRECTION sweepDirection(BattleInstance * battleInstance);
	BATTLE_DIRECTION getDirection(BattleInstance * battleInstance,
		int rank, int file);

	static const int RANK_SIZE =3;
	static const int FILE_SIZE = 3;

  void addLoot(ItemRule * item, int number);
	void addAttackerTitles(TitlesAttribute * titles);
	void addDefenderTitles(TitlesAttribute * titles);
  vector <ItemElement> & getAllLoot(){return loot_;}
	vector <ItemElement>  getCurrentLoot();
  vector <TitleElement *> & getAllFreeAttackerTitles(){return freeAttackerTitles_;}
  vector <TitleElement *> & getAllFreeDefenderTitles(){return freeDefenderTitles_;}
	inline string getName()const {return name_;}
	inline void setName(string name) {name_ = name;}
	string printPosition(int file, int rank);
	void shuffleOnce(BattleInstance * instance);
  void shiftEntity(BattleInstance *instance, int newFile, int newRank);
	void takeLoot(vector <ItemElement> items, vector <TokenEntity *> units);
	void distributeLoot(vector <ItemElement> items, vector <TokenEntity *> units);
	void reclaimTitles(vector <TitleElement *> titles, vector <TokenEntity *> units, int chance);
	void reportLoot();
	inline int getMinFile() const {return minFile_;}
	inline int getMaxFile() const {return maxFile_;}
	inline int getMinRank() const {return minRank_;}
	inline int getMaxRank() const {return maxRank_;}
  inline LocalInstantiations & getUnits(int file, int rank)
	 {return (units[file][rank]);}

	  protected:
		string name_;
		 int maxFile_;
		 int minFile_;
		 int maxRank_;
		 int minRank_;
  LocalInstantiations units[FILE_SIZE + 1][RANK_SIZE * 2 + 1];
  string sectorNames[FILE_SIZE + 1][RANK_SIZE * 2 + 1];
  vector <ItemElement>   loot_;
  vector <TitleElement *>   freeAttackerTitles_;
  vector <TitleElement *>   freeDefenderTitles_;
	CombatReport * report_;
	CombatEngine * engine_;
  private:
};
extern CombatRankVariety * frontCombatRank;
extern CombatRankVariety * middleCombatRank;
extern CombatRankVariety * rearCombatRank;
extern CombatFileVariety * leftCombatFile;
extern CombatFileVariety * centerCombatFile;
extern CombatFileVariety * rightCombatFile;


#endif

