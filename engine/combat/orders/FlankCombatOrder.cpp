/***************************************************************************
                          FlankCombatOrder.cpp
                             -------------------
    begin                : Thu Nov 16 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "FlankCombatOrder.h"
#include "TokenEntity.h"
#include "BattleInstance.h"
#include "BattleField.h"
#include "CombatReport.h"
#include "CombatEngine.h"
#include "reporting.h"



FlankCombatOrder * instantiateFlankCombatOrder = new FlankCombatOrder();
extern ReportPattern * combatAdvanceReporter;
//ReportPattern * combatParryReporter= new ReportPattern("combatParryReporter");

FlankCombatOrder::FlankCombatOrder(){
  keyword_ = "Flank";
  registerOrder_();
  description = string("FLANK:  \n") +
	"Essentially the same setting as SWEEP, except if there is an opposing\n" +
  "unit on the same rank, but on a different file. If there is such an unit on\n" +
  "an opposing flank, the FLANK unit will try to move to the center, then move\n" +
  "back to its flank setting once all opposing units on the same rank are\n" +
  "removed.\n";
  orderType_   = COMBAT_MOVEMENT_ORDER;
  initiative_ = 0;
 isSequentive_ = true;
  mayInterrupt_ = true;
}

STATUS FlankCombatOrder::loadParameters(Parser * parser,
                         ParameterList &parameters, Entity * entity )
{
   if(!entityIsTokenEntity(entity))
            return IO_ERROR;
   return OK;

}



ORDER_STATUS FlankCombatOrder::process (Entity * entity, ParameterList &parameters)
{

 TokenEntity * unit = dynamic_cast<TokenEntity *>(entity);
  assert(unit);
  BattleInstance * instance = unit->getBattleInstantiation();
	BattleField * battleField = instance->getBattleField();
  int initiative = battleField->getCombatEngine()->getCurrentInitiativeSegment();

	if(battleField->haveEnemies(instance,LEFT)) // Not advancing
	{
		return SUCCESS;
	}

	if(battleField->haveEnemies(instance,RIGHT)) // Not advancing
	{
		return SUCCESS;
	}




	if(!battleField->haveEnemies(instance,FORWARD))
	{
	    combatReportFile<<entity<<" advancing(flank) from ("<<instance->getRank() << ","<<instance->getFile()<<")";

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
	    combatReportFile<<entity<<" can't advance(flank)."<<endl;
	return SUCCESS;
}

