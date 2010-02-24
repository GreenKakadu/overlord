/***************************************************************************
                          SummonCombatAction.cpp
                             -------------------
    begin                :
    copyright            : (C) 2005 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#include "SummonCombatAction.h"
#include "TokenEntity.h"
#include "RaceElement.h"
#include "CombatEngine.h"
#include "BattleInstance.h"
#include "BattleField.h"
#include "reporting.h"
#include "CombatAttackMessage.h"
#include "CombatReport.h"
#include "BattleEntity.h"
extern int Roll_1Dx(int n);
extern string longtostr(long u);
extern ReportPattern * noTargetsReporter;
extern ReportPattern * combatSummonReporter;

SummonCombatAction     sampleCombatSummonAction     ("COMBAT_SUMMON",  &sampleCombatAction);

GameData * SummonCombatAction::createInstanceOfSelf()
{
  return CREATE_INSTANCE<SummonCombatAction> (this);
}
SummonCombatAction::SummonCombatAction ( const SummonCombatAction * prototype ):
				 CombatActionStrategy(prototype)
{
	entity_ = 0;
	randomPlace_ = false;
}

			STATUS
SummonCombatAction::initialize        ( Parser *parser )
{
	CombatActionStrategy::initialize(parser);

  if (parser->matchKeyword ("SUMMONS") )
    {
			entity_ =  new BattleEntity(sampleBattleEntity);
 		 	RaceRule * race = races[ parser->getWord()];
			if(race)
			{
				entity_->setRace(race);
			int figures =  parser->getInteger();
			if(figures == 0)
				figures = 1;
				entity_->setFigures(figures);
			}
			else
				{
					cerr << "Summoning has no race defined"<<endl;
				}


     entity_->initialize(parser);
      return OK;
    }
   if (parser->matchKeyword ("PLACE") )
    {
			if(parser->matchKeyword ("random") )
			{
				randomPlace_ = true;
			}
      return OK;
    }
     return OK;

}





// Dummy. Summon doesn't use target
BattleTargets SummonCombatAction::getPotentialTargets(
									BattleInstance * battleInstance, CombatReport * )
{
  BattleTargets potentialTargets;
  potentialTargets.push_back(BattleTargetElement(battleInstance,SELF,0));
	return potentialTargets;
}



void		SummonCombatAction::performAction(BattleInstance * battleInstance, BattleTargets & potentialTargets, CombatReport * report)
{
	// create temporary BattleEntity for BattleInstance
  BattleField * battleField = battleInstance->getBattleField();
	entity_->recalculateStats();
	BattleEntity * summonedEntity = entity_->makeCopy();
	BattleInstance * summonedInstance = summonedEntity->createBattleInstantiation(battleField);

	int randomNum = Roll_1Dx(1000);
	string name = entity_->getRace()->getName() +  " (summoned by " + battleInstance->print() +")";
  summonedEntity->setName(name);

  summonedEntity->setTag(string("SS") + longtostr(randomNum));
  summonedEntity->preprocessData();
	combatReportFile<< " ==SS==  "<<battleInstance->print()<< " Summoning "
	<<entity_->getRace()->getName()	<<" "<<summonedEntity<<endl;
      report->add(new BinaryMessage(combatSummonReporter, battleInstance->getOrigin(),  new RaceElement(entity_->getRace(), entity_->getFigures()))); 
  battleField->getCombatEngine()->addSummonedEntity(battleInstance,summonedEntity);
	// now place it on field
	if(randomPlace_)
	{
	}
	else // default the same place as summoner
	{
   battleField->placeEntity(summonedEntity, battleInstance->getRank(), battleInstance->getFile());
	 // place on battlefield
	}
}


