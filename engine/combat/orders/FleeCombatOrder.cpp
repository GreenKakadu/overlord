/***************************************************************************
                          FleeCombatOrder.cpp
                             -------------------
    begin                : Thu Nov 09 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "FleeCombatOrder.h"
#include "RetreatCombatOrder.h"
#include "TokenEntity.h"
#include "BattleInstance.h"
#include "BattleField.h"
#include "CombatReport.h"
#include "CombatEngine.h"
#include "reporting.h"
FleeCombatOrder * instantiateFleeCombatOrder = new FleeCombatOrder();
extern ReportPattern * combatRetreatReporter;

FleeCombatOrder::FleeCombatOrder(){
  keyword_ = "Flee";
  registerOrder_();
  description = string("FLEE:  \n") +
  "The unit tries to move to the rear rank on its side.  Once at the rear\n" +
  "rank, it will try to move to the flank that is less exposed to opposing\n" +
  "forces. If the unit manages to stay three rounds at the rear rank without\n" +
  "having to move or to face a melee attack, the unit flees the battlefield,\n" +
  "and is removed from the battle.  If the unit's side loses the battle, the\n" +
  "unit will have a 10% chance per round since it departed to avoid the\n" +
	"retreat round\n";
  orderType_   = COMBAT_MOVEMENT_ORDER;
  initiative_ = 0;
 isSequentive_ = true;
  mayInterrupt_ = true;
}

STATUS FleeCombatOrder::loadParameters(Parser * parser,
                         ParameterList &parameters, Entity * entity )
{
   if(!entityIsTokenEntity(entity))
            return IO_ERROR;
   return OK;

}



ORDER_STATUS FleeCombatOrder::process (Entity * entity, ParameterList &parameters)
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
                                          instance->setFleing(true);

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
                                          instance->setFleing(true);

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
                                          instance->setFleing(true);

							return SUCCESS;
					}
			 }


// else not move (advance counter)
                         instance->setFleing(true);


		return FAILURE;
}

