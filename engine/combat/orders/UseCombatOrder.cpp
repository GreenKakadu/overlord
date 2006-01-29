/***************************************************************************
                          UseCombatOrder.cpp
                             -------------------
    begin                : Thu Nov 16 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "UseCombatOrder.h"
#include "CombatActionStrategy.h"
#include "TokenEntity.h"
#include "BattleInstance.h"
#include "BattleField.h"
#include "CombatReport.h"
#include "CombatEngine.h"
#include "reporting.h"



UseCombatOrder * instantiateUseCombatOrder = new UseCombatOrder();
//ReportPattern * combatParryReporter= new ReportPattern("combatParryReporter");

UseCombatOrder::UseCombatOrder(){
  keyword_ = "CombatUse";
  registerOrder_();
  description = string("USE:  [item tag]\n") +
	"The unit attempts to use the indicated item, if there is a valid\n" +
  "target within range.\n";
  orderType_   = COMBAT_ACTION_ORDER;
  initiative_ = 0;
 isSequentive_ = true;
}

STATUS UseCombatOrder::loadParameters(Parser * parser,
                         ParameterList &parameters, Entity * entity )
{
   if(!entityIsTokenEntity(entity))
            return IO_ERROR;
    if(!parseGameDataParameter(entity,  parser, items, "item tag", parameters))
            return IO_ERROR;

	// Set initiative to initiative of CombatAction
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
	ItemRule * item = dynamic_cast<ItemRule *>(parameters[0]);

	if(item == 0)
	{
   return OK;
	}

	if(!unit->hasItem(item))
	{
   return OK;
	}

	CombatActionStrategy * combatAction = 	item->getCombatAction();
	if(combatAction == 0)
	{
   return OK;
	}

//	combatReportFile << "Loading USE " << item->printTag()<< " order for " <<unit->printTag();
	initiative_ = combatAction->getInitiative();
//	combatReportFile << " Initiative " << initiative_<<endl;
	// initiative is relative. That's why nonCumulative initiative stat is not used

   return OK;

}



ORDER_STATUS UseCombatOrder::process (Entity * entity, ParameterList &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
 ItemRule * item = dynamic_cast< ItemRule*>(parameters[0]);

  if ( item == 0) // item doesn't exist but we don't want to let player to know that
     {
 		  return FAILURE;
      }

	if(!unit->hasItem(item))
	{
   return FAILURE;
	}

	CombatActionStrategy * combatAction = 	item->getCombatAction();
	if(combatAction == 0)
	{
   return FAILURE;
	}

	combatReportFile<<entity<<" uses "<<item->print() <<endl;


  BattleInstance * battleInstance = unit->getBattleInstantiation();
	CombatReport * report = battleInstance->getBattleField()->getCombatEngine()
										->getCombatReport();


 	vector <BattleTargetElement> potentialTargets =
			 					combatAction->getPotentialTargets(battleInstance, report);
	if(potentialTargets.empty()) // No targets. But already eported
	{
		return FAILURE;
	}
	battleInstance->setAffectingAction(combatAction);
  vector <MeleeAttackElement> attacks =
	combatAction->makeAttack(battleInstance, potentialTargets, report);

	return SUCCESS;
}

