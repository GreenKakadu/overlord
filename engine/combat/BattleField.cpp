/***************************************************************************
                      BattleField.cpp
               Physical location of battle
                          ------------------
    begin                : Mon Nov 01 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include <algorithm>
#include <cstdlib>
#include "BattleField.h"
#include "TokenEntity.h"
#include "BattleInstance.h"
#include "CombatReport.h"
#include "CombatEngine.h"
#include "reporting.h"
#include "CombatAttackMessage.h"
#include "CombatLootMessage.h"
#include "TitlesAttribute.h"
#include "TitleRule.h"
#include "LocationEntity.h"

const int BattleField::RANK_SIZE;
const int BattleField::FILE_SIZE;
CombatRankVariety * frontCombatRank;
CombatRankVariety * middleCombatRank;
CombatRankVariety * rearCombatRank;
CombatFileVariety * leftCombatFile;
CombatFileVariety * centerCombatFile;
CombatFileVariety * rightCombatFile;

extern ReportPattern * reclaimTitleReporter;
extern ofstream combatReportFile;
extern int Roll_1Dx(int n);

BattleField::BattleField(CombatEngine * engine)
{
	engine_ = engine;
  freeAttackerTitles_.clear();
  freeDefenderTitles_.clear();
	report_ = engine->getCombatReport();
	maxFile_ = FILE_SIZE;
	minFile_ = 1;
	maxRank_ = RANK_SIZE * 2;
	minRank_ = 1;
	// Initialize battlefield array
	cout << "         Initialize battlefield array"<<endl;
	for(int i=0; i <=maxFile_; ++i)
		for(int j=0; j <=maxRank_; ++j)
		{
			(units[i][j]).clear();

			if((i==0 )|| (j == 0)) // it is not used
			{
				sectorNames[i][j] += "undefined position ";
				continue;
			}
			 // Determine side of field
			if(j * 2 > maxRank_ )
			{
			 sectorNames[i][j] += "defender's " + (combatRanks[RANK_SIZE * 2 -j])->getName() + " ";
			 sectorNames[i][j] += (combatFiles[FILE_SIZE - i])->getName();
			}
			else
			{
			 sectorNames[i][j] +=  "attacker's " + (combatRanks[j-1])->getName() + " ";
			 sectorNames[i][j] += (combatFiles[i-1])->getName();
			}
//		cout << "Size of units["<<i<<"]["<<j<<"]="<<(units[i][j]).size()<<endl;
		}

 }



BattleField::~BattleField()
{
}


void BattleField::placeEntity(TokenEntity * entity, int rank, int file)
{
  if((rank == 0) && (file == 0))
	{
		CombatFileVariety * combatFile = entity->getCombatFile();
		CombatRankVariety * combatRank = entity->getCombatRank();
		if(combatFile == leftCombatFile)
			{
				if(entity->getBattleInstantiation()->isAttacker())
					file = 1;
				else
					file = 3;
			}
		if(combatFile == centerCombatFile)
			{
					file = 2;
			}
		if(combatFile == rightCombatFile)
			{
				if(entity->getBattleInstantiation()->isAttacker())
					file = 3;
				else
					file = 1;
			}
		if(combatRank == frontCombatRank)
			{
				if(entity->getBattleInstantiation()->isAttacker())
					rank = 3;
				else
					rank = 4;
			}
		if(combatRank == middleCombatRank)
			{
				if(entity->getBattleInstantiation()->isAttacker())
					rank = 2;
				else
					rank = 5;
			}
		if(combatRank == rearCombatRank)
			{
				if(entity->getBattleInstantiation()->isAttacker())
					rank = 1;
				else
					rank = 6;
			}
	}
	else
		assert	((file <= maxFile_) && (file >= minFile_)
		        && (rank >=minRank_) && (rank <=maxRank_));

	entity->getBattleInstantiation()->setRank(rank);
	entity->getBattleInstantiation()->setFile(file);
	(units[file][rank]).push_back(entity);
}




void BattleField::removeEntity(TokenEntity * entity)
{
 int rank = entity->getBattleInstantiation()->getRank();
 int file = entity->getBattleInstantiation()->getFile();
//LocalInstantiations * localUnits =  &(units[file][rank]);
 vector<TokenEntity *>::iterator iter;
 iter = find((units[file][rank]).begin(),
  		        (units[file][rank]).end(), entity);
	if(iter != (units[file][rank]).end())
 		(units[file][rank]).erase(iter);
	else
		cerr << "Error on attempt to remove from the battlefield ("<<file
		<<","<<rank<<") not existing unit " <<entity<<endl;
}




bool BattleField::moveEntity(TokenEntity * entity, BATTLE_DIRECTION battleDirection)
{
	bool isMovementPossible;
	int oldFile, oldRank;
	int newFile, newRank;
	oldFile = entity->getBattleInstantiation()->getFile();
	oldRank = entity->getBattleInstantiation()->getRank();
	isMovementPossible = calculateMovementCoordinates(entity,battleDirection);
	if(isMovementPossible)
	{
	 vector<TokenEntity *>::iterator iter = find((units[oldFile][oldRank]).begin(),
  		        (units[oldFile][oldRank]).end(), entity);
 	(units[oldFile][oldRank]).erase(iter);

	newFile = entity->getBattleInstantiation()->getFile();
	newRank = entity->getBattleInstantiation()->getRank();

	(units[newFile][newRank]).push_back(entity);
	}
	return isMovementPossible;
}



bool BattleField::calculateMovementCoordinates(TokenEntity * entity, BATTLE_DIRECTION battleDirection)
{
 int position = 0;

 bool isAttacker = entity->getBattleInstantiation()->isAttacker();
	switch(battleDirection)
	{
		case FORWARD:
			{
				position =entity->getBattleInstantiation()->getRank();
				if(isAttacker)
				{
					if(position < maxRank_)
						{
							entity->getBattleInstantiation()->setRank(position + 1);
							return true;
						}
							return false;
				}
				else
				{
					if(position > minRank_)
						{
							entity->getBattleInstantiation()->setRank(position - 1);
							return true;
						}
							return false;
				}
			}
		case BACKWARD:
			{
				position = entity->getBattleInstantiation()->getRank();
				if(isAttacker)
				{
					if(position > minRank_)
						{
							entity->getBattleInstantiation()->setRank(position - 1);
							return true;
						}
							return false;
				}
				else
				{
					if(position < maxRank_)
						{
							entity->getBattleInstantiation()->setRank(position + 1);
							return true;
						}
							return false;
				}
			}
		case LEFT:
			{
				position =entity->getBattleInstantiation()->getFile();
				if(isAttacker)
				{
					if(position > minFile_)
						{
							entity->getBattleInstantiation()->setFile(position - 1);
							return true;
						}
							return false;
				}
				else
				{
					if(position < maxFile_)
						{
							entity->getBattleInstantiation()->setFile(position + 1);
							return true;
						}
							return false;
				}
			}
		case RIGHT:
			{
				position =entity->getBattleInstantiation()->getFile();
				if(isAttacker)
				{
					if(position < maxFile_)
						{
							entity->getBattleInstantiation()->setFile(position + 1);
							return true;
						}
							return false;
				}
				else
				{
					if(position > minFile_)
						{
							entity->getBattleInstantiation()->setFile(position - 1);
							return true;
						}
							return false;
				}
			}
    case SELF: // Do nothing
		{
		}
	}
	return false;
}



void BattleField::shuffleOnce(BattleInstance * instance)
{
	int rank = instance->getRank();
	int file = instance->getFile();
  if (Roll_1Dx(2) == 1) //shuffle file or rank? (1 - rank, 0 - file)
	{
			int minRank;
			int maxRank;
		// ranks are different for attacker and defender
		if(instance->isAttacker())
		{
			minRank = minRank_;
			maxRank = maxRank_/2;
		}
		else
		{
			minRank = minRank_ + maxRank_/2;
			maxRank = maxRank_;
		}
		if(rank == minRank) // or move or remain at place
		{
			if (Roll_1Dx(2) == 1)
			{
        shiftEntity(instance,instance->getFile(),minRank + 1);
				return;
			}
			return;
		}
		if(rank == maxRank)
		{
			if (Roll_1Dx(2) == 1)
			{
        shiftEntity(instance,instance->getFile(),maxRank - 1);
				return;
			}
			return;
		}
		int shift = Roll_1Dx(3) - 1; //
    shiftEntity(instance,instance->getFile(),instance->getRank()+ shift);
	}
	else // Shuffle File
	{
		if(file == minFile_) // or move or remain at place
		{
			if (Roll_1Dx(2) == 1)
			{
        shiftEntity(instance,minFile_ + 1,instance->getRank());
				return;
			}
			return;
		}
		if(rank == maxFile_)
		{
			if (Roll_1Dx(2) == 1)
			{
        shiftEntity(instance,maxFile_ - 1,instance->getRank());
				return;
			}
			return;
		}
		int shift = Roll_1Dx(3) - 1; //
    shiftEntity(instance,instance->getFile() + shift,instance->getRank());
	}

}



void BattleField::shiftEntity(BattleInstance *instance, int newFile, int newRank)
{
	int oldFile = instance->getFile();
	int oldRank = instance->getRank();
	TokenEntity *entity = instance->getOrigin();

	 vector<TokenEntity *>::iterator iter = find((units[oldFile][oldRank]).begin(),
  		        (units[oldFile][oldRank]).end(), entity);

	(units[oldFile][oldRank]).erase(iter);
	(units[newFile][newRank]).push_back(entity);
	instance->setFile(newFile);
	instance->setRank(newRank);
}



bool BattleField::haveEnemies(BattleInstance * battleInstance, BATTLE_DIRECTION battleDirection, int range)
{
	int rank = getRelativeRank(battleInstance,battleDirection, range);
	int file = getRelativeFile(battleInstance,battleDirection, range);

//	vector<TokenEntity *>::iterator iter;
//	cout<< battleInstance->print()<< " is looking for Enemies at "<< file<<","<<rank <<": (size= "<<(units[file][rank]).size()<< ") ";
 	if((file == 0) || (rank == 0))
		return false;

  if((units[file][rank]).empty())
		return false;
//	cout<< " [-> ";
//	for(iter = (units[file][rank]).begin(); iter != (units[file][rank]).begin(); ++iter)
//	{
//		if(*iter)
//		cout<<"   "<<(*iter)<<endl;
//		else
//		cout<< " [] ";
//	}
//		cout<<"  <-] "<<endl;

	if( (*((units[file][rank]).begin()))->getBattleInstantiation()->isAttacker() ==
	 battleInstance->isAttacker())
		{
		return false;
		}
		return true;
}



bool BattleField::haveEnemiesRelative(BattleInstance * battleInstance,
			int deltaRank, int deltaFile)
{
	int rank = battleInstance->getRank();
	int file = battleInstance->getFile();

	if(battleInstance->isAttacker())
	{
		rank += deltaRank;
		file += deltaFile;
	}

	else
	{
		rank -= deltaRank;
		file -= deltaFile;
	}

//	vector<TokenEntity *>::iterator iter;
//	cout<< battleInstance->print()<< " is looking for Enemies at "<< file<<","<<rank <<": (size= "<<(units[file][rank]).size()<< ") ";

	if((file < minFile_) || (rank < minRank_))
		return false;
 	if((file > maxFile_) || (rank > maxRank_))
		return false;

  if((units[file][rank]).empty())
		return false;
//	cout<< " [-> ";
//	for(iter = (units[file][rank]).begin(); iter != (units[file][rank]).begin(); ++iter)
//	{
//		if(*iter)
//		cout<<"   "<<(*iter)<<endl;
//		else
//		cout<< " [] ";
//	}
//		cout<<"  <-] "<<endl;

	if( (*((units[file][rank]).begin()))->getBattleInstantiation()->isAttacker() ==
	 battleInstance->isAttacker())
		{
		return false;
		}
		return true;
}


// is this position inside the battlefield or not?
bool BattleField::isBattlefieldRelative(BattleInstance * battleInstance,
			int deltaRank, int deltaFile)
{
	int rank = battleInstance->getRank();
	int file = battleInstance->getFile();

	if(battleInstance->isAttacker())
	{
		rank += deltaRank;
		file += deltaFile;
	}

	else
	{
		rank -= deltaRank;
		file -= deltaFile;
	}

	if(file < minFile_)
		return false;

	if(file > maxFile_)
		return false;

	if(rank < minRank_)
		return false;

	if(rank > maxRank_)
		return false;

	return true;
}



// Calculate Rank coordinate of battlefield cell in a given direction from unit
int BattleField::getRelativeRank(BattleInstance * battleInstance,
															BATTLE_DIRECTION battleDirection, int range)
{
 bool isAttacker = battleInstance->isAttacker();
 int position = battleInstance->getRank();
	switch(battleDirection)
	{
    case SELF:
		case LEFT:
			{
				return position;
			}
		case RIGHT:
			{
				return position;
			}
		case FORWARD:
			{
				if(isAttacker)
				{
					if(position + range <= maxRank_)
						{
							return (position + range);
						}
					else
					{
//				   cerr<< "Rank error "<< position <<" for "<<
//					                       battleInstance->print()<<endl;
						return 0;
					}
				}
				else
				{
					if(position - range >= minRank_)
						{
							return (position - range);
						}
					else
					{
//				   cerr<< "Rank error "<< position <<" for "<<
//					                       battleInstance->print()<<endl;
						return 0;
					}
				}
			}
		case BACKWARD:
			{
				if(isAttacker)
				{
					if(position  - range >= minRank_)
						{
							return (position - range);
						}
					else
					{
//				   cerr<< "Rank error "<< position <<" for "<<
//					                       battleInstance->print()<<endl;
						return 0;
					}

				}
				else
				{
					if(position + range <= maxRank_)
						{
							return(position + range);
						}

					else
					{
//				   cerr<< "Rank error "<< position <<" for "<<
//					                       battleInstance->print()<<endl;
						return 0;
					}
				}
			}
	}
	cerr<< "Direction error "<< battleDirection <<" for "<<					                       battleInstance->print()<<endl;
	return 0;
}




// Calculate File coordinate of battlefield cell in given direction from unit
int BattleField::getRelativeFile(BattleInstance * battleInstance,
															BATTLE_DIRECTION battleDirection, int range)
{
 bool isAttacker = battleInstance->isAttacker();
 int position = battleInstance->getFile();
	switch(battleDirection)
	{
    case SELF:
		case FORWARD:
			{
				return position;
			}
		case BACKWARD:
			{
				return position;
			}
		case LEFT:
			{
				if(isAttacker)
				{
					if(position  - range >= minFile_)
						{
							return (position - range);
						}
					else
					{
//				   cerr<< "File error "<< position <<" for "<<
//					                       battleInstance->print()<<endl;
						return 0;
					}

				}
				else
				{
					if(position  + range <= maxFile_)
						{
							return(position + range);
						}
					else
					{
//				   cerr<< "File error "<< position <<" for "<<
//					                       battleInstance->print()<<endl;
						return 0;
					}

				}
			}
		case RIGHT:
			{
				if(isAttacker)
				{
					if(position + range <= maxFile_)
						{
							return (position + range);
						}
					else
					{
//				   cerr<< "File error "<< position <<" for "<<
//					                       battleInstance->print()<<endl;
						return 0;
					}

				}
				else
				{
					if(position  - range >= minFile_)
						{
							return(position - range);
						}
					else
					{
//				   cerr<< "File error "<< position <<" for "<<
//					                       battleInstance->print()<<endl;
						return 0;
					}
				}
			}
	}
	cerr<< "Direction error "<< battleDirection <<" for "<<					                       battleInstance->print()<<endl;
	return 0;
}


// Add to target list all enemies at given direction (range 1)
void BattleField::addEnemies(BattleInstance * battleInstance,
									BattleTargets & potentialTargets,
									BATTLE_DIRECTION battleDirection)
{
	int rank = getRelativeRank(battleInstance,battleDirection);
	int file = getRelativeFile(battleInstance,battleDirection);
  	if((file < minFile_) || (rank <minRank_))
		return;

 	if((file > maxFile_) || (rank > maxRank_))
		return;

  if((units[file][rank]).empty())
		return;

	if( (*((units[file][rank]).begin()))->getBattleInstantiation()->isAttacker() ==
	 battleInstance->isAttacker())
		{
		return;
		}

	for(vector <TokenEntity *>::iterator iter = (units[file][rank]).begin();
			iter != (units[file][rank]).end(); ++iter)
			{
				if((*iter)->getBattleInstantiation()->isAttacker() == battleInstance->isAttacker())
					continue;
				potentialTargets.push_back(
					BattleTargetElement((*iter)->getBattleInstantiation(),
					 battleDirection, 1));
			}
}



// Add to target list all friends at given direction (range 1)
void BattleField::addFriends(BattleInstance * battleInstance,
									BattleTargets & potentialTargets,
									BATTLE_DIRECTION battleDirection)
{
	int rank = getRelativeRank(battleInstance,battleDirection);
	int file = getRelativeFile(battleInstance,battleDirection);
  	if((file < minFile_) || (rank <minRank_))
		return;

 	if((file > maxFile_) || (rank > maxRank_))
		return;

  if((units[file][rank]).empty())
		return;

	if( (*((units[file][rank]).begin()))->getBattleInstantiation()->isAttacker() !=
	 battleInstance->isAttacker())
		{
		return;
		}

	for(vector <TokenEntity *>::iterator iter = (units[file][rank]).begin();
			iter != (units[file][rank]).end(); ++iter)
			{
				if((*iter)->getBattleInstantiation()->isAttacker() != battleInstance->isAttacker())
					continue;
				potentialTargets.push_back(
					BattleTargetElement((*iter)->getBattleInstantiation(),
					 battleDirection, 1));
			}
}


BATTLE_DIRECTION BattleField::getDirection(BattleInstance * battleInstance,
		int rank, int file)
{
	BATTLE_DIRECTION battleDirection;
  bool isAttacker = battleInstance->isAttacker();
	int rankDelta = battleInstance->getRank() - rank;
	int fileDelta = battleInstance->getFile() - file;

	bool side =  abs(fileDelta) > abs(rankDelta);
	if(isAttacker)
	{
		if(side)
			{
				if(fileDelta <= 0)
					battleDirection = LEFT;
				else
					battleDirection = RIGHT;
			}
		else
		{
				if(rankDelta <= 0)
					battleDirection = FORWARD;
				else
					battleDirection = BACKWARD;
		}
	}
	else
	{
		if(side)
			{
				if(fileDelta <= 0)
					battleDirection = RIGHT;
				else
					battleDirection = LEFT;
			}
		else
		{
				if(rankDelta <= 0)
					battleDirection = BACKWARD;
				else
					battleDirection = FORWARD;

		}
	}
 return battleDirection;
}



// Add to target list all enemies at given position
void BattleField::addEnemies(BattleInstance * battleInstance,
									BattleTargets & potentialTargets,
									int rank, int file)
{


// 	if((file < minFile_) || (rank < minRank_))
//		return;

// 	if((file > maxFile_) || (rank > maxRank_))
//		return;

  if((units[file][rank]).empty())
		return;

	if( (*((units[file][rank]).begin()))->getBattleInstantiation()->isAttacker() ==
	 battleInstance->isAttacker())
		{
		return;
		}

	BATTLE_DIRECTION battleDirection = getDirection( battleInstance,
		rank, file);

		int range = abs(battleInstance->getRank() - rank) +
		 abs(battleInstance->getFile() - file);



	for(vector <TokenEntity *>::iterator iter = (units[file][rank]).begin();
			iter != (units[file][rank]).end(); ++iter)
			{
				if((*iter)->getBattleInstantiation()->isAttacker() == battleInstance->isAttacker())
					continue;
				potentialTargets.push_back(
					BattleTargetElement((*iter)->getBattleInstantiation(),
					 battleDirection,range));
			}
}


// Add to target list all friends at given position
void BattleField::addFriends(BattleInstance * battleInstance,
									BattleTargets & potentialTargets,
									int rank, int file)
{


// 	if((file < minFile_) || (rank < minRank_))
//		return;

// 	if((file > maxFile_) || (rank > maxRank_))
//		return;

  if((units[file][rank]).empty())
		return;

	if( (*((units[file][rank]).begin()))->getBattleInstantiation()->isAttacker() !=
	 battleInstance->isAttacker())
		{
		return;
		}

	BATTLE_DIRECTION battleDirection = getDirection( battleInstance,
		rank, file);

		int range = abs(battleInstance->getRank() - rank) +
		 abs(battleInstance->getFile() - file);



	for(vector <TokenEntity *>::iterator iter = (units[file][rank]).begin();
			iter != (units[file][rank]).end(); ++iter)
			{
				if((*iter)->getBattleInstantiation()->isAttacker() != battleInstance->isAttacker())
					continue;
				potentialTargets.push_back(
					BattleTargetElement((*iter)->getBattleInstantiation(),
					 battleDirection,range));
			}
}




void BattleField::addAllEnemiesAtRange(BattleInstance * battleInstance,
									BattleTargets & potentialTargets,
									int range)
{
	int rank0 = battleInstance->getRank();
	int file0 = battleInstance->getFile();
	int currentRange = 0;
	for(int file= minFile_; file <=maxFile_; ++file)
		for(int rank= minRank_; rank <=maxRank_; ++rank)
		{
			 currentRange = abs(rank0 - rank) + abs(file0 - file);
			 if(currentRange <= range)
			 	addEnemies(battleInstance, potentialTargets, rank,  file);

		}
}



void BattleField::addAllFriendsAtRange(BattleInstance * battleInstance,
									BattleTargets & potentialTargets,
									int range)
{
	int rank0 = battleInstance->getRank();
	int file0 = battleInstance->getFile();
	int currentRange = 0;
	for(int file= minFile_; file <=maxFile_; ++file)
		for(int rank= minRank_; rank <=maxRank_; ++rank)
		{
			 currentRange = abs(rank0 - rank) + abs(file0 - file);
			 if(currentRange <= range)
			 	addFriends(battleInstance, potentialTargets, rank,  file);

		}
}



void BattleField::addLoot(ItemRule * item, int number)
{
  for (ItemElementIterator iter  = loot_.begin(); iter != loot_.end(); ++iter)
    {
        if((*iter).getItemType() == item)
                {
                 (*iter).setItemNumber((*iter).getItemNumber()+ number);
                      return;
                }
    }
  loot_.push_back(ItemElement(item,number));
}



void BattleField::addAttackerTitles(TitlesAttribute * titles)
{
  if(titles == 0)
    return;
    for(TitleIterator iter = titles->getAll()->begin();
	iter != titles->getAll()->end(); ++iter)
  {
  	freeAttackerTitles_.push_back(*iter);
  }
}


void BattleField::addDefenderTitles(TitlesAttribute * titles)
{
  if(titles == 0)
    return;
  for(TitleIterator iter = titles->getAll()->begin();
	iter != titles->getAll()->end(); ++iter)
  {
  	freeDefenderTitles_.push_back(*iter);
  }
}



string BattleField::printPosition(int file, int rank)
{
	return sectorNames[file][rank];
}


// returns part of loot available for actual looting
vector <ItemElement>  BattleField::getCurrentLoot()
{
	vector <ItemElement> currentLoot;
	int num;

//  combatReportFile << "Loot: ";
  for (ItemElementIterator iter  = loot_.begin(); iter != loot_.end(); )
    {
  		combatReportFile << (*iter).print() <<endl;
//			if((*iter) == mana)
//			{
//				loot_.erase(iter);
//				continue;
//			}

			num = (*iter).getItemNumber();
			if((*iter).getItemType()->isUnique()) // unique items are never lost
				{
        currentLoot.push_back(ItemElement((*iter).getItemType() , num));
				loot_.erase(iter);
				continue;
				}

			if(num == 1)
			{
        currentLoot.push_back(ItemElement((*iter).getItemType() , 1));
				loot_.erase(iter);
				continue;
			}
			else
			{
				(*iter).setItemNumber(num - (num+1)/2);
      	currentLoot.push_back(ItemElement((*iter).getItemType() ,(num+1) /2));
				++iter;
			}
		}
  			combatReportFile <<endl;
 return currentLoot;
}



// Loot items by set of units
void BattleField::distributeLoot(vector <ItemElement> items, vector <TokenEntity *> units)
{
	for(TokenIterator iter = units.begin(); iter != units.end();)
	{
		if((*iter)->isDisbanded())
		{
			units.erase(iter);
			continue;
		}

		MovementVariety * mode = (*iter)->getReservedMode();//-- determine -- mode

    // overloaded units trying to pick something adding capacity
		if((*iter)->getCapacity(mode) <=  (*iter)->getWeight())// overloaded(mode)
		{
			if((*iter)->takeTransport(items, mode))// try to find appropriate transport
			{
				++iter;
				continue;
			}
			else
			{
				units.erase(iter);
				continue;
			}
		}
		++iter;
	}

	if(units.empty())
		return;




	while(!(items.empty()))// while loot available all units try to take it
	{
		bool gotSomething = false;
	// temporary units just take loot
		for(TokenIterator iter = units.begin(); iter != units.end();++iter)
		{
			gotSomething |= (*iter)->takeLoot(items);
		}

		if(!gotSomething)
			break;
	} // end of while
}


void BattleField::reportLoot()
{
	 new CombatLootMessage(loot_) >> *report_;
}


void BattleField::reclaimTitles(vector <TitleElement *> titles, vector <TokenEntity *> units, int chance)
{
	UnitEntity * unit = 0;
	for(TitleIterator iter = titles.begin(); iter != titles.end(); ++iter)
	{
			if(Roll_1Dx(100) < chance)
			{
				continue;
			}
		// select all units able to reclaim this title
		TitleElement * title = *iter;
		vector <UnitEntity *> potentialTitleHolders;
		for(TokenIterator unitIter = units.begin(); unitIter != units.end();
		 ++unitIter)
		 {
				//UnitEntity *
				 unit = dynamic_cast<UnitEntity *>(*unitIter);
				if(!unit) // Not unit - can't hold titles
					continue;
				if(title->getClaimingCondition()->isSatisfied(unit))
					potentialTitleHolders.push_back(unit);
		 }
			int num = potentialTitleHolders.size();
			int index;
		 if(num == 0)
		 	continue;
		 if(num == 1)
		 {
		 	index = 0;
		 	continue;
		 }
		// roll random
     index = Roll_1Dx(num);
		 unit = potentialTitleHolders[index];
     unit->claimTitle(title);
     unit->addReport(
		 	new TertiaryMessage(reclaimTitleReporter,unit ,
			 title->getTitle(),title->getTitleLocation()));

	}
}


// try to retreat backward
