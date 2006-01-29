/***************************************************************************
                          AdvanceCombatOrder.cpp
                             -------------------
    begin                : Thu Nov 09 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "AdvanceCombatOrder.h"
#include "TokenEntity.h"
#include "BattleInstance.h"
#include "BattleField.h"
#include "CombatReport.h"
#include "CombatEngine.h"
#include "reporting.h"

//extern ReportPattern *	invalidParameterReporter;
//extern ReportPattern *	missingParameterReporter;

AdvanceCombatOrder * instantiateAdvanceCombatOrder = new AdvanceCombatOrder();
ReportPattern * combatAdvanceReporter= new ReportPattern("combatAdvanceReporter");

AdvanceCombatOrder::AdvanceCombatOrder(){
  keyword_ = "Advance";
  registerOrder_();
  description = string("ADVANCE:  \n") +
 "The unit will advance to the square in front of it if it contains no \n" +
 "opposing units.  If multiple units have the same initiative, and want to\n" +
 "advance to the same square, one will be chosen at random to advance first,\n" +
 "then all the units on the same side.  A unit that has all figures equipped\n" +
 "with a mount has a double chance of moving first.\n";
  orderType_   = COMBAT_MOVEMENT_ORDER;
  initiative_ = 0;
 isSequentive_ = true;
}

STATUS AdvanceCombatOrder::loadParameters(Parser * parser,
                         ParameterList &parameters, Entity * entity )
{
   if(!entityIsTokenEntity(entity))
            return IO_ERROR;
   return OK;

}



ORDER_STATUS AdvanceCombatOrder::process (Entity * entity, ParameterList &parameters)
{
 TokenEntity * unit = dynamic_cast<TokenEntity *>(entity);
  assert(unit);
  BattleInstance * instance = unit->getBattleInstantiation();
	BattleField * battleField = instance->getBattleField();
  int initiative = battleField->getCombatEngine()->getCurrentInitiativeSegment();

	if(!battleField->haveEnemies(instance,FORWARD))
	{
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

