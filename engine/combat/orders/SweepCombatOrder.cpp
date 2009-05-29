/***************************************************************************
                          SweepCombatOrder.cpp
                             -------------------
    begin                : Thu Nov 16 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "SweepCombatOrder.h"
#include "TokenEntity.h"
#include "BattleInstance.h"
#include "BattleField.h"
#include "CombatReport.h"
#include "CombatEngine.h"
#include "reporting.h"



SweepCombatOrder * instantiateSweepCombatOrder = new SweepCombatOrder();
extern ReportPattern * combatAdvanceReporter;
//ReportPattern * combatParryReporter= new ReportPattern("combatParryReporter");

SweepCombatOrder::SweepCombatOrder(){
  keyword_ = "Sweep";
  registerOrder_();
  description = string("SWEEP:  \n") +
	"Essentially the same setting as ADVANCE, except that the unit will not\n" +
  "move if there is an opposing unit on the same rank and in an adjacent file.\n";
  orderType_   = COMBAT_MOVEMENT_ORDER;
  initiative_ = 0;
 isSequentive_ = true;
  mayInterrupt_ = true;
}

STATUS SweepCombatOrder::loadParameters(Parser * parser,
                         ParameterList &parameters, Entity * entity )
{
   if(!entityIsTokenEntity(entity))
            return IO_ERROR;
   return OK;

}



ORDER_STATUS SweepCombatOrder::process (Entity * entity, ParameterList &parameters)
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

  BATTLE_DIRECTION direction = sweepDirection(instance);


	if(!battleField->haveEnemies(instance,direction))
	{
	    combatReportFile<<entity<<" sweep moving  from ("<<instance->getRank() << ","<<instance->getFile()<<")";

			if(battleField->moveEntity(unit,direction))
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
	    combatReportFile<<entity<<" can't advance(sweep)."<<endl;
	return SUCCESS;

}

BATTLE_DIRECTION SweepCombatOrder::sweepDirection(BattleInstance * battleInstance)
{
	BATTLE_DIRECTION decreaseDirection;
	BATTLE_DIRECTION increaseDirection;
	int rank =battleInstance->getRank();
	int originFile =battleInstance->getFile();
	BattleField * battleField = battleInstance->getBattleField();


	if(battleInstance->isAttacker())
		{
			decreaseDirection = LEFT;
			increaseDirection = RIGHT;
		}
	else
	{
			increaseDirection = LEFT;
			decreaseDirection = RIGHT;
	}

	int minFile = battleField->getMinFile();
	int maxFile = battleField->getMaxFile();

	if (originFile < minFile + (maxFile - minFile)/2)
	{// Check decrease direction
		for(int currentFile = minFile; currentFile < originFile; currentFile++)
		{
  		if((battleField->getUnits(currentFile, rank)).empty())
				continue;

			if( (*(battleField->getUnits(currentFile, rank)).begin())->
					getBattleInstantiation()->isAttacker() !=	 battleInstance->isAttacker())
				{
				return decreaseDirection;
				}
		}
	}
	else// Check increase direction
	{
		for(int currentFile = originFile + 1; currentFile < maxFile; currentFile++)
		{
  		if(battleField->getUnits(currentFile, rank).empty())
				continue;

			if( (*(battleField->getUnits(currentFile, rank)).begin())->
					getBattleInstantiation()->isAttacker() !=	 battleInstance->isAttacker())
				{
				return decreaseDirection;
				}
		}
	}

			return FORWARD;
}



