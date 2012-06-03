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
#include "CombatOrderLine.h"

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
SummonCombatAction::initialize(Parser *parser)
{
    CombatActionStrategy::initialize(parser);

    if (parser->matchKeyword("SUMMONS"))
    {
        entity_ = new BattleEntity(sampleBattleEntity);
        RaceRule * race = gameFacade->races[ parser->getWord()];
        if (race)
        {
            int figures = parser->getInteger();
            if (figures == 0)
                figures = 1;
            //entity_->setFigures(figures);
             entity_->setRace(race,figures);
       } else
        {
            cerr << "Summoning has no race defined" << endl;
        }



        return OK;
    }
    if(entity_)
    {
          entity_->initialize(parser);
    }
    //Prefix to provide initialization for summoned entity
    if (parser->matchKeyword("SUMMONED"))
    {
        entity_->initialize(parser);
        return OK;
    }
    if (parser->matchKeyword("PLACE"))
    {
        if (parser->matchKeyword("random"))
        {
            randomPlace_ = true;
        }
        return OK;
    }
    if(entity_)
    {
      if (parser->matchKeyword("COMBAT_ORDER"))
      {
        entity_->addOrder("COMBAT_ORDER " + parser->getText());
      }
    }
   return OK;

}

void SummonCombatAction::save(ostream &out)
{
    CombatActionStrategy::save(out);

    if(entity_)
    {
        out<<"SUMMONS"<<" "<<entity_->getRace()->getTag()<<" "<<entity_->getFiguresNumber() <<endl;
        entity_->save(out,"SUMMONED ");
    }

    if(randomPlace_)out<<"COMBAT "<< "PLACE"<<" "<<"random" <<endl;
//    for(vector<CombatOrderLine*>::iterator iter =  (entity_->getCombatOrderList()).begin();
//            iter != (entity_->getCombatOrderList()).end(); ++iter )
//    {
//       out<< "COMBAT_ORDER"<<" ";
//       (*iter)->save(out);
//    }


}





// Dummy. Summon doesn't use target
BattleTargets SummonCombatAction::getPotentialTargets(
									BattleInstance * battleInstance, CombatReport * )
{
  BattleTargets potentialTargets;
  potentialTargets.push_back(BattleTargetElement(battleInstance,SELF,0));
	return potentialTargets;
}

void SummonCombatAction::performAction(BattleInstance * battleInstance, BattleTargets & potentialTargets, CombatReport * report)
{
    // create temporary BattleEntity for BattleInstance
    BattleField * battleField = battleInstance->getBattleField();
    //cout<<"===== Summoning performAction ======="<<endl;
    //entity_->UnitEntity::privateReport(cout);
//    entity_->recalculateStats();
    entity_->setCombatSettings();
    string name = entity_->getName() + " (summoned by " + battleInstance->print() + ")";
    BattleEntity * summonedEntity = entity_->makeCopy();
    summonedEntity->setLocation(battleInstance->getOrigin()->getLocation());
    summonedEntity->setName(name);
     int randomNum = Roll_1Dx(1000);
   summonedEntity->setTag(string("SS") + longtostr(randomNum));
    combatReportFile << " ==SS==  " << battleInstance->print() << " Summoning "<< entity_->getFiguresNumber()<<" "
            << entity_->getRace()->getName() << " " << summonedEntity << endl;
    if(battleInstance->getOrigin()->isTraced())
    {
    summonedEntity->setTraced(true);
    }

//    for(vector<string>::iterator iter = combatOrders_.begin(); iter != combatOrders_.end(); ++iter)
//    {
//        summonedEntity->addOrder(*iter);
////        summonedEntity->addCombatSetting(*iter);
////        combatReportFile <<"Order: " <<(*iter)<<endl;
//    }
    summonedEntity->setCombatSettings();
    report->add(new BinaryMessage(combatSummonReporter, battleInstance->getOrigin(), new RaceElement(entity_->getRace(), entity_->getFiguresNumber())));
     BattleInstance * summonedInstance = summonedEntity->createBattleInstantiation(battleField);
     summonedInstance->recalculateStats();
//     if(summonedEntity->isTraced())
//     {
//       combatReportFile << " ==SS==Num:  "<<summonedEntity->getFiguresNumber() <<" -> "<< summonedInstance->getFiguresNumber()<<endl;
//     }
     battleField->getCombatEngine()->addSummonedEntity(battleInstance, summonedEntity);

    // now place it on field
    if (randomPlace_)
    {
    } else // default the same place as summoner
    {
        battleField->placeEntity(summonedEntity, battleInstance->getRank(), battleInstance->getFile());
        // place on battlefield
    }
    //cout<<"===$$===> Summon: "<<summonedEntity->print()<<" Instance: "<<summonedEntity->getBattleInstantiation()<<endl;
}

void    SummonCombatAction::extractKnowledge (Entity * recipient, int parameter)
{
   CombatActionStrategy::extractKnowledge(recipient,parameter);
  if(entity_)
  {
      entity_->extractAndAddKnowledge(recipient,parameter);
  }
}
