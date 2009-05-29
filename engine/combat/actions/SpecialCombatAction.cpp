/***************************************************************************
                          SpecialCombatAction.cpp
                             -------------------
    begin                : Sun jul 03 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "SpecialCombatAction.h"
#include "TokenEntity.h"
#include "CombatEngine.h"
#include "BattleInstance.h"
#include "BattleField.h"
#include "reporting.h"
#include "CombatAttackMessage.h"
#include "CombatReport.h"

extern int Roll_1Dx(int n);
extern ReportPattern * noTargetsReporter;

SpecialCombatAction     sampleSpecialAction     ("SPECIAL",  &sampleCombatAction);

GameData * SpecialCombatAction::createInstanceOfSelf()
{
  return CREATE_INSTANCE<SpecialCombatAction> (this);
}




// dummy function. Returns instance as target
BattleTargets SpecialCombatAction::getPotentialTargets(
									BattleInstance * battleInstance, CombatReport * report)
{
  BattleTargets potentialTargets;
	potentialTargets.push_back(BattleTargetElement(battleInstance,FORWARD,0));
	return potentialTargets;
}


// dummy function. Prodices warning. Returns dummy attack result
MeleeReport		SpecialCombatAction::makeAttack(BattleInstance * battleInstance, BattleTargets & potentialTargets, CombatReport * report)
{
  MeleeReport attacks;
	attacks.push_back(MeleeAttackElement(battleInstance,0,0,0));
	cerr << "Combat Action "<< keyword_ <<" is not implemented yet!\n";
	return attacks;
}

