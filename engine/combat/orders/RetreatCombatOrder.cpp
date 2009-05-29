/***************************************************************************
                          RetreatCombatOrder.cpp
                             -------------------
    begin                : Thu Nov 16 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "RetreatCombatOrder.h"
#include "TokenEntity.h"
#include "BattleInstance.h"
#include "BattleField.h"
#include "CombatReport.h"
#include "CombatEngine.h"
#include "reporting.h"



RetreatCombatOrder * instantiateRetreatCombatOrder = new RetreatCombatOrder();
ReportPattern * combatRetreatReporter= new ReportPattern("combatRetreatReporter");
//ReportPattern * combatParryReporter= new ReportPattern("combatParryReporter");

RetreatCombatOrder::RetreatCombatOrder(){
  keyword_ = "Retreat";
  registerOrder_();
  description = string("RETREAT:  \n") +
	"Essentially the same setting as FLEE, but the unit does not desert.\n" +
  "the battle.  It is the movement setting of choice for most mages.\n" ;
  orderType_   = COMBAT_MOVEMENT_ORDER;
  initiative_ = 0;
 isSequentive_ = true;
  mayInterrupt_ = true;
}



STATUS RetreatCombatOrder::loadParameters(Parser * parser,
                         ParameterList &parameters, Entity * entity )
{
   if(!entityIsTokenEntity(entity))
            return IO_ERROR;
   return OK;

}



ORDER_STATUS RetreatCombatOrder::process (Entity * entity, ParameterList &parameters)
{
 TokenEntity * unit = dynamic_cast<TokenEntity *>(entity);
  assert(unit);
  BattleInstance * instance = unit->getBattleInstantiation();
	BattleField * battleField = instance->getBattleField();
  int initiative = battleField->getCombatEngine()->getCurrentInitiativeSegment();

// If in contact with enemy try move away
    if(RetreatCombatOrder::retreatAway(instance))
		{
				return SUCCESS;
		}
// If not in contact with enemy
// Try move back without entering into contact
   bool isEnemyBehind = battleField->haveEnemies(instance,BACKWARD,2);
   bool isEnemyDiagonalLeft = battleField->haveEnemiesRelative(instance, -1, -1);
   bool isEnemyDiagonalRight = battleField->haveEnemiesRelative(instance, -1, +1);

   if( !isEnemyBehind && !isEnemyDiagonalLeft && !isEnemyDiagonalRight)
			 {
					if(battleField->moveEntity(unit,BACKWARD))
				{
 					new TertiaryMessage(combatRetreatReporter,
						new IntegerData(initiative), unit,
						new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return SUCCESS;
				}
			 }

// if there is enemy back - try to move left then right
   if( isEnemyBehind )
	 {
					if (RetreatCombatOrder::mayRetreatLeft(instance))
					{
						if(battleField->moveEntity(unit,LEFT))
						{
 							new TertiaryMessage(combatRetreatReporter,
								new IntegerData(initiative), unit,
								new StringData(battleField->printPosition(instance->getFile(),
			 				instance->getRank()))) >>*(battleField->getCombatReport()) ;
							return SUCCESS;
						}
					}
					if (RetreatCombatOrder::mayRetreatRight(instance))
					{
						if(battleField->moveEntity(unit,RIGHT))
						{
 							new TertiaryMessage(combatRetreatReporter,
								new IntegerData(initiative), unit,
								new StringData(battleField->printPosition(instance->getFile(),
			 				instance->getRank()))) >>*(battleField->getCombatReport()) ;
							return SUCCESS;
						}
					}
					else
					{
					// Stay on place. may advance counter
							return SUCCESS;
					}
	 }

// if there is enemy back by diagonal - try to move opposite direction
   if( isEnemyDiagonalLeft && !isEnemyDiagonalRight)
			 {
					if (RetreatCombatOrder::mayRetreatRight(instance))
					{
						if(battleField->moveEntity(unit,RIGHT))
						{
 							new TertiaryMessage(combatRetreatReporter,
								new IntegerData(initiative), unit,
								new StringData(battleField->printPosition(instance->getFile(),
			 				instance->getRank()))) >>*(battleField->getCombatReport()) ;
							return SUCCESS;
						}
					}
					else
					{
					// Stay on place. may advance counter
							return SUCCESS;
					}
			 	}
   if( isEnemyDiagonalRight  && !isEnemyDiagonalLeft)
			 {
					if (RetreatCombatOrder::mayRetreatRight(instance))
					{
						if(battleField->moveEntity(unit,LEFT))
						{
 							new TertiaryMessage(combatRetreatReporter,
								new IntegerData(initiative), unit,
								new StringData(battleField->printPosition(instance->getFile(),
			 				instance->getRank()))) >>*(battleField->getCombatReport()) ;
							return SUCCESS;
						}
					}
					else
					{
					// Stay on place. may advance counter
							return SUCCESS;
					}
			 }


// else not move (advance counter)
	return SUCCESS;
}


// If has enemies nearby tries to retreat  in OPPOSITE direction.
bool RetreatCombatOrder::retreatAway(BattleInstance * instance)
{

	BattleField * battleField = instance->getBattleField();
	TokenEntity * unit = instance->getOrigin();
  int initiative = battleField->getCombatEngine()->getCurrentInitiativeSegment();

	if(battleField->haveEnemies(instance,FORWARD))
	{

// =====  First try to retreat backward =======
	if (RetreatCombatOrder::mayRetreatBackward(instance))
	{
			if(battleField->moveEntity(unit,BACKWARD))
			{
 				new TertiaryMessage(combatRetreatReporter,
					new IntegerData(initiative), unit,
					new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return true;
			}
	}
	// Then left

	if (RetreatCombatOrder::mayRetreatLeft(instance))
	{
			if(battleField->moveEntity(unit,LEFT))
			{
 				new TertiaryMessage(combatRetreatReporter,
					new IntegerData(initiative), unit,
					new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return true;
			}
	}

	// Then Right
	if (RetreatCombatOrder::mayRetreatRight(instance))
	{
			if(battleField->moveEntity(unit,RIGHT))
			{
 				new TertiaryMessage(combatRetreatReporter,
					new IntegerData(initiative), unit,
					new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return true;
			}
	}
	return true;
	} // ==== End of retreat from Forward =====






// ============= Retreat from left
	if(battleField->haveEnemies(instance,LEFT))
	{
	// Right
	if (RetreatCombatOrder::mayRetreatRight(instance))
	{
			if(battleField->moveEntity(unit,RIGHT))
			{
 				new TertiaryMessage(combatRetreatReporter,
					new IntegerData(initiative), unit,
					new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return true;
			}
	}

	if (RetreatCombatOrder::mayRetreatBackward(instance))
	{
			if(battleField->moveEntity(unit,BACKWARD))
			{
 				new TertiaryMessage(combatRetreatReporter,
					new IntegerData(initiative), unit,
					new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return true;
			}
	}
	if (RetreatCombatOrder::mayRetreatForward(instance))
	{
			if(battleField->moveEntity(unit,FORWARD))
			{
 				new TertiaryMessage(combatRetreatReporter,
					new IntegerData(initiative), unit,
					new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return true;
			}
	}
		return true;
	}




//  ============= Retreat from right
	if(battleField->haveEnemies(instance,RIGHT))
	{
	if (RetreatCombatOrder::mayRetreatLeft(instance))
	{
			if(battleField->moveEntity(unit,LEFT))
			{
 				new TertiaryMessage(combatRetreatReporter,
					new IntegerData(initiative), unit,
					new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return true;
			}
	}



	if (RetreatCombatOrder::mayRetreatBackward(instance))
	{
			if(battleField->moveEntity(unit,BACKWARD))
			{
 				new TertiaryMessage(combatRetreatReporter,
					new IntegerData(initiative), unit,
					new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return true;
			}
	}

	if (RetreatCombatOrder::mayRetreatForward(instance))
	{
			if(battleField->moveEntity(unit,FORWARD))
			{
 				new TertiaryMessage(combatRetreatReporter,
					new IntegerData(initiative), unit,
					new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return true;
			}
	}
		return true;

	}



// =============== Retreat from behind
	if(battleField->haveEnemies(instance,BACKWARD))
	{
// =====   try to retreat forward =======
	if (RetreatCombatOrder::mayRetreatForward(instance))
	{
			if(battleField->moveEntity(unit,FORWARD))
			{
 				new TertiaryMessage(combatRetreatReporter,
					new IntegerData(initiative), unit,
					new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return true;
			}
	}
	// Then left

	if (RetreatCombatOrder::mayRetreatLeft(instance))
	{
			if(battleField->moveEntity(unit,LEFT))
			{
 				new TertiaryMessage(combatRetreatReporter,
					new IntegerData(initiative), unit,
					new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return true;
			}
	}

	// Then Right
	if (RetreatCombatOrder::mayRetreatRight(instance))
	{
			if(battleField->moveEntity(unit,RIGHT))
			{
 				new TertiaryMessage(combatRetreatReporter,
					new IntegerData(initiative), unit,
					new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return true;
			}
	}

	return true;
	} // ==== End of retreat from behind =====

		return false;

}


// If has enemies nearby tries to retreat back .
// Difference from RetreatAway is that if unit have enemies on flanks
// it will first try to retreat back and only then in opposite direction
// from enemy.
bool RetreatCombatOrder::retreatBack(BattleInstance * instance)
{

	BattleField * battleField = instance->getBattleField();
	TokenEntity * unit = instance->getOrigin();
  int initiative = battleField->getCombatEngine()->getCurrentInitiativeSegment();

	if(battleField->haveEnemies(instance,FORWARD))
	{

// =====  First try to retreat backward =======
	if (RetreatCombatOrder::mayRetreatBackward(instance))
	{
			if(battleField->moveEntity(unit,BACKWARD))
			{
 				new TertiaryMessage(combatRetreatReporter,
					new IntegerData(initiative), unit,
					new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return true;
			}
	}
	// Then left

	if (RetreatCombatOrder::mayRetreatLeft(instance))
	{
			if(battleField->moveEntity(unit,LEFT))
			{
 				new TertiaryMessage(combatRetreatReporter,
					new IntegerData(initiative), unit,
					new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return true;
			}
	}

	// Then Right
	if (RetreatCombatOrder::mayRetreatRight(instance))
	{
			if(battleField->moveEntity(unit,RIGHT))
			{
 				new TertiaryMessage(combatRetreatReporter,
					new IntegerData(initiative), unit,
					new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return true;
			}
	}
	return false;
	} // ==== End of retreat from Forward =====






// ============= Retreat from left
	if(battleField->haveEnemies(instance,LEFT))
	{
	if (RetreatCombatOrder::mayRetreatBackward(instance))
	{
			if(battleField->moveEntity(unit,BACKWARD))
			{
 				new TertiaryMessage(combatRetreatReporter,
					new IntegerData(initiative), unit,
					new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return true;
			}
	}
	// Right
	if (RetreatCombatOrder::mayRetreatRight(instance))
	{
			if(battleField->moveEntity(unit,RIGHT))
			{
 				new TertiaryMessage(combatRetreatReporter,
					new IntegerData(initiative), unit,
					new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return true;
			}
	}

	if (RetreatCombatOrder::mayRetreatForward(instance))
	{
			if(battleField->moveEntity(unit,FORWARD))
			{
 				new TertiaryMessage(combatRetreatReporter,
					new IntegerData(initiative), unit,
					new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return true;
			}
	}
		return false;
	}




//  ============= Retreat from right
	if(battleField->haveEnemies(instance,RIGHT))
	{

	if (RetreatCombatOrder::mayRetreatBackward(instance))
	{
			if(battleField->moveEntity(unit,BACKWARD))
			{
 				new TertiaryMessage(combatRetreatReporter,
					new IntegerData(initiative), unit,
					new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return true;
			}
	}
	if (RetreatCombatOrder::mayRetreatLeft(instance))
	{
			if(battleField->moveEntity(unit,LEFT))
			{
 				new TertiaryMessage(combatRetreatReporter,
					new IntegerData(initiative), unit,
					new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return true;
			}
	}




	if (RetreatCombatOrder::mayRetreatForward(instance))
	{
			if(battleField->moveEntity(unit,FORWARD))
			{
 				new TertiaryMessage(combatRetreatReporter,
					new IntegerData(initiative), unit,
					new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return true;
			}
	}
		return false;

	}



// =============== Retreat from behind
	if(battleField->haveEnemies(instance,BACKWARD))
	{
	// Then left

	if (RetreatCombatOrder::mayRetreatLeft(instance))
	{
			if(battleField->moveEntity(unit,LEFT))
			{
 				new TertiaryMessage(combatRetreatReporter,
					new IntegerData(initiative), unit,
					new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return true;
			}
	}

	// Then Right
	if (RetreatCombatOrder::mayRetreatRight(instance))
	{
			if(battleField->moveEntity(unit,RIGHT))
			{
 				new TertiaryMessage(combatRetreatReporter,
					new IntegerData(initiative), unit,
					new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return true;
			}
	}
// =====   try to retreat forward =======
	if (RetreatCombatOrder::mayRetreatForward(instance))
	{
			if(battleField->moveEntity(unit,FORWARD))
			{
 				new TertiaryMessage(combatRetreatReporter,
					new IntegerData(initiative), unit,
					new StringData(battleField->printPosition(instance->getFile(),
			 		instance->getRank()))) >>*(battleField->getCombatReport()) ;
				return true;
			}
	}

	return false;
	} // ==== End of retreat from behind =====

		return false;

}



// try to retreat forward
bool RetreatCombatOrder::mayRetreatForward(BattleInstance * instance)
{
	BattleField * battleField = instance->getBattleField();

		if(battleField->haveEnemies(instance,FORWARD))
			{
				return false;
			}
		if(!battleField->isBattlefieldRelative(instance,+1,0))
			{
				return false;
			}
				//check enemies around new place
    if(battleField->haveEnemies(instance,FORWARD,2) ||
       battleField->haveEnemiesRelative(instance,+1,+1) ||
       battleField->haveEnemiesRelative(instance,+1,-1)		)
			{
				return false;
			}

	return true;
}



// try to retreat backward
bool RetreatCombatOrder::mayRetreatLeft(BattleInstance * instance)
{
	BattleField * battleField = instance->getBattleField();

		if(battleField->haveEnemies(instance,LEFT))
			{
				return false;
			}
		if(!battleField->isBattlefieldRelative(instance, 0, -1))
			{
				return false;
			}
				//check enemies around new place
    if(battleField->haveEnemies(instance,LEFT,2) ||
       battleField->haveEnemiesRelative(instance, -1, -1) ||
       battleField->haveEnemiesRelative(instance, +1, -1)		)
			{
				return false;
			}

	return true;
}



// try to retreat backward
bool RetreatCombatOrder::mayRetreatRight(BattleInstance * instance)
{
	BattleField * battleField = instance->getBattleField();

		if(battleField->haveEnemies(instance, RIGHT))
			{
				return false;
			}
		if(!battleField->isBattlefieldRelative(instance, 0, +1))
			{
				return false;
			}
				//check enemies around new place
    if(battleField->haveEnemies(instance, RIGHT, 2) ||
       battleField->haveEnemiesRelative(instance, -1, +1) ||
       battleField->haveEnemiesRelative(instance, +1, +1)		)
			{
				return false;
			}
	return true;
}

// try to retreat backward
bool RetreatCombatOrder::mayRetreatBackward(BattleInstance * instance)
{
	BattleField * battleField = instance->getBattleField();

		if(battleField->haveEnemies(instance,BACKWARD))
			{
				return false;
			}
		if(!battleField->isBattlefieldRelative(instance, -1, 0))
			{
				return false;
			}
				//check enemies around new place
    if(battleField->haveEnemies(instance,BACKWARD,2) ||
       battleField->haveEnemiesRelative(instance,-1,+1) ||
       battleField->haveEnemiesRelative(instance,-1,-1)		)
			{
				return false;
			}

	return true;
}





