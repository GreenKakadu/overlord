/***************************************************************************
                          RangedCombatAction.cpp
                             -------------------
    begin                : Sun jul 03 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "RangedCombatAction.h"
#include "TokenEntity.h"
#include "CombatEngine.h"
#include "BattleInstance.h"
#include "reporting.h"


extern int Roll_1Dx(int n);
extern ReportPattern * noTargetsReporter;

RangedCombatAction     sampleRangedAction     ("RANGED",  &sampleCombatAction);

GameData * RangedCombatAction::createInstanceOfSelf()
{
  return CREATE_INSTANCE<RangedCombatAction> (this);
}





// get all potential targets
BattleTargets RangedCombatAction::getPotentialTargets(
									BattleInstance * battleInstance, CombatReport * report)
{
  BattleTargets potentialTargets;
	BattleField * battleField = battleInstance->getBattleField();

	battleField->addAllEnemiesAtRange(battleInstance, potentialTargets, range_);


	if(potentialTargets.empty()) // No targets. Report
	{
		return potentialTargets;
	}

	int stealthBonus;


	for(vector <BattleTargetElement>::iterator iter =potentialTargets.begin();
	iter != potentialTargets.end(); )
	{
		if((*iter).range_ == 1) // Ranged attacks may not target adjacient units
		 	{
				potentialTargets.erase(iter);
				continue;
		 	}
  // may be
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
		RangedCombatAction::attackPreference);

   combatReportFile << "After sorting: "<<endl;
	for(vector <BattleTargetElement>::iterator iter =potentialTargets.begin();
	iter != potentialTargets.end(); ++iter)
	{
   combatReportFile <<(*iter).instance_->print()<< " ";
	}
	combatReportFile<<endl;

	return potentialTargets;
}




MeleeAttackElement RangedCombatAction::attack(BattleInstance * battleInstance,
					BattleInstance * target, int totalHits)
{
 	int missile =  battleInstance->getMissile();
	int damage = battleInstance->getDamage();
	return rangedAttack( battleInstance,  missile,  damage, target,  totalHits);
}


// It is almost the same as melee attack
// but it may evolve away in future
MeleeAttackElement RangedCombatAction::rangedAttack(BattleInstance * battleInstance,
							int missile, int damage, BattleInstance * target, int attFigNum)
{


	 combatReportFile <<"     " <<battleInstance->print()<< " fires at "<<target->print() <<endl;
	target->setAttackedOnRound();

  int targetDefence = target->getDefence();

	// Flying bonuses
	if(battleInstance->isFlying() && !target->isFlying())
	{
		missile = missile + 1;
	}
	if(! battleInstance->isFlying() && target->isFlying())
	{
    targetDefence += 2; // may be better decrease defence?
	}

	targetDefence = targetDefence / 3;

	// check damage != 0
	int chanceToStike = 40 + 4 * missile;
	int numStrikes = 0;


	for (int i = 0; i < attFigNum; ++i)
	{
	 int roll = Roll_1Dx(100);
	combatReportFile<<"     "<<"     "<<"     "<< " roll " <<i+1<<": "<<roll<<endl;
	 if (chanceToStike  >  roll )
// 		if (chanceToStike  >  Roll_1Dx(100) )
			numStrikes++;
 	}

combatReportFile  <<"     "<<"     "<< "Chance to strike " <<chanceToStike<<endl;
combatReportFile  <<"     "<<"     "<< "Figures " <<attFigNum<<endl;
combatReportFile  <<"     "<<"     "<< "Number of strikes " <<numStrikes<<endl;

   int hitNums =calculateHitNumber(numStrikes, missile, targetDefence);




combatReportFile  <<"     "<<"     "<< "Attack value "
 << missile<<endl;
combatReportFile  <<"     "<<"     "<< "Defence value "
 << targetDefence  << endl;
combatReportFile  <<"     "<<"     "<< "Number of hits " << hitNums;

if(hitNums)
	battleInstance->getBattleField()->getCombatEngine()->
		clearWoundlessRoundCounter();
//  ====== Additional rule for wounds
int correctedHits=0;
if(missile < targetDefence)
	{
		int woundLevel = 40 + 4 * missile;
		for(int i = 0; i < hitNums; ++i)
		{
			if(woundLevel > Roll_1Dx(100))
				correctedHits++;
		}
	}
	int wounds = hitNums - correctedHits;
	hitNums = correctedHits;

	// Convert wounds to hits
	int currentWounds = target->getWounds();
	int currentFigures = target->getFiguresNumber();

	if(currentWounds + wounds <= currentFigures)
	{
		target->setWounds(currentWounds + wounds);
	}
	else
	{
		target->setWounds(currentFigures);
  	hitNums += (currentWounds + wounds - currentFigures);
	}
// ====== End of additional rule for wounds
combatReportFile  <<"("<< hitNums<<")" <<endl;
// === Distribute hits between figures

  int totalDamage = damage * hitNums;
	int  figuresDied = target->sufferDamage(hitNums,totalDamage);


	return MeleeAttackElement(target, numStrikes, totalDamage, figuresDied);
}
