/***************************************************************************
                      CombatEngine.cpp
               Standard Engine for processing  combat
							 Manages initialization of combat data
							 sequence of orders
							 detection of combat end
							 managing post-combat action
                          ------------------
    begin                : Mon Oct 26 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include <fstream>
#include "CombatEngine.h"
#include "TokenEntity.h"
#include "BattleEntity.h"
#include "LocationEntity.h"
#include "FactionEntity.h"
#include "BattleField.h"
#include "ProcessingMode.h"
#include "CombatOrderProcessor.h"
#include "CombatManager.h"
#include "CombatReport.h"
#include "reporting.h"
#include "CombatParticipantMessage.h"
using namespace std;
extern int currentDay;
extern ReportPattern * combatStart2Reporter;
extern ReportPattern * combatEndReporter;
extern ReportPattern * combatAttackersReporter;
extern ReportPattern * combatDefendersReporter;
extern ReportPattern * listReporter;

ProcessingMode  combatActionOrders(COMBAT_ACTION_ORDER);
ProcessingMode  combatMovementOrders(COMBAT_MOVEMENT_ORDER);
string combatName("combat_");
ofstream combatReportFile;
extern string longtostr(long u);


ReportPattern * combatRoundReporter= new ReportPattern("combatRoundReporter");
CombatEngine realCombatEngine;
ReportPattern * combatAdvantageReporter = new ReportPattern("combatAdvantageReporter");
ReportPattern * combatTacticsReporter = new ReportPattern("combatTacticsReporter");
ReportPattern * combatStrategyReporter = new ReportPattern("combatStrategyReporter");
ReportPattern * combatAmbushReporter = new ReportPattern("combatAmbushReporter");
ReportPattern * attackerLossReporter = new ReportPattern("attackerLossReporter");
ReportPattern * defenderLossReporter = new ReportPattern("defenderLossReporter");
ReportPattern * combatAttackersVictoryReporter = new ReportPattern("combatAttackersVictoryReporter");
ReportPattern * combatDefendersVictoryReporter = new ReportPattern("combatDefendersVictoryReporter");
ReportPattern * combatDrawReporter = new ReportPattern("combatDrawReporter");


CombatEngine::CombatEngine() : BasicCombatEngine()
{
cout << "===> CombatEngine created " <<report_<<endl;
		 maxFile_ = 3;
		 minFile_ = 0;
		 maxRank_ = 3;
		 minRank_ = -3;
		 battleField_ = 0;
		 currentInitiativeSegment_ = 0;
		 result_ = DRAW;
		 attackers_ = 0;
		 defenders_ = 0;
		 attackerLossCount_ =0;
		 defenderLossCount_ =0;
		 attackersCount_ =0;
		 defendersCount_ =0;
		 woundlessRoundCounter_ = 0;
		 attackerMoralCheckCount_ = 0;
		 defenderMoralCheckCount_ = 0;
		 attackersAliveCount_ = 0;
		 defendersAliveCount_ = 0;
		 attackerStrategy_ = 0;
		 defenderStrategy_ = 0;
		 attackerAmbush_ = 0;
		 defenderAmbush_ = 0;
		 attackerTactics_ = 0;
		 defenderTactics_ = 0;
		 attackerObservation_ = 0;
		 defenderObservation_ = 0;
  	 initMin_ = 0;
  	 initMax_ = 0;
}


CombatEngine::~CombatEngine()
{
	deadCombatants_.clear();
}


BATTLE_RESULT CombatEngine::processBattle(vector< TokenEntity * >& attackers, vector< TokenEntity * >& defenders)
{
	report_ = new CombatReport();
	allReports_.push_back(report_);
cout << "===> New combat created using CombatEngine" <<endl;
//	BattleInstance * newInstance = 0;
	battleField_ = new BattleField(this);
  location_ = (*(defenders.begin()))->getLocation();
	if(location_ == 0)
		{
			cout << "Error with determining battle location for"<< *(*(defenders.begin()))<<endl;
			return DRAW;
		}
	battleField_->setName(location_->print());
  attackers_ = &attackers;
	defenders_ = &defenders;
  vector<TokenEntity *>::iterator iter;


// ==== For debugging: open file for printing debugging info
cout << "Combat on day "<<currentDay<<" at " << location_<<endl;
 string combatReportFileName = combatName + longtostr(BasicCombatEngine::battleId) + ".crep";
 combatReportFile.open(combatReportFileName.c_str());
 combatReportFile<< "Combat on day "<<currentDay<<" at " << location_<<endl;
 combatReportFile<< "Attackers: "<<endl;
// ==== End of debugging info

 new BinaryMessage(combatStart2Reporter,new IntegerData(currentDay),location_) >>*report_ ;
 new SimpleMessage(combatAttackersReporter) >>*report_ ;
 BattleInstance * currentInstance = 0;
// Create BattleInstances
 for(iter = attackers.begin(); iter != attackers.end(); ++iter )
  {
		currentInstance = (*iter)->createBattleInstantiation(battleField_);
    currentInstance->setAttacker();// mark as attackers
	  currentInstance->setSideEnchantment(&attackerSideEnchantments_);
   battleField_->placeEntity(*iter,0,0);// place on battlefield

	 (*iter)->setFullDayOrderFlag();// If combat happened
		                               // no more full day orders possible
	// Report
	 combatReportFile <<"      "<< (*iter) << " placed at (" <<
		   currentInstance->getFile()<<"," <<
			 currentInstance->getRank()<<") i.e. "
			 << battleField_->printPosition(
			  currentInstance->getFile(),
			 currentInstance->getRank())<<endl;

		new CombatParticipantMessage(*iter) >>*report_ ;
		(*iter)->getFaction()->addCombatReport(report_);
	}
    attackerObservation_ = calculateSideObservation(attackers_);
    attackerAmbush_ = calculateSideAmbush(attackers_);
		attackerStrategy_ = calculateSideStrategy(attackers_);
		attackerTactics_ = calculateSideTactics(attackers_);

	if(attackerStrategy_)
 	new UnaryMessage(combatStrategyReporter, new IntegerData(attackerStrategy_))
	 >>*report_ ;
	if(attackerTactics_)
 	new UnaryMessage(combatTacticsReporter, new IntegerData(attackerTactics_))
	 >>*report_ ;
	if(attackerAmbush_)
 	new UnaryMessage(combatAmbushReporter, new IntegerData(attackerAmbush_))
	 >>*report_ ;
 endLineMessage >>*report_ ;

// =====================================================================
 new SimpleMessage(combatDefendersReporter) >>*report_ ;

 combatReportFile<<endl<< "Defenders: "<<endl;
 for(iter = defenders.begin(); iter != defenders.end(); ++iter )
  {
		currentInstance = (*iter)->createBattleInstantiation(battleField_);
    currentInstance->setDefender();// mark as defender
	  currentInstance->setSideEnchantment(&defenderSideEnchantments_);
   battleField_->placeEntity(*iter,0,0);// place on battlefield

	 (*iter)->setFullDayOrderFlag();// If combat happened
		                               // no more full day orders possible
	 combatReportFile <<"      "<< (*iter) << " placed at (" <<
	  currentInstance->getFile()<<"," <<
		 currentInstance->getRank()<<")"<<endl;

		new CombatParticipantMessage(*iter) >>*report_ ;
		(*iter)->getFaction()->addCombatReport(report_);
  }

    defenderObservation_ = calculateSideObservation(defenders_);
		defenderStrategy_ = calculateSideStrategy(defenders_);
		defenderAmbush_ = calculateSideAmbush(defenders_);
		defenderTactics_ = calculateSideTactics(defenders_);

	if(defenderStrategy_)
 	new UnaryMessage(combatStrategyReporter, new IntegerData(defenderStrategy_))
	>>*report_ ;
	if(defenderTactics_)
 	new UnaryMessage(combatTacticsReporter, new IntegerData(defenderTactics_))
	>>*report_ ;
	if(defenderAmbush_)
 	new UnaryMessage(combatAmbushReporter, new IntegerData(defenderAmbush_))
	>>*report_ ;
 endLineMessage >>*report_ ;

	 preProcess();
		int maxRoundNum = 10;
//		int maxRoundNum = 5;
		int N = 1;
	 while (N <= maxRoundNum)
	 {
		preProcessRound(N);
	 	processRound(N);
		if(isBattleEnd())
			break;
		else
			postProcessRound(N);
			N++;
	 }

	 processRoutingRound();

	 if(N > maxRoundNum) // normal exit from the loop
	 {
   	new SimpleMessage(combatDrawReporter) >>*report_ ;
	 }
	postProcess();

 new SimpleMessage(combatEndReporter) >>*report_ ;
 endLineMessage >>*report_ ;
 combatReportFile.close();
 delete battleField_;
	BasicCombatEngine::battleId++;
		return result_;
}



void CombatEngine::preProcess()
{
		 result_ = DRAW;
		 attackerLossCount_ =0;
		 defenderLossCount_ =0;
		 attackersCount_ =0;
		 defendersCount_ =0;
		 woundlessRoundCounter_ = 0;
		 attackerMoralCheckCount_ = 0;
		 defenderMoralCheckCount_ = 0;
		 attackersAliveCount_ = 0;
		 defendersAliveCount_ = 0;
	// determine best tactics
		if(attackerStrategy_ >defenderStrategy_)
			{
				attackerStrategy_ -= defenderStrategy_;
				defenderStrategy_ = 0;
			}
		else
		{
			defenderStrategy_ -= attackerStrategy_;
			attackerStrategy_ = 0;
		}

	// report tactics, strategy, Ambush
	 combatReportFile <<"      Attacker:"<<endl;
	 combatReportFile <<"                Strategy "<< attackerStrategy_ <<endl;
	 combatReportFile <<"                Tactics "<< attackerTactics_ <<endl;
	 combatReportFile <<"                Ambush "<< attackerAmbush_ <<endl;
	 combatReportFile <<"      Defender:"<<endl;
	 combatReportFile <<"                Strategy "<< defenderStrategy_ <<endl;
	 combatReportFile <<"                Tactics "<< defenderTactics_ <<endl;
	 combatReportFile <<"                Ambush "<< defenderAmbush_ <<endl;

	// Determine ambush bonus

	if(attackerAmbush_ >= defenderAmbush_ + defenderObservation_)
	{
		attackerAmbush_ -= (defenderAmbush_ + defenderObservation_);
		defenderAmbush_ = 0;
	}
	else
	{
		attackerAmbush_  /= 2;
		defenderAmbush_  /= 2;
		if(defenderAmbush_ > attackerAmbush_ + attackerObservation_)
		{
			attackerAmbush_ = 0;
			defenderAmbush_ -= (attackerAmbush_ + attackerObservation_);
		}
		else
		{
		attackerAmbush_  = 0;
		defenderAmbush_  = 0;
		}
	}
	// Shuffle
	if(attackerAmbush_)
	{
	}



	if(attackerStrategy_)
				new TertiaryMessage(combatAdvantageReporter,
				new StringData("Attacker "), new StringData("strategical "),
				 new IntegerData(attackerStrategy_)) >>*report_ ;
	if(defenderStrategy_)
				new TertiaryMessage(combatAdvantageReporter,
					new StringData("Defender "), new StringData("strategical "),
					 new IntegerData(defenderStrategy_)) >>*report_ ;

	if(attackerTactics_)
				new TertiaryMessage(combatAdvantageReporter,
				new StringData("Attacker "), new StringData("tactical "),
				 new IntegerData(attackerTactics_)) >>*report_ ;
	if(defenderTactics_)
				new TertiaryMessage(combatAdvantageReporter,
					new StringData("Defender "), new StringData("tactical "),
					 new IntegerData(defenderTactics_)) >>*report_ ;

	if(attackerAmbush_)
				new TertiaryMessage(combatAdvantageReporter,
				new StringData("Attacker "), new StringData("ambush "),
				 new IntegerData(attackerAmbush_)) >>*report_ ;
	if(defenderAmbush_)
				new TertiaryMessage(combatAdvantageReporter,
					new StringData("Defender "), new StringData("ambush "),
					 new IntegerData(defenderAmbush_)) >>*report_ ;



		for(BattleIterator iter = attackers_->begin();
	    iter != attackers_->end(); ++iter )
  	{
			attackersCount_ += (*iter)->getBattleInstantiation()->getFiguresNumber();
		}
  	for(BattleIterator iter = defenders_->begin();
	     iter != defenders_->end(); ++iter )
  	{
			defendersCount_ += (*iter)->getBattleInstantiation()->getFiguresNumber();
  	}
	attackersAliveCount_ = attackersCount_;
	defendersAliveCount_ = defendersCount_;
  combatReportFile << "Attackers "<< attackersCount_ << " figures. ";
  combatReportFile << "Defenders "<< defendersCount_ << " figures."<<endl;
}



// set round counters
// calculate current initiative bonuces
// determine initiative interval
// plan round orders
void CombatEngine::preProcessRound(int round)
{
// int initMin_ = ProcessingMode::MIN_INITIATIVE;
// int initMax_ = ProcessingMode::MAX_INITIATIVE;
 initMin_ = 0;
 initMax_ = 0;
 woundlessRoundCounter_++;
 new UnaryMessage(combatRoundReporter,new IntegerData(round)) >>*report_ ;


 /*combatReportFile*/ cout << "------ Round " << round <<"-------"<<endl;
 combatReportFile << "------ Round " << round<<"-------" <<endl;
// Determine tactics bonus
	int currentAttackerTactics = calculateAttackerInitiative(round);
	int currentDefenderTactics = calculateDefenderInitiative(round);
//
		for(BattleIterator iter = attackers_->begin();
	    iter != attackers_->end(); ++iter )
  	{
				if(!(*iter)->getBattleInstantiation()->isAlive())
					continue;
				if((*iter)->getBattleInstantiation()->isFled())
					continue;
				(*iter)->getBattleInstantiation()->recheckOrders();
        (*iter)->getBattleInstantiation()->addCombatExperience(1);

		}
  	for(BattleIterator iter = defenders_->begin();
	     iter != defenders_->end(); ++iter )
  	{
				if(!(*iter)->getBattleInstantiation()->isAlive())
					continue;
				if((*iter)->getBattleInstantiation()->isFled())
					continue;
				(*iter)->getBattleInstantiation()->recheckOrders();
        (*iter)->getBattleInstantiation()->addCombatExperience(1);
  	}

// /*combatReportFile*/ cout << " recalculation initiative for unit is done"  <<endl;

//     combatReportFile << "    Initiative calculation - Before: initMax= " << initMax<<endl;
// =====================================================================
// determine min and max initiative
// and plan round orders
// Clear flags
		for(BattleIterator iter = attackers_->begin();
	    iter != attackers_->end(); ++iter )
  	{
//			cout << "Examine "<< (*iter);
				if(!(*iter)->getBattleInstantiation()->isAlive())
					continue;
				if((*iter)->getBattleInstantiation()->isFled())
					continue;
			(*iter)->getBattleInstantiation()->
				reCalculateInitiative(currentAttackerTactics, initMin_, initMax_);
				(*iter)->getBattleInstantiation()->clearRoundFlags();
				(*iter)->getBattleInstantiation()->planRoundOrders();
		}
  	for(BattleIterator iter = defenders_->begin();
	     iter != defenders_->end(); ++iter )
  	{
				if(!(*iter)->getBattleInstantiation()->isAlive())
					continue;
				if((*iter)->getBattleInstantiation()->isFled())
					continue;
			(*iter)->getBattleInstantiation()->
				reCalculateInitiative(currentDefenderTactics, initMin_, initMax_);
				(*iter)->getBattleInstantiation()->clearRoundFlags();
				(*iter)->getBattleInstantiation()->planRoundOrders();
  	}

		 combatReportFile<< "    Initiative calculation: initMin= " <<initMin_
		 <<" initMax= "<< initMax_<<endl;

}



// =====================================================================
// Process orders
void CombatEngine::processRound(int round)
{
	for (int initiative = initMax_; initiative >= initMin_; initiative--)
	{
//     combatReportFile << "      Initiative " << initiative<<endl;
    currentInitiativeSegment_ = initiative;
    combatActionOrders.setInitiative(initiative);
    combatMovementOrders.setInitiative(initiative);
		for(BattleIterator iter = attackers_->begin();
	    iter != attackers_->end(); ++iter )
  	{
				if(!(*iter)->getBattleInstantiation()->isAlive())
					continue;
				if((*iter)->getBattleInstantiation()->isFled())
					continue;
			combatReportFile<< "Initiative: "<< initiative<<" process orders for "
			<< (*iter)<<
			" ("<<((*iter)->getBattleInstantiation()->getCombatOrderList()).size()
			<<") "<<endl;
//    for( currentIterator = ((*iter)->getBattleInstantiation()->getCombatOrderList()).begin();
// 	      currentIterator != ((*iter)->getBattleInstantiation()->getCombatOrderList()).end();)
//      {

			// clear waiting for initiative flag
			(*iter)->getBattleInstantiation()->setWaiting(false);
			// Process combat movement orders first
			CombatOrderProcessor::process((*iter),&combatMovementOrders,round);
			// Process combat action orders
			CombatOrderProcessor::process((*iter),&combatActionOrders,round);
		}

  	for(BattleIterator iter = defenders_->begin();
	     iter != defenders_->end(); ++iter )
  	{
				if(!(*iter)->getBattleInstantiation()->isAlive())
					continue;
				if((*iter)->getBattleInstantiation()->isFled())
					continue;
			combatReportFile << "Initiative: "<< initiative<<" process orders for "
			<< (*iter)<<endl;
			// clear waiting for initiative flag
			(*iter)->getBattleInstantiation()->setWaiting(false);
			// Process combat movement orders first
			CombatOrderProcessor::process((*iter),&combatMovementOrders,round);
			// Process combat action orders
			CombatOrderProcessor::process((*iter),&combatActionOrders,round);
  	}
	}
}



// =====================================================================
void CombatEngine::postProcessRound(int round)
{
		attackerSideEnchantments_.processExpiration();
		defenderSideEnchantments_.processExpiration();

		for(BattleIterator iter = attackers_->begin();
	    iter != attackers_->end(); ++iter )
  	{
				if(!(*iter)->getBattleInstantiation()->isAlive())
					continue;
				if((*iter)->getBattleInstantiation()->isFled())
					continue;
				(*iter)->getBattleInstantiation()->postProcess();
	  }


		for(BattleIterator iter = defenders_->begin();
	     iter != defenders_->end(); ++iter )
  	{
				if(!(*iter)->getBattleInstantiation()->isAlive())
					continue;
				if((*iter)->getBattleInstantiation()->isFled())
					continue;
				(*iter)->getBattleInstantiation()->postProcess();
		}

}



// Determine if battle ended.
// Battle ends if one or both sides destroyed
bool CombatEngine::isBattleEnd()
{
	if(getWoundlessRoundCounter() >= 5)
	{
	  result_ = DRAW;
  	combatReportFile << "The draw: no figures were damaged duiring last 5 rounds."<<endl;
 		new SimpleMessage(combatDrawReporter) >>*report_ ;
 		return true;
	}
	if(attackersAliveCount_ <= 0)
	{
	  result_ = DEFENDER_VICTORY;
  	combatReportFile << "Defenders are victorious: all attackers destroyed."<<endl;
 		new SimpleMessage(combatDefendersVictoryReporter) >>*report_ ;
		return true;
	}
	if(defendersAliveCount_ <= 0)
	{
	  result_ = ATTACKER_VICTORY;
  	combatReportFile << "Defenders are victorious: all defenders destroyed."<<endl;
 		new SimpleMessage(combatAttackersVictoryReporter) >>*report_ ;
		return true;
	}
	// moral checks
	bool defendersRouted = false;
	bool attackersRouted = false;
	int sideModifier = 0;
	if(attackerLossCount_  > attackersAliveCount_)
	{
    sideModifier = 5 * calculateSideTactics(attackers_) -
		 attackerMoralCheckCount_;
		if(!sideMoralCheck(attackers_,sideModifier))
		{
			attackersRouted = true;
  	 combatReportFile << "Attackers are routed."<<endl;
			// report
		}
		attackerMoralCheckCount_++;
	}
	if(defenderLossCount_  > defendersAliveCount_)
	{
    sideModifier = 5 * calculateSideTactics(defenders_) -
		 defenderMoralCheckCount_;
		if(!sideMoralCheck(defenders_,sideModifier))
		{
			defendersRouted = true;
  	  combatReportFile << "Defenders are routed."<<endl;
			// report
		}
		defenderMoralCheckCount_++;
	}
  if(attackersRouted && defendersRouted)
	{
	  result_ = DRAW;
  	combatReportFile << "The draw: Both Attackers and Defenders are routed."<<endl;
 		new SimpleMessage(combatDrawReporter) >>*report_ ;
		return true;
	}
  if(attackersRouted)
	{
	  result_ = DEFENDER_VICTORY;
  	combatReportFile << "Defenders are victorious: attackers are routed."<<endl;
 		new SimpleMessage(combatDefendersVictoryReporter) >>*report_ ;
		return true;
	}
  if(defendersRouted)
	{
	  result_ = ATTACKER_VICTORY;
  	combatReportFile << "Attackers are victorious: defenders are routed."<<endl;
 		new SimpleMessage(combatAttackersVictoryReporter) >>*report_ ;
		return true;
	}
	return false;
}


void CombatEngine::postProcess()
{
  combatReportFile << "Attackers lost "<< (attackersCount_ - attackersAliveCount_)<< " men and defenders lost "<< (defendersCount_ - defendersAliveCount_)<<" men."<<endl;
	// die from wounds

// Add dead bodies
	ItemRule * deads = items["dead"];
	if(deads != 0)
	{
	location_->addResource(deads, attackersCount_ - attackersAliveCount_ 
					+ defendersCount_ - defendersAliveCount_);
	}

    BattleInstance * current;
// unsummon summoned units
		for(BattleEntityIterator iter = summoned_.begin();
	    iter != summoned_.end(); ++iter)
  	{
			current = (*iter)->getBattleInstantiation();


			if(current->isAttacker())
				{
					attackersCount_ -= (*iter)->getFigures();
					attackersAliveCount_ -= current->getFiguresNumber();
					for(BattleIterator iterA = attackers_->begin();
	    			iterA != attackers_->end(); ++iterA )
  					{
							if(*iter == *iterA)
							{
								attackers_->erase(iterA);
								break;
							}
						}
				}
			else
				{
					defendersCount_ -= (*iter)->getFigures();
					defendersAliveCount_ -= current->getFiguresNumber();
					for(BattleIterator iterA = defenders_->begin();
	    			iterA != defenders_->end(); ++iterA )
  					{
							if(*iter == *iterA)
							{
								defenders_->erase(iterA);
								break;
							}
						}
				}
			delete (*iter)->getBattleInstantiation();
			delete (*iter);


		}
		summoned_.clear();
	// remove dead figures, add  experience and loot
		for(BattleIterator iter = attackers_->begin();
	    iter != attackers_->end(); ++iter )
  	{
				current = (*iter)->getBattleInstantiation();
				current->dropLoot(result_ == DEFENDER_VICTORY);
				if(!current->isAlive()) // Dead
				{
				  current->dropTitles();
					(*iter)->disband();// here all titles are released
					continue;
				}
				else // Alive
				{
				current->updateOrigin();
				current->addFinalExperience(attackersAliveCount_,defendersCount_,
					 defendersAliveCount_);
				}
		}
  	for(BattleIterator iter = defenders_->begin();
	     iter != defenders_->end(); ++iter )
  	{
				current = (*iter)->getBattleInstantiation();
				current->dropLoot(result_ == ATTACKER_VICTORY);
				if(!current->isAlive())// Dead
				{
				  current->dropTitles();
					(*iter)->disband();
					continue;
				}
				else// Alive
				{
				current->updateOrigin();
				current->addFinalExperience(defendersAliveCount_, attackersCount_,
					  attackersAliveCount_);
				}
		}
	// loot distribution


/*  for (vector <ItemElement>::iterator iter = battleField_->getAllLoot().begin();
			iter != battleField_->getAllLoot().end(); ++iter)
			{
  			combatReportFile << (*iter).print() <<endl;
			}*/

 		battleField_->reportLoot();// report all loot
		// 50% of non-unique items will be lost in loot
	vector <ItemElement> currentLoot;
	currentLoot = battleField_->getCurrentLoot();
		// sort loot by price/weight value
  std::sort(currentLoot.begin(),currentLoot.end(),CombatEngine::sortLoot);
 		// distrinute loot randomly between all survived winner's units proportionally to unit's number untill unit has capacity to take

		switch (result_)
		{
			case DEFENDER_VICTORY:
			{
				battleField_->distributeLoot (currentLoot, *defenders_);
				// defender reclaims defender's titles
				battleField_->reclaimTitles(battleField_->getAllFreeDefenderTitles(),
				 *defenders_, 100);
				// defender reclaims attacker's titles
				battleField_->reclaimTitles(battleField_->getAllFreeDefenderTitles(),
				 *attackers_, 50);
				break;
			}
			case ATTACKER_VICTORY:
			{
				battleField_->distributeLoot (currentLoot, *attackers_);
				// attacker reclaims attacker's titles
				battleField_->reclaimTitles(battleField_->getAllFreeAttackerTitles(),
				 *attackers_, 100);
				// attacker reclaims defender's titles
				battleField_->reclaimTitles(battleField_->getAllFreeAttackerTitles(),
				 *defenders_, 50);
				break;
			}
			case DRAW:
			{
				// defender reclaims defender's titles
				// attacker reclaims attacker's titles
				battleField_->reclaimTitles(battleField_->getAllFreeAttackerTitles(),
				 *attackers_, 100);
				battleField_->reclaimTitles(battleField_->getAllFreeDefenderTitles(),
				 *defenders_, 100);
			}
			case BATTLE_ERROR:
			case BATTLE_UNDEFINED:
			case BATTLE_RESULTS_END:
			default:
				break;
		}

	// titles re-distribution
	// 50% lost
	// if draw - the same side leaders capable to claim - random
	// if victory - winning side leaders capable to claim - random





 if(attackersCount_ != attackersAliveCount_)
 	new UnaryMessage(attackerLossReporter,
		new IntegerData( attackersCount_ - attackersAliveCount_ )) >> *report_ ;
 if(defendersCount_ != defendersAliveCount_)
 	new UnaryMessage(defenderLossReporter,
		new IntegerData(defendersCount_ - defendersAliveCount_)) >> *report_ ;
}



void CombatEngine::addToDead(TokenEntity * dead)
{
	deadCombatants_.push_back(dead);
}


// Selects battle instance with worst morale and rolls for it
bool CombatEngine::sideMoralCheck(vector <TokenEntity *> * side, int modifier)
{
    int worstMorale = 200;
		int morale;
		for(BattleIterator iter = side->begin();
	     iter != side->end(); ++iter )
			 {
			   if((*iter)->getBattleInstantiation()->isFanatic())
				 	continue; // fanatics never fail morale check
			   if((*iter)->getBattleInstantiation()->isFled())
				 	continue; // unit is already far away

			   morale = (*iter)->getBattleInstantiation()->getMorale();
				 // consider introduce mallus for fleeing units
				 if(morale < worstMorale)
						worstMorale = morale;
			 }
		return (worstMorale + modifier > Roll_1Dx(100));
}



// Best side tactics
int CombatEngine::calculateSideTactics(vector <TokenEntity *> * side)
{
	int current;
	int max = 0;
	for(BattleIterator iter = side->begin();
	     iter != side->end(); ++iter )
			 {
			 	TokenEntity *currentEntity = *iter;
        current = currentEntity->getSkillLevel(tacticSkill);
				if(current > max)
					max = current;
			 }
	return max;
}

// Best side strategy
int CombatEngine::calculateSideStrategy(vector <TokenEntity *> * side)
{
	int current;
	int max = 0;
	for(BattleIterator iter = side->begin();
	     iter != side->end(); ++iter )
			 {
			 	TokenEntity *currentEntity = *iter;
        current = currentEntity->getSkillLevel(strategySkill);
				if(current > max)
					max = current;
			 }
	return max;
}


//best side ambush
int CombatEngine::calculateSideAmbush(vector <TokenEntity *> * side)
{
	int current;
	int max = 0;
	for(BattleIterator iter = side->begin();
	     iter != side->end(); ++iter )
			 {
			 	TokenEntity *currentEntity = *iter;
        current = currentEntity->getSkillLevel(ambushSkill);
				if(current > max)
					max = current;
			 }
	return max;
}


// best side observation
int CombatEngine::calculateSideObservation(vector <TokenEntity *> * side)
{
	int current;
	int max = 0;
	for(BattleIterator iter = side->begin();
	     iter != side->end(); ++iter )
			 {
			 	TokenEntity *currentEntity = *iter;
        current = currentEntity->getObservation();
				if(current > max)
					max = current;
			 }
	return max;
}



int CombatEngine::calculateAttackerInitiative(int round)
{
	int tactics = 2 * (attackerTactics_ - defenderTactics_) - round;

	if(tactics > 0 )
		return attackerStrategy_ + tactics;
	else
		return attackerStrategy_;

}



int CombatEngine::calculateDefenderInitiative(int round)
{
	int tactics = 2 * (defenderTactics_ - attackerTactics_) - round;

	if(tactics > 0 )
		return defenderStrategy_ + tactics;
	else
		return defenderStrategy_;

}



void CombatEngine::processRoutingRound()
{
	if(result_ == DEFENDER_VICTORY)
	{	// rout attackers
		for(BattleIterator iter = attackers_->begin();
	    iter != attackers_->end(); ++iter )
  	{
				if(!(*iter)->getBattleInstantiation()->isAlive())
					continue;
				(*iter)->getBattleInstantiation()->rout();
		}

	}

	if(result_ == ATTACKER_VICTORY)
	{
	// rout defenders
  	for(BattleIterator iter = defenders_->begin();
	     iter != defenders_->end(); ++iter )
  	{
				if(!(*iter)->getBattleInstantiation()->isAlive())
					continue;
				(*iter)->getBattleInstantiation()->rout();
		}
	}


}



void CombatEngine::shuffleSide(vector <TokenEntity *> * side, int ambush)
{
	for(BattleIterator iter = side->begin();
	     iter != side->end(); ++iter )
			 {
			 	BattleInstance *currentInstance = (*iter)->getBattleInstantiation();
				for (int i= 0; i<ambush; ++i)
					{
						this->battleField_->shuffleOnce(currentInstance);
					}
			 }

}


bool CombatEngine::sortLoot(ItemElement element1, ItemElement element2)
{
	int weight1 = element1.getItemType()->getWeight();
	int weight2 = element2.getItemType()->getWeight();
// zero-weight items go first.
	if(weight1 == 0)
		return true;

	if(weight2 == 0)
		return false;

	return (element1.getItemType()->getFormalPrice() /weight1 >= element2.getItemType()->getFormalPrice() /weight2);
}




void CombatEngine::addAttackerSideEnchantment(EnchantmentElement * data)
{
	attackerSideEnchantments_.add(data);
	for(BattleIterator iter = attackers_->begin();
	    iter != attackers_->end(); ++iter )
  	{
				if(!(*iter)->getBattleInstantiation()->isAlive())
					continue;
				if((*iter)->getBattleInstantiation()->isFled())
					continue;
				(*iter)->getBattleInstantiation()->recalculateStats();
		}

}



void CombatEngine::removeAttackerSideEnchantment(EnchantmentElement * data)
{
	attackerSideEnchantments_.remove(data);
	for(BattleIterator iter = attackers_->begin();
	    iter != attackers_->end(); ++iter )
  	{
				if(!(*iter)->getBattleInstantiation()->isAlive())
					continue;
				if((*iter)->getBattleInstantiation()->isFled())
					continue;
				(*iter)->getBattleInstantiation()->recalculateStats();
		}

}



void CombatEngine::removeAllAttackerSideEnchantment()
{
	attackerSideEnchantments_.removeAll();
}



void CombatEngine::addDefenderSideEnchantment(EnchantmentElement * data)
{
	defenderSideEnchantments_.add(data);
	for(BattleIterator iter = defenders_->begin();
	    iter != defenders_->end(); ++iter )
  	{
				if(!(*iter)->getBattleInstantiation()->isAlive())
					continue;
				if((*iter)->getBattleInstantiation()->isFled())
					continue;
				(*iter)->getBattleInstantiation()->recalculateStats();
		}
}



void CombatEngine::removeDefenderSideEnchantment(EnchantmentElement * data)
{
	defenderSideEnchantments_.remove(data);
	for(BattleIterator iter = defenders_->begin();
	    iter != defenders_->end(); ++iter )
  	{
				if(!(*iter)->getBattleInstantiation()->isAlive())
					continue;
				if((*iter)->getBattleInstantiation()->isFled())
					continue;
				(*iter)->getBattleInstantiation()->recalculateStats();
		}
}



void CombatEngine::removeAllDefenderSideEnchantment()
{
	defenderSideEnchantments_.removeAll();
}


void CombatEngine::extendMinInitiative()
{
	if (!(currentInitiativeSegment_ >= initMin_ + 1))
		initMin_--;

}


void CombatEngine::addSummonedEntity(BattleInstance * summoner, BattleEntity * summonedOne)
{
	 summoned_.push_back(summonedOne);
	 BattleInstance * summonedInstance =summonedOne->getBattleInstantiation();
	// name?
  if(summoner->isAttacker())
	{
    summonedInstance->setAttacker();// mark as attacker
	  summonedInstance->setSideEnchantment(&attackerSideEnchantments_);
		attackers_->push_back(summonedOne);
		attackersCount_ += summonedOne->getFigures();
		attackersAliveCount_ += summonedOne->getFigures();
	}
	else
	{
    summonedInstance->setDefender();// mark as defender
	  summonedInstance->setSideEnchantment(&defenderSideEnchantments_);
		defenders_->push_back(summonedOne);
		defendersCount_ += summonedOne->getFigures();
		defendersAliveCount_ += summonedOne->getFigures();
	}
	// side observation update
 // no update for side  ambush, tactis, stategy -
 // new summoned unit has no time to reorganize troops that are already in the battle

}
