/***************************************************************************
                          StandCombatOrder.cpp
                             -------------------
    begin                : Thu Nov 16 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "StandCombatOrder.h"
#include "TokenEntity.h"
#include "BattleInstance.h"
#include "BattleField.h"
#include "CombatReport.h"
#include "CombatEngine.h"
#include "reporting.h"



StandCombatOrder * instantiateStandCombatOrder = new StandCombatOrder();

StandCombatOrder::StandCombatOrder(){
  keyword_ = "STAND";
  registerOrder_();
  description = string("STAND: The unit will not move at all, until destroyed.\n");
  orderType_   = COMBAT_MOVEMENT_ORDER;
  initiative_ = 0;
 isSequentive_ = true;
  mayInterrupt_ = true;
}

STATUS StandCombatOrder::loadParameters(Parser * parser,
                         ParameterList &parameters, Entity * entity )
{
   if(!entityIsTokenEntity(entity))
            return IO_ERROR;
   return OK;

}



ORDER_STATUS StandCombatOrder::process (Entity * entity, ParameterList &parameters)
{
	//Stand does nothing
	combatReportFile<<entity<<" stands."<<endl;

	return SUCCESS;
}

