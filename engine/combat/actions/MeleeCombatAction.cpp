/***************************************************************************
                          MeleeCombatAction.cpp
                             -------------------
    begin                : Wed Apr 27 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "MeleeCombatAction.h"
#include "TokenEntity.h"
#include "CombatEngine.h"
//#include "BattleInstance.h"
//#include "BattleField.h"
#include "reporting.h"
//#include "CombatAttackMessage.h"

extern int Roll_1Dx(int n);
extern ReportPattern * noTargetsReporter;

MeleeCombatAction     sampleMeleeAction     ("MELEE",  &sampleCombatAction);

GameData * MeleeCombatAction::createInstanceOfSelf()
{
  return CREATE_INSTANCE<MeleeCombatAction> (this);
}







MeleeAttackElement MeleeCombatAction::meleeAttack(
					BattleInstance * battleInstance, int melee, int damage,
					BattleInstance * target, int attFigNum)
{


	 combatReportFile <<"     " <<battleInstance->print()<< " attacks "<<target->print() <<endl;
	target->setAttackedOnRound();
	target->setFleeCounter(0);

  int targetDefence = target->getDefence();
	// Flying bonuses
	if(battleInstance->isFlying() && !target->isFlying())
	{
		melee = melee + 1;
	}
	if(! battleInstance->isFlying() && target->isFlying())
	{
    targetDefence += 5;
	}

	// check damage != 0
	int chanceToStike = 40 + 4 * melee;
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

   int hitNums =calculateHitNumber(numStrikes, melee, targetDefence);




combatReportFile  <<"     "<<"     "<< "Attack value "
 <<melee <<endl;
combatReportFile  <<"     "<<"     "<< "Defence value "
 << targetDefence << endl;
combatReportFile  <<"     "<<"     "<< "Number of hits " << hitNums;

if(hitNums)
	battleInstance->getBattleField()->getCombatEngine()->
		clearWoundlessRoundCounter();
//  ====== Additional rule for wounds
int correctedHits=0;
if(battleInstance->getMelee() < targetDefence)
	{
		int woundLevel = 40 + 4 * melee; //should use unarmed melee
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



// Attack against single target
// Returns number of killed figures.
MeleeAttackElement MeleeCombatAction::attack(BattleInstance * battleInstance,
					BattleInstance * target, int totalHits)
{
//	if(this->
	int melee =  battleInstance->getMelee();
	int damage = battleInstance->getDamage();
	return meleeAttack( battleInstance,  melee,  damage, target,  totalHits);
}







