/***************************************************************************
                          ChargeCombatOrder.cpp
                             -------------------
    begin                : Thu Nov 09 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "ChargeCombatOrder.h"
#include "TokenEntity.h"
#include "BattleInstance.h"
#include "BattleField.h"
#include "CombatReport.h"
#include "CombatEngine.h"
#include "reporting.h"


//extern ReportPattern *	invalidParameterReporter;
//extern ReportPattern *	missingParameterReporter;

ChargeCombatOrder * instantiateChargeCombatOrder = new ChargeCombatOrder();
ReportPattern * combatChargeReporter= new ReportPattern("combatChargeReporter");

ChargeCombatOrder::ChargeCombatOrder(){
  keyword_ = "Charge";
  registerOrder_();
  description = string("CHARGE:  \n") +
 "The unit will advance toward the nearest opposing unit.\n";
  orderType_   = COMBAT_MOVEMENT_ORDER;
  initiative_ = 0;
 isSequentive_ = true;
  mayInterrupt_ = true;
}

STATUS ChargeCombatOrder::loadParameters(Parser * parser,
                         ParameterList &parameters, Entity * entity )
{
   if(!entityIsTokenEntity(entity))
            return IO_ERROR;
   return OK;

}



ORDER_STATUS ChargeCombatOrder::process (Entity * entity, ParameterList &parameters)
{
 TokenEntity * unit = dynamic_cast<TokenEntity *>(entity);
  assert(unit);
  BattleInstance * instance = unit->getBattleInstantiation();
	BattleField * battleField = instance->getBattleField();
			instance->setMovedOnRound(true);
  int initiative = battleField->getCombatEngine()->getCurrentInitiativeSegment();

// scan battlefield for closest enemy
// prefer front, then left-right-behind

	int rank0 = instance->getRank();
	int file0 = instance->getFile();
	int range;
	int currentRange = 0;
	BATTLE_DIRECTION currentDirection = FORWARD;
	BATTLE_DIRECTION direction = FORWARD;
	for(int file = battleField->getMinFile(); file <= battleField->getMaxFile();
			 ++file)
		for(int rank= battleField->getMinRank(); rank <= battleField->getMaxRank();
			 ++rank)
		{
			 if(battleField->haveEnemiesRelative(instance,rank - rank0, file - file0))
					{
			 			range = abs(rank0 - rank) + abs(file0 - file);
						if(currentRange == 0)// first enemy found
							{
								currentRange = range;
								currentDirection =
									 battleField->getDirection(instance,rank,file);
									continue;
							}
							else
							{
								if(currentRange < range)
								{
									continue;
								}
								if(currentRange > range)
								{
								currentRange = range;
								currentDirection =
									 battleField->getDirection(instance,rank,file);
									continue;
								}
								else //currentRange ==range
								{
									direction = battleField->getDirection(instance,rank,file);
									if((currentDirection == FORWARD) ||( direction == FORWARD))
									{
										currentDirection = FORWARD;
										continue;
									}
									if((currentDirection == LEFT) ||( direction == LEFT))
									{
										currentDirection = LEFT;
										continue;
									}
									if((currentDirection == RIGHT) ||( direction == RIGHT))
									{
										currentDirection = RIGHT;
										continue;
									}
									if((currentDirection == BACKWARD) ||( direction == BACKWARD))
									{
										currentDirection = BACKWARD;
										continue;
									}
									cerr << "Unknown direction " << direction << " for " <<
									 instance->getOrigin()->print() <<endl;
								}
							}
					}


		}



//----------------------


	if(!battleField->haveEnemies(instance,currentDirection))
	{
	    combatReportFile<<entity<<" charges from ("<<instance->getRank() << ","<<instance->getFile()<<")";

			if (battleField->moveEntity(unit,currentDirection))
			{

	    	combatReportFile<<" to ("<<instance->getRank() <<
				 ","<<instance->getFile()<<")"<<endl;
			// Report
				new TertiaryMessage(combatChargeReporter,
					new IntegerData(initiative), entity,
					new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return SUCCESS;
			}
	    combatReportFile<<entity<<" but the battlefield is over."<<endl;
	}
	else
	    combatReportFile<<entity<<" can't charge."<<endl;
	return SUCCESS;

}

