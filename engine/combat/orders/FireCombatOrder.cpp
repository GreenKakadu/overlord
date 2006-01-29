/***************************************************************************
                          FireCombatOrder.cpp
                             -------------------
    begin                : Thu Nov 16 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "FireCombatOrder.h"
#include "TokenEntity.h"
#include "BattleInstance.h"
#include "BattleField.h"
#include "CombatReport.h"
#include "CombatEngine.h"
#include "reporting.h"



FireCombatOrder * instantiateFireCombatOrder = new FireCombatOrder();
extern ReportPattern * combatAdvanceReporter;
//ReportPattern * combatParryReporter= new ReportPattern("combatParryReporter");

FireCombatOrder::FireCombatOrder(){
  keyword_ = "Fire";
  registerOrder_();
  description = string("FIRE:  \n") +
	"Essentially the same setting as ADVANCE, but the unit will not try to\n" +
  "advance unless there are no units for two squares, leaving at least one\n" +
  "square between itself and the nearest opposing unit.\n";
  orderType_   = COMBAT_MOVEMENT_ORDER;
  initiative_ = 0;
 isSequentive_ = true;
}

STATUS FireCombatOrder::loadParameters(Parser * parser,
                         ParameterList &parameters, Entity * entity )
{
   if(!entityIsTokenEntity(entity))
            return IO_ERROR;
   return OK;

}



ORDER_STATUS FireCombatOrder::process (Entity * entity, ParameterList &parameters)
{
 TokenEntity * unit = dynamic_cast<TokenEntity *>(entity);
  assert(unit);
  BattleInstance * instance = unit->getBattleInstantiation();
	BattleField * battleField = instance->getBattleField();
  int initiative = battleField->getCombatEngine()->getCurrentInitiativeSegment();

	if(!battleField->haveEnemies(instance,FORWARD))
	{
    if(battleField->haveEnemies(instance,FORWARD,2) ||
       battleField->haveEnemiesRelative(instance,+1,+1) ||
       battleField->haveEnemiesRelative(instance,+1,-1)		)
		{
	    combatReportFile<<entity<<" prefers not to advance and fire from distance."<<endl;
				return SUCCESS;
		}

			combatReportFile<<entity<<" advances from ("<<instance->getRank() << ","<<instance->getFile()<<")";

			if(battleField->moveEntity(unit,FORWARD))
			{

	    	combatReportFile<<" to ("<<instance->getRank() <<
				 ","<<instance->getFile()<<")"<<endl;

 				new TertiaryMessage(combatAdvanceReporter,
					new IntegerData(initiative), entity,
					new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return SUCCESS;
			}
	}
	else
	    combatReportFile<<entity<<" can't advance."<<endl;


	return SUCCESS;
}

