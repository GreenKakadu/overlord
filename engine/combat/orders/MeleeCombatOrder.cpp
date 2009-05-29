/***************************************************************************
                          MeleeCombatOrder.cpp
                             -------------------
    begin                : Thu Nov 16 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "MeleeCombatOrder.h"
#include "TokenEntity.h"
#include "CombatEngine.h"
#include "BattleInstance.h"
#include "BattleField.h"
#include "reporting.h"
#include "CombatAttackMessage.h"
#include "CombatReport.h"
#include "MeleeCombatAction.h"

MeleeCombatOrder * instantiateMeleeCombatOrder = new MeleeCombatOrder();

MeleeCombatOrder::MeleeCombatOrder(){
  keyword_ = "Melee";
  registerOrder_();
  description = string("MELEE:  \n") +
  "The unit attacks hand to hand one of the opposing units in an adjacent\n" +
  "square. If there are opposing units on the same file, those units will\n" +
  "be engaged.  If not, opposing units on the same rank will be engaged.\n" +
  "If not, the order is skipped.\n";
  orderType_   = COMBAT_ACTION_ORDER;
  initiative_ = 0;
 isSequentive_ = true;
  mayInterrupt_ = true;
}

STATUS MeleeCombatOrder::loadParameters(Parser * parser,
                         ParameterList  &parameters, Entity * entity )
{
   if(!entityIsTokenEntity(entity))
            return IO_ERROR;
   return OK;

}



bool MeleeCombatOrder::evaluate(Entity * entity, ParameterList  &)
{
 TokenEntity * unit = dynamic_cast<TokenEntity *>(entity);
  assert(unit);
  BattleInstance * instance = unit->getBattleInstantiation();
	BattleField * battleField = instance->getBattleField();

	combatReportFile<<entity<<" testing possibility of attack:"<<endl;

	if(battleField->haveEnemies(instance, FORWARD))
		return true;
	if(battleField->haveEnemies(instance, LEFT))
		return true;
	if(battleField->haveEnemies(instance, RIGHT))
		return true;
	if(battleField->haveEnemies(instance, BACKWARD))
		return true;


	return false;
}







ORDER_STATUS MeleeCombatOrder::process (Entity * entity, ParameterList  &)
{
 TokenEntity * unit = dynamic_cast<TokenEntity *>(entity);
  assert(unit);
  BattleInstance * battleInstance = unit->getBattleInstantiation();
	CombatReport * report = battleInstance->getBattleField()->getCombatEngine()
										->getCombatReport();

	vector <BattleTargetElement> potentialTargets =
			 					sampleMeleeAction.getPotentialTargets(battleInstance, report);

	if(potentialTargets.empty()) // No targets. Already reported
	{
		return FAILURE;
	}

   sampleMeleeAction.performAction(battleInstance, potentialTargets, report);
//   vector <MeleeAttackElement> attacks =
// 	sampleMeleeAction.makeAttack(battleInstance, potentialTargets, report);
//
// 	battleInstance->addMeleeExperience(1);
// 
// 	bool killedAll = false;
// 	for (unsigned int i= 0; i< attacks.size(); ++i)
// 			{
// 				if(attacks[i].target->getFiguresNumber() == 0)
// 					killedAll = true;
// 				else
// 					killedAll = false;
// 
// 				new CombatAttackMessage(initiative, battleInstance,
// 						attacks[i].hits, attacks[i].target, attacks[i].damage,
// 						attacks[i].killed, killedAll)>>*report;
//  				if(killedAll)
// 					new UnaryMessage(unitSlainReporter,
// 						attacks[i].target->getOrigin()) >>*report ;
// 			}

//			instance->setActedOnRound(true);
			return SUCCESS;
}



