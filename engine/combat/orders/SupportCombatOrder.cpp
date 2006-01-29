/***************************************************************************
                          SupportCombatOrder.cpp
                             -------------------
    begin                : Thu Nov 16 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "SupportCombatOrder.h"
#include "TokenEntity.h"
#include "BattleInstance.h"
#include "BattleField.h"
#include "CombatReport.h"
#include "CombatEngine.h"
#include "reporting.h"
#include "RetreatCombatOrder.h"



SupportCombatOrder * instantiateSupportCombatOrder = new SupportCombatOrder();
extern ReportPattern * combatRetreatReporter;
extern ReportPattern * combatAdvanceReporter;

SupportCombatOrder::SupportCombatOrder(){
  keyword_ = "Support";
  registerOrder_();
  description = string("SUPPORT:  \n") +
	"Essentially the same setting as FIRE, but the unit will also retreat\n" +
  "if an opposing unit is present on an adjacent square\n" ;
  orderType_   = COMBAT_MOVEMENT_ORDER;
  initiative_ = 0;
 isSequentive_ = true;
}

STATUS SupportCombatOrder::loadParameters(Parser * parser,
                         ParameterList &parameters, Entity * entity )
{
   if(!entityIsTokenEntity(entity))
            return IO_ERROR;
   return OK;

}


ORDER_STATUS SupportCombatOrder::process (Entity * entity, ParameterList &parameters)
{

 TokenEntity * unit = dynamic_cast<TokenEntity *>(entity);
  assert(unit);
  BattleInstance * instance = unit->getBattleInstantiation();
	BattleField * battleField = instance->getBattleField();
  int initiative = battleField->getCombatEngine()->getCurrentInitiativeSegment();

// Retreat?

    if(RetreatCombatOrder::retreatAway(instance))
		{
				return SUCCESS;
		}




// === End of retreat attempts

//
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



