/***************************************************************************
                          HeavinessCombatAction.cpp
                             -------------------
    begin                : Sun jul 03 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "HeavinessCombatAction.h"
#include "TokenEntity.h"
#include "CombatEngine.h"
#include "BattleInstance.h"
#include "BattleField.h"
#include "reporting.h"
#include "CombatAttackMessage.h"
#include "CombatReport.h"

extern int Roll_1Dx(int n);
extern ReportPattern * noTargetsReporter;

HeavinessCombatAction     sampleHeavinessAction     ("HEAVINESS",  &sampleCombatAction);

GameData * HeavinessCombatAction::createInstanceOfSelf()
{
  return CREATE_INSTANCE<HeavinessCombatAction> (this);
}





// get all potential targets
BattleTargets HeavinessCombatAction::getPotentialTargets(
									BattleInstance * battleInstance, CombatReport * report)
{
  BattleTargets potentialTargets;
	BattleField * battleField = battleInstance->getBattleField();

	battleField->addEnemies(battleInstance, potentialTargets, FORWARD);
	battleField->addEnemies(battleInstance, potentialTargets, LEFT);
	battleField->addEnemies(battleInstance, potentialTargets, RIGHT);
	battleField->addEnemies(battleInstance, potentialTargets, BACKWARD);

	if(potentialTargets.empty()) // No targets. Report
	{
		new UnaryMessage(noTargetsReporter,battleInstance->getOrigin()) >>*report ;
		return potentialTargets;
	}

	std::sort(potentialTargets.begin(),potentialTargets.end(),
		HeavinessCombatAction::attackPreference);

	return potentialTargets;

}




MeleeReport		HeavinessCombatAction::makeAttack(BattleInstance * battleInstance, BattleTargets & potentialTargets, CombatReport * report)
{
  MeleeReport attacks;
	attacks.push_back(MeleeAttackElement(battleInstance,0,0,0));
	cerr << "Combat Action Heaviness ("<< keyword_ <<") is not implemented yet!\n";
	return attacks;
}


