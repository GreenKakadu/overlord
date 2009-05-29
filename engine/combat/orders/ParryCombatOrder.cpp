/***************************************************************************
                          ParryCombatOrder.cpp
                             -------------------
    begin                : Thu Nov 16 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "ParryCombatOrder.h"
#include "TokenEntity.h"
#include "BattleInstance.h"
#include "BattleField.h"
#include "CombatReport.h"
#include "CombatEngine.h"
#include "reporting.h"



ParryCombatOrder * instantiateParryCombatOrder = new ParryCombatOrder();
ReportPattern * combatParryReporter= new ReportPattern("combatParryReporter");

ParryCombatOrder::ParryCombatOrder(){
  keyword_ = "Parry";
  registerOrder_();
  description = string("PARRY:  \n") +
	"The unit does not attempt to attack, and adds its attack rating to its\n" +
  "defense rating for the rest of the round. PARRY has a +2 initiative bonus.\n";
  orderType_   = COMBAT_ACTION_ORDER;
  initiative_ = +2;
 isSequentive_ = true;
  mayInterrupt_ = true;
}

STATUS ParryCombatOrder::loadParameters(Parser * parser,
                         ParameterList &parameters, Entity * entity )
{
   if(!entityIsTokenEntity(entity))
            return IO_ERROR;
   return OK;

}



ORDER_STATUS ParryCombatOrder::process (Entity * entity, ParameterList &parameters)
{
 TokenEntity * unit = dynamic_cast<TokenEntity *>(entity);
  assert(unit);
  BattleInstance * instance = unit->getBattleInstantiation();
	BattleField * battleField = instance->getBattleField();
 int initiative = battleField->	getCombatEngine()->getCurrentInitiativeSegment();
	combatReportFile<<entity<<" parries."<<endl;
  instance->parry();
	// Report
 	new BinaryMessage(combatParryReporter, new IntegerData(initiative), entity)>>*(battleField->getCombatReport()) ;
	return SUCCESS;
}

