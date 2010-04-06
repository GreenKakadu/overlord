/***************************************************************************
                          MeleeCombatAction.cpp
                             -------------------
    begin                : Wed Apr 27 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "MeleeCombatAction.h"
#include "TokenEntity.h"
#include "CombatEngine.h"
//#include "BattleInstance.h"
//#include "BattleField.h"
#include "reporting.h"
#include "CombatAttackMessage.h"

extern int Roll_1Dx(int n);
extern ReportPattern * noTargetsReporter;
ReportPattern * unitSlainReporter= new ReportPattern("unitSlainReporter");

MeleeCombatAction     sampleMeleeAction     ("MELEE",  &sampleCombatAction);

GameData * MeleeCombatAction::createInstanceOfSelf()
{
  return CREATE_INSTANCE<MeleeCombatAction> (this);
}

// get all potential targets
BattleTargets MeleeCombatAction::getPotentialTargets(
			BattleInstance * battleInstance, CombatReport * report)
{
  BattleTargets potentialTargets;
	if(battleInstance->getDamage()==0)// Can't do damage anyway
        {
            if(battleInstance->isTraced())
            {
            combatReportFile<<battleInstance->print()<<" cat'd do damage"<<endl;
            }
		return potentialTargets;
        }
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

   combatReportFile << "-------- Targets after sorting: (";
	for(vector <BattleTargetElement>::iterator iter =potentialTargets.begin();
	iter != potentialTargets.end(); ++iter)
	{
   combatReportFile <<(*iter).instance_->print()<< " ";
	}
	combatReportFile<< ")"<<endl;

	return potentialTargets;
}






MeleeAttackElement MeleeCombatAction::meleeAttack(
					BattleInstance * battleInstance, int melee, int damage,
					BattleInstance * target, int attFigNum)
{
int woundLevel;

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
//	combatReportFile<<"     "<<"     "<<"     "<< " roll " <<i+1<<": "<<roll<<endl;
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
combatReportFile  <<"     "<<"     "<< "Number of hits " << hitNums<<endl;;

if(hitNums)
{
	battleInstance->getBattleField()->getCombatEngine()->
		clearWoundlessRoundCounter();
//  ====== Additional rule for wounds
int correctedHits=0;
combatReportFile  <<"   Melee: "<<battleInstance->getMelee()<<" Defence "<<targetDefence<<endl;
if(battleInstance->getMelee() < targetDefence)
{
combatReportFile  <<"   Some hits would be converted to wounds  "<<endl;
		woundLevel = 40 + 4 * melee; //should use unarmed melee
		for(int i = 0; i < hitNums; ++i)
		{
			int roll = Roll_1Dx(100);
 			combatReportFile  <<" ["<<woundLevel<<">"<<roll<<"] "; 
			if(woundLevel > roll)
				correctedHits++;
		}
	int wounds = hitNums - correctedHits;
	hitNums = correctedHits;
combatReportFile  <<", corrected: "<<correctedHits<<" turned to wounds " <<wounds;

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
}
// ====== End of additional rule for wounds
combatReportFile  <<" final hits: "<< hitNums <<endl;
// === Distribute hits between figures
}
  int totalDamage = damage * hitNums;
	int  figuresDied = target->sufferDamage(hitNums,damage);


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




void		MeleeCombatAction::performAction(BattleInstance * battleInstance, BattleTargets & target, CombatReport * report)
{
	if(battleInstance->getDamage() ==0) // can't do damage. No attack.
	{
	return;
	}
	int initiative = battleInstance->getBattleField()->getCombatEngine()->
										getCurrentInitiativeSegment();

	combatReportFile<<" Initiative: "<<initiative<<endl;
  vector <MeleeAttackElement> attacks =
	makeAttack(battleInstance, target, report);

	battleInstance->addMeleeExperience(1);

	bool killedAll = false;
	for (unsigned int i= 0; i< attacks.size(); ++i)
			{
                          if(attacks[i].target == 0)
                            continue;
                          if(attacks[i].target->getFiguresNumber() == 0)
					killedAll = true;
				else
					killedAll = false;

				new CombatAttackMessage(initiative, battleInstance,
						attacks[i].hits, attacks[i].target, attacks[i].damage,
						attacks[i].killed, killedAll)>>*report;
 				if(killedAll)
					new UnaryMessage(unitSlainReporter,
						attacks[i].target->getOrigin()) >>*report ;
			}


}



