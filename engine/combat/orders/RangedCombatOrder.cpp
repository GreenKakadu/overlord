/***************************************************************************
                          RangedCombatOrder.cpp
                             -------------------
    begin                : Wed Apr 20 2005
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "RangedCombatOrder.h"
#include "RangedCombatAction.h"
#include "TokenEntity.h"
#include "BattleInstance.h"
#include "BattleField.h"
#include "CombatReport.h"
#include "CombatEngine.h"
#include "CombatAttackMessage.h"
#include "reporting.h"



RangedCombatOrder * instantiateRangedCombatOrder = new RangedCombatOrder();
//ReportPattern * combatParryReporter= new ReportPattern("combatParryReporter");

RangedCombatOrder::RangedCombatOrder(){
  keyword_ = "Ranged";
  registerOrder_();
  description = string("RANGED:  \n") +
  "The unit fires ranged attack against one of the opposing units at range 2.\n" +
	"Units in adjacent squares can't be attacked. If there are opposing \n" +
  "units in front, those units will be selected for attack. \n" +
  "If not, opposing units on left or right will be attacked.\n" +
  "Then units behind will be attacked. In all cases the nearest target \n" +
  "will be selected. If no targets available, the order is skipped.\n";
  orderType_   = COMBAT_ACTION_ORDER;
  initiative_ = 0;
 isSequentive_ = true;
  mayInterrupt_ = true;
}

STATUS RangedCombatOrder::loadParameters(Parser * parser,
                         ParameterList &parameters, Entity * entity )
{
   if(!entityIsTokenEntity(entity))
            return IO_ERROR;
   return OK;

}


bool RangedCombatOrder::evaluate(Entity * entity, ParameterList  &)
{
 TokenEntity * unit = dynamic_cast<TokenEntity *>(entity);
  assert(unit);
  BattleInstance * battleInstance = unit->getBattleInstantiation();
	BattleField * battleField = battleInstance->getBattleField();

	combatReportFile<<entity<<" testing possibility of attack:"<<endl;

  BattleTargets potentialTargets;

	battleField->addAllEnemiesAtRange(battleInstance, potentialTargets, 2);
// ranged attack at range 2

	if(potentialTargets.empty()) // No targets. Report
	{
		return false;
	}

		return true;
}





ORDER_STATUS RangedCombatOrder::process (Entity * entity, ParameterList &parameters)
{
 TokenEntity * unit = dynamic_cast<TokenEntity *>(entity);
  assert(unit);
  BattleInstance * battleInstance = unit->getBattleInstantiation();
  CombatReport * report = battleInstance->getBattleField()->getCombatEngine()->getCombatReport();
 // Fails if ranged damage of unit =0 
  if((battleInstance->getRangedDamage() <= 0)/* || (battleInstance->getRange() == 0)*/)
  {
    return FAILURE;
  }
  // If there are adjacient units RANGED fails
  BattleField * battleField = battleInstance->getBattleField();
  if(  battleField->haveEnemiesRelative(battleInstance, -1, -1) ||
       battleField->haveEnemiesRelative(battleInstance, +1, +1) ||
       battleField->haveEnemiesRelative(battleInstance, +1, -1) ||
       battleField->haveEnemiesRelative(battleInstance, -1, +1) )
  {
    return FAILURE;
  }  
  vector <BattleTargetElement> potentialTargets =
          sampleRangedAction.getPotentialTargets(battleInstance, report);

  if(potentialTargets.empty()) // No targets. Already reported
  {
          return FAILURE;
  }


  sampleRangedAction.performAction(battleInstance, potentialTargets, report);

//			instance->setActedOnRound(true);
			return SUCCESS;

}

