/***************************************************************************
                          CombatActionStrategy.cpp
                             -------------------
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "CombatActionStrategy.h"
#include "CombatTargetVariety.h"
#include "TokenEntity.h"
#include "ActionRule.h"
#include "RulesCollection.h"
#include "BattleInstance.h"
#include "reporting.h"

CombatActionStrategy      sampleCombatAction("COMBAT_ACTION",&sampleGameData);
ReportPattern * noTargetsReporter= new ReportPattern("noTargetsReporter");

GameData * CombatActionStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<CombatActionStrategy> (this);
}



CombatActionStrategy::CombatActionStrategy ( const string & keyword, GameData * parent): Strategy(keyword, parent)
{
	range_ = 0;
	action_ = 0;
	target_ = 0;
}


			STATUS
CombatActionStrategy::initialize        ( Parser *parser )
{
  if (parser->matchKeyword ("TARGET") )
    {
			target_ = combatTargets[parser->getWord()];
      return OK;
    }
  if (parser->matchKeyword ("EFFECT") )
    {
			action_ = fx_actions[parser->getWord()];
			return OK;
    }

	if (parser->matchKeyword ("RANGE") )
    {
			range_ = parser->getInteger();
			return OK;
    }

	if (parser->matchKeyword ("BONUS") )
    {
     modifyingStats.initialize(parser);
    }
	else
		{
     nonCumulativeStats.initialize(parser);
		}
     return OK;
}



// get all potential targets
BattleTargets CombatActionStrategy::getPotentialTargets(
									BattleInstance * battleInstance, CombatReport * report)
{
  BattleTargets potentialTargets;
	BattleField * battleField = battleInstance->getBattleField();
//	CombatReport * report = battleField->getCombatEngine()->getCombatReport();

	battleField->addEnemies(battleInstance, potentialTargets, FORWARD);
	battleField->addEnemies(battleInstance, potentialTargets, LEFT);
	battleField->addEnemies(battleInstance, potentialTargets, RIGHT);
	battleField->addEnemies(battleInstance, potentialTargets, BACKWARD);

	if(potentialTargets.empty()) // No targets. Report
	{
		new UnaryMessage(noTargetsReporter,battleInstance->getOrigin()) >>*report ;
		return potentialTargets;
	}

	int stealthBonus;


	for(vector <BattleTargetElement>::iterator iter =potentialTargets.begin();
	iter != potentialTargets.end(); )
	{
	 stealthBonus = (*iter).instance_->getStealth() -
	 													 battleInstance->getObservation();
	 if(stealthBonus > 0)
	 {
	 	 if(stealthBonus > Roll_1Dx(10))
		 	{
				potentialTargets.erase(iter);
				continue;
		 	}
		}
		++iter;
	}
	if(potentialTargets.empty()) // No targets.
	{
		new UnaryMessage(noTargetsReporter,battleInstance->getOrigin()) >>*report ;
		return potentialTargets;
	}

  std::sort(potentialTargets.begin(),potentialTargets.end(),
		&attackPreference);

   combatReportFile << "After sorting: "<<endl;
	for(vector <BattleTargetElement>::iterator iter =potentialTargets.begin();
	iter != potentialTargets.end(); ++iter)
	{
   combatReportFile <<(*iter).instance_->print()<< " ";
	}
	combatReportFile<<endl;

	return potentialTargets;
}



//--  Performs attacks against all potential targets untill all of them are dead
//--   or maximum hits
MeleeReport CombatActionStrategy::makeAttack(BattleInstance * battleInstance,
		BattleTargets & potentialTargets, CombatReport * report)
{
	BattleInstance * target;
	int targetSize;
  MeleeReport attacks;

// apply changes to stats if any (recalculate stats)

  battleInstance->recalculateStats();

	for(vector <BattleTargetElement>::iterator iter =potentialTargets.begin();
	iter != potentialTargets.end(); ++iter)
	{
	 attacks.push_back(MeleeAttackElement((*iter).instance_,0,0,0));
	}

	int totalHits =battleInstance->getFiguresNumber();


	while (totalHits)
	{
	  if(potentialTargets.empty())
			break;
		for(vector <BattleTargetElement>::iterator iter =potentialTargets.begin();
		iter != potentialTargets.end(); )
		{
			target = (*iter).instance_;
	  	targetSize = target->getFiguresNumber();
			unsigned int i; // index of attackMessage related to current target
			for (i= 0; i< attacks.size(); ++i)
			{
			 if(attacks[i].target == target)
			 	break;
			}

			MeleeAttackElement attackResults(0,0,0,0);
			if(totalHits > targetSize)
				{
					attackResults = attack(battleInstance, target, targetSize);
					attacks[i] = attacks[i] + attackResults;
					totalHits -=targetSize;
					if(attackResults.killed >= targetSize)
					{
						potentialTargets.erase(iter);
						continue;
					}
				}
			else
				{
					attackResults = attack(battleInstance, target, totalHits);
					attacks[i] = attacks[i] + attackResults;
					totalHits = 0;
					if(attackResults.killed >= targetSize)
					{
						potentialTargets.erase(iter);
						continue;
					}
					break;
				}
			++iter;
		}
	}
// Was it multihit?

    int hitNumbers = battleInstance->getHitNumbers();
		if(hitNumbers > 1)
		{
			battleInstance->processMultiHitting(hitNumbers);
		}
	return attacks;
}



MeleeAttackElement CombatActionStrategy::attack(BattleInstance * battleInstance, BattleInstance * target, int totalHits)
{
	cerr << "Illegal call to virtual function CombatActionStrategy::attack\n";
	return MeleeAttackElement(target, 0, 0, 0);
}



bool CombatActionStrategy::attackPreference( const BattleTargetElement & trg1, const BattleTargetElement & trg2)
{

/*// Sort by number times being attacked
	if(trg1.instance_->getAttackedOnRound() && !trg2.instance_->getAttackedOnRound())
		return false;
	if(!trg1.instance_->getAttackedOnRound() &&  trg2.instance_->getAttackedOnRound())
		return true;*/

// Sort by stealth
	if(trg1.instance_->getStealth() < trg2.instance_->getStealth())
		return true;
	if(trg1.instance_->getStealth() > trg2.instance_->getStealth())
		return false;

// Sort by direction
	if((trg1.battleDirection_ == FORWARD) && (trg2.battleDirection_ != FORWARD))
		return true;
	if((trg2.battleDirection_ == FORWARD) && (trg1.battleDirection_ != FORWARD))
		return false;
	if((trg1.battleDirection_ == LEFT) && (trg2.battleDirection_ != LEFT))
		return true;
	if((trg2.battleDirection_ == LEFT) && (trg1.battleDirection_ != LEFT))
		return false;
	if((trg1.battleDirection_ == RIGHT) && (trg2.battleDirection_ != RIGHT))
		return true;
	if((trg2.battleDirection_ == RIGHT) && (trg1.battleDirection_ != RIGHT))
		return false;

// sort by range
	if(trg1.range_ < trg2.range_)
		return true;

		return false;
}



int CombatActionStrategy::calculateHitNumber(int numStrikes, int att, int def)
{

	if(att <= 0)
		{
			return 0; // Never hits
		}
	if(def <= 0)
		{
			return  numStrikes; // Allways hits
		}

	int hitNums =0;
	int hitChance = 0;

	if(att > def)
	{
		hitChance = att - def + 2; // chance: (att-def)+1/((att - def + 1) + 1)
		for (int i = 0; i < numStrikes; ++i)
		{
 			if ( Roll_1Dx(hitChance)  != 0)
				hitNums++;
		}
		return hitNums;
	}
	else
	{
		hitChance = def - att + 2;
		for (int i = 0; i < numStrikes; ++i)
		{

			if ( Roll_1Dx(hitChance) == 0)
			{
				hitNums++;
			}
		}
		return hitNums;
	}
}



int CombatActionStrategy::getInitiative()
{
	int initiative = nonCumulativeStats.getInitiative();
	if(initiative == 0)
	{
	initiative = modifyingStats.getInitiative();
	}
	return initiative;
}

