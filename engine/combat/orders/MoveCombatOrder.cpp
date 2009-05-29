/***************************************************************************
                          MoveCombatOrder.cpp
                             -------------------
    begin                : Dec 08 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "MoveCombatOrder.h"
#include "TokenEntity.h"
#include "BattleInstance.h"
#include "CombatEngine.h"
#include "CombatReport.h"
#include "BattleField.h"
#include "reporting.h"
//extern ReportPattern *	invalidParameterReporter;
//extern ReportPattern *	missingParameterReporter;

ReportPattern * combatMoveReporter= new ReportPattern("combatMoveReporter");
MoveCombatOrder * instantiateMoveCombatOrder = new MoveCombatOrder();

MoveCombatOrder::MoveCombatOrder(){
  keyword_ = "COMBAT_MOVE";
  registerOrder_();
  description = string("MOVE: [dir] \n") +
  "The unit moves in specified ditection\n";
  orderType_   = COMBAT_MOVEMENT_ORDER;
  initiative_ = 0;
  mayInterrupt_ = true;
}

STATUS MoveCombatOrder::loadParameters(Parser * parser,
                         ParameterList &parameters, Entity * entity )
{
   if(!entityIsTokenEntity(entity))
            return IO_ERROR;
   return OK;

}



ORDER_STATUS MoveCombatOrder::process (Entity * entity, ParameterList &parameters)
{
 TokenEntity * unit = dynamic_cast<TokenEntity *>(entity);
  assert(unit);
  BattleInstance * instance = unit->getBattleInstantiation();
	BattleField * battleField = instance->getBattleField();

  int initiative = battleField->getCombatEngine()->getCurrentInitiativeSegment();

	if(!battleField->haveEnemies(instance,FORWARD))
	{
	    combatReportFile<<entity<<" moves."<<endl;
			if(battleField->moveEntity(unit,FORWARD))
			{
 				new TertiaryMessage(combatMoveReporter,
					new IntegerData(initiative), entity,
					new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return SUCCESS;
			}
	}
	else
	  combatReportFile<<entity<<" can't move."<<endl;
		return FAILURE;
}

