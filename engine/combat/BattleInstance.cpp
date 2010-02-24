/***************************************************************************
                      BattleInstance.cpp
               Instance taking part in combat
                          ------------------
    begin                : Mon Oct 26 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "BattleInstance.h"
#include "BattleField.h"
#include "TokenEntity.h"
#include "UnitEntity.h"
#include "CombatOrderLine.h"
#include "CombatMoveVariety.h"
#include "CombatManager.h"
#include "CombatReport.h"
#include "CombatEngine.h"
#include "CombatActionStrategy.h"
#include "reporting.h"
#include "BasicLearningStrategy.h"
#include "EquipmentSlotVariety.h"
#include "LeaderRaceRule.h"

SkillRule * parrySkill;
SkillRule * meleeSkill;
SkillRule * missileSkill;
EquipmentSlotVariety * weaponSlot;
extern ofstream combatReportFile;
extern int Roll_1Dx(int n);
ReportPattern * unitSlainAutoReporter= new ReportPattern("unitSlainAutoReporter");
ReportPattern * combatFleeReporter = new ReportPattern("combatFleeReporter");
static const int FLEE_TRASHOLD = 3;

BattleInstance::BattleInstance(TokenEntity * origin, CombatReport * report)
{
	report_ = report;
	positionRank_ =0;
	positionFile_ =0;
	isMovedOnRound_= 0;
	isActedOnRound_= 0;
	isAttacker_ = false;
	combatOrders_ = origin->getCombatOrderList();
	currentOrder_ = 0;
	origin_ = origin;
	battleField_ = 0;
	parryFlag_ = false;
	isAttackedOnRound_ = 0;
	isWaiting_ = false;
	wounds_ = 0;
	fanatic_ =origin_->isFanatic();
	routed_ = false;
	combatExperience_ = 0;
	parryExperience_ = 0;
	meleeExperience_ = 0;
	missileExperience_ = 0;
	currentlyRepeatingOrder_ = 0;
	repetitionCounter_ = 0;
	fleeCounter_ = 0;
	fled_ = false;
        fleing_ =false;
	affectingAction_ = 0;
	movementInitiative_ = 0;
	// Attributes

	inventory_ = InventoryAttribute(origin_);

	for(InventoryIterator iter = origin_->getAllInventory().begin();
			iter != origin_->getAllInventory().end();	 ++iter)
  {
		inventory_.add(*iter);
  }
	// try to get titles
	UnitEntity * originUnit = dynamic_cast<UnitEntity *>(origin_);
	if(originUnit)
	{
  	titles_ = originUnit->getTitlesAttribute();
		equipSlots_ = &(originUnit->getRace()->getAllEquipmentSlots());
		race_ =originUnit->getRace();
	}
	else
		{
  	titles_ = 0;
		equipSlots_ = 0;
		race_ = 0;
//  	titles_ = new TitlesAttribute();// empty
		}
	skills_ = origin_->getAllSkills();
	stats_.addStats(origin_->getStats());

	damageType_ = origin_->getDamageType();
	figuresNumber_ = origin->getFiguresNumber();


	int figuresLife = stats_.getLife();
	for (int i = 0; i <figuresNumber_; ++i)
		figures_.push_back(figuresLife);

	// equip some items on Battle instance (not Origin)
	  sideEnchantments_ =0;
		recalculateStats();
}


BattleInstance::~BattleInstance()
{
 combatOrders_.clear();
}



void BattleInstance::initialize(BattleField * battleField)
{
  battleField_ = battleField;
	combatReportFile<< "     Explicit orders for "<<origin_ <<":"<<endl;
 for (CombatOrderIterator iter = combatOrders_.begin();
 			iter != combatOrders_.end(); ++iter)
			{
				(*iter)->initialize(origin_);
				 // calculate initiative
				 (*iter)->printOrderLine(combatReportFile);
			}
//	combatReportFile << endl;
}


string BattleInstance::print()
{
		return origin_->print();
}

void BattleInstance::postProcess()
{
  if(fleing_)
  {
    fleing_ = false;
    advanceFleeCounter();
    if(getFleeCounter() >= FLEE_TRASHOLD)
    {
      fleeAway();
    }	
  }
//  enchantments_.carryOutAllActions(this,1); //BattleEnchantment
  enchantments_.processExpiration(0);
	affectingAction_ = 0;
	recalculateStats();
}

void BattleInstance::preProcess()
{
}



// Check that there are at least some actions and movements
// in combat oredrs list. If not - respawn them
void BattleInstance::recheckOrders()
{
 if(routed_) // no respawn orders for routed unit
 	return;
 bool combatActionOrdersPresent = false;
 bool combatMovementOrdersPresent = false;
 combatReportFile << "       - Checking orders for "<< origin_
 			<<"("<< positionFile_<<","<< positionRank_<<")"<<endl;
 for (CombatOrderIterator iter = combatOrders_.begin();
 			iter != combatOrders_.end(); ++iter)
			{
//        combatReportFile << "       -"; (*iter)->printOrderLine(combatReportFile);
				if ((*iter)->getOrderType() == COMBAT_ACTION_ORDER)
					{
						combatActionOrdersPresent = true;
						break;
					}

			}
 for (CombatOrderIterator iter = combatOrders_.begin();
 			iter != combatOrders_.end(); ++iter)
			{
//        combatReportFile << "       -"; (*iter)->printOrderLine(combatReportFile);
				if ((*iter)->getOrderType() == COMBAT_MOVEMENT_ORDER)
					{
						combatMovementOrdersPresent = true;
						break;
					}

			}
	if(!combatActionOrdersPresent)
		respawnCombatActionOrders();

	if(!combatMovementOrdersPresent)
		respawnCombatMovementOrders();
}


// Called if no combat action orders left.
// fill the list with the set of default combat orders
void BattleInstance::respawnCombatActionOrders()
{
//   if(origin_ == units["u1003"])
//   {
//     cout<<"ff";
//   }
  for(CombatOrderIterator iter = origin_->getDefaultCombatOrders().begin();
          iter != origin_->getDefaultCombatOrders().end(); ++iter)
  {
          // When we copy new order all flags should be cleared:
          (*iter)->initialize(origin_);
          (*iter)->setPermanent(true);
          combatOrders_.push_back(*iter);
          combatReportFile << "added combat action order to "<<origin_<<": ";
          (*iter)->printOrderLine(combatReportFile);
  }
  combatOrders_.push_back(new CombatOrderLine(string("@MELEE"),origin_));
  combatOrders_.push_back(new CombatOrderLine(string("@PARRY"),origin_));


}



// Called if no combat movement orders left.
// fill the list with the set of default combat orders
void BattleInstance::respawnCombatMovementOrders()
{
 	combatReportFile<< "adding combat movement order to "<< origin_<<": ";
	origin_->getDefaultCombatMovement()->printOrderLine(combatReportFile);
	// When we copy new order all flags should be cleared:
	origin_->getDefaultCombatMovement()->initialize(origin_);
	origin_->getDefaultCombatMovement()->setPermanent(true);
	combatOrders_.push_back(origin_->getDefaultCombatMovement());
}



int BattleInstance::reCalculateInitiative(int sideBonus,
																					int &initMin, int &initMax )
{
 int current = 0;
//     combatReportFile << "    Inside: initMax= " << initMax<< " initMin= " << initMin<<endl;
 for (CombatOrderIterator iter = combatOrders_.begin();
 			iter != combatOrders_.end(); ++iter)
			{

				current = (*iter)->reCalculateInitiative(origin_->getInitiative(),sideBonus);
				if (current > initMax)
					initMax = current;
				if (current < initMin)
					initMin = current;
//     cout << "           current="<< current<<" initMax= " << initMax<<endl;
			}
			return current;
}



bool BattleInstance::mayActOnRound(OrderPrototype * orderType,int round )
{
  switch(orderType->getOrderType())
	{
		case COMBAT_ACTION_ORDER:
		  if(isWaiting())
				return false;
			if(isActedOnRound_ == round)
				return false;
			else
				return true;
		case COMBAT_MOVEMENT_ORDER:
			if(isMovedOnRound_ == round)
				return false;
			else
				return true;
		case INSTANT_ORDER:
				return true;
		default:
		cout << "Unexpected type of order in mayAct " << orderType->getOrderType()<<endl;
	}
		return false;
}



void BattleInstance::setRoundFlags(ORDER_TYPE orderType,int round)
{
  switch(orderType)
	{
		case COMBAT_ACTION_ORDER:
			isActedOnRound_ = round;
				return;

		case COMBAT_MOVEMENT_ORDER:
			isMovedOnRound_ = round;
				return;

		case INSTANT_ORDER:
				return ;
		default:
		cout << "Unexpected type of order in setRoundFlags " << orderType<<endl;
	}

}



void BattleInstance::setSideEnchantment(EnchantmentAttribute * sideEnchantment)
{
	sideEnchantments_ = sideEnchantment;
}



bool BattleInstance::isAlive()
{
   if(!figures_.empty())
				return true;
	else
		return false;
}


void BattleInstance::clearRoundFlags()
{
//	isMovedOnRound_= 0;
//	isActedOnRound_= 0;
	parryFlag_ = false;
	isAttackedOnRound_ = 0;
	isWaiting_ = false;
}



void BattleInstance::parry()
{
	parryFlag_ = true;
}



int BattleInstance::getHitNumbers() const
{
		return stats_.getHits();
}



int BattleInstance::getStealth()const
{
	if(routed_)
		return 0;
	else
		return stats_.getStealth();
}



int BattleInstance::getObservation()const
{
	return stats_.getObservation();
}



int BattleInstance::getDamage()  const
{
	return stats_.getDamage();
}


int BattleInstance::getRangedDamage()  const
{
	return stats_.getRangedDamage();
}


int BattleInstance::getMelee()const
{
	return stats_.getMelee();
}



int BattleInstance::getMissile()const
{
	return stats_.getMissile();
}


int BattleInstance::getDefence()const
{

	if(parryFlag_)
		return stats_.getDefence() + 2;
	else
		return stats_.getDefence();
}


int BattleInstance::getFiguresNumber()const
{
	return figuresNumber_;
}

void BattleInstance::setFiguresNumber(int value)
{
	figuresNumber_ = value;
}

int BattleInstance::getWounds()const
{
	return wounds_;
}

void BattleInstance::setWounds(int value)
{
	 wounds_ = value;
}


// In this implementation morale is defence * 5
int BattleInstance::getMorale() const
{
  return 5 * getDefence();
}


DAMAGE_TYPE BattleInstance::modifyDamageType(DAMAGE_TYPE damageType)
{
	if (damageType_ == PHYSICAL)
	{
		damageType_ = damageType;
	}
	else
		{
			if (damageType != PHYSICAL) // two magic damage neutralize each other
				damageType_ = PHYSICAL;
		}
	return damageType_;
}



// Estimate which orders may be executed on this round.
// If we have mandatory orders like RELOAD it should be treated here
void BattleInstance::planRoundOrders()
{
 int initiative;
 bool isFirst=true;
 // if RELOAD flag mark all action orders as unusable
   for( CombatOrderIterator iter = combatOrders_.begin();
	      iter != combatOrders_.end();++iter)
     {
		 	// check only sequentive action orders
			  if ((*iter)->getOrderType() != COMBAT_ACTION_ORDER)
					continue;
				// If (RELOAD flag set) {(*iter)->setPlanned(false);continue;}
				if(!(*iter)->isSequentive())
					continue;
			// evaluate
				if((*iter)->evaluate(origin_))
				{
						(*iter)->setPlanned(true);
						initiative = (*iter)->getInitiative();
								//check that tactics and personal init included.
						if (isFirst)
						{
								isFirst = false;
								movementInitiative_ = initiative;
						}
				}
				else
				{
					//mark as not usable;
					(*iter)->setPlanned(false);
				}

		 }

 // clear reload flag
}


// Instance takes damage.
// It may loose some figures or die
// Returns number of killed figures.
int BattleInstance::sufferDamage(int hits, int damage, DAMAGE_TYPE type)
{
  combatReportFile  <<origin_<<" takes "<< hits << " hits of " <<damage<<" damage" <<endl;
  
  
  if(hits <= 0)
    return 0;
  if(damage <= 0)
    return 0;
  combatReportFile  <<"--- Life ---"<<endl;
  for (int i = 0; i <figures_.size(); ++i)
  {
    combatReportFile  <<figures_[i]<<endl;
  }
  combatReportFile  <<"--- Life End ---"<<endl;
  //combatReportFile  <<origin_<<" takes "<< hits << " hits of " <<damage<<" damage" <<endl;
  
  int overkill = 0; // calculated for possible future use
  int figuresDied = 0;
  for (int i = 0; i <hits; ++i)
  {
    if(figures_.empty())
    {
      overkill += damage;
      continue;
    }
    int randomIndex = Roll_1Dx(figures_.size());
    int currentLife = figures_[randomIndex];
    if(currentLife > damage)
      figures_[randomIndex] -= damage;
    else
    {
      overkill += damage - figures_[randomIndex];
      figures_.erase(figures_.begin() +randomIndex);
      figuresDied ++;
    }
  }
  if(figuresDied)
  {
    lossFigures(figuresDied);
  }
  if(figures_.empty())
  {
    destroy();
  }
  return figuresDied;
}



// Instance lost some figures. Some actions may be requried
// Count losses
// unequip extra items
// recalculate stats
void BattleInstance::lossFigures(int figuresDied)
{
  CombatEngine * combat = battleField_->getCombatEngine();

	figuresNumber_ -= figuresDied;

		if(isAttacker())
		{
			combat->decreaseAliveAttackers(figuresDied);
		}
		else
		{
			combat->decreaseAliveDefender(figuresDied);
		}

	if(!isFanatic())
	{
		if(isAttacker())
  		combat->addAttackerLoss(figuresDied); 
		else
  		combat->addDefenderLoss(figuresDied);
	}
	// Report

  combatReportFile << origin_ << " lost " << figuresDied << " figures"<<endl;


	// unequip some items on Battle instance (not Origin)
  if(figuresNumber_) // still alive. No need to unequip dead units
	{
		if(equipSlots_)
		{
	  	int equipMax;
  		vector < InventoryElement> unequiped;// not really used here
  		for (EquipSlotIterator iter = equipSlots_->begin();
                    iter != equipSlots_->end(); ++iter)
    	{
	 			equipMax = figuresNumber_ * race_-> getEquipCapacity((*iter)->type);
				inventory_.updateSlotEquipement((*iter)->type, unequiped,  equipMax);
			}
		recalculateStats();
		}
	}
}




// Instance destroyed. Some actions may be requried
// Remove it from the battlefield
//
void BattleInstance::destroy()
{
	figuresNumber_ = 0;
	battleField_->removeEntity(origin_);
	// Report

	origin_->addReport(new UnaryMessage(unitSlainAutoReporter,
 		new StringData(battleField_->getName())),0,0);
  combatReportFile << origin_ << " is destroyed."<<endl;

}

// If dead drop all and drop all inventory and titles
// If alive and on loosing side ?
void BattleInstance::dropLoot(bool loosing)
{

	vector < InventoryElement > & inventory = origin_->getAllInventory();
	if(isAlive())
	{
		if(loosing)
		{
		}
		if(routed_)
		{
		}
		if(fled_)
		{
		}
	}
	else // dead
	{
		for (InventoryIterator iter = inventory.begin(); iter != inventory.end();
											 iter++)
		{
				battleField_->addLoot((*iter).getItemType(), (*iter).getItemNumber());
		}
	}

}




void BattleInstance::dropTitles()
{
	if(isAttacker())
		battleField_->addAttackerTitles(getAllTitles());
	else
		battleField_->addDefenderTitles(getAllTitles());
}


// apply changes that happened with BattleInstance to origin TokenEntity
// remove dead figures
void BattleInstance::updateOrigin()
{
	int oldFigures = origin_->getFiguresNumber();
	if(oldFigures > figuresNumber_ )
	{
  	origin_->sufferDamage(oldFigures - figuresNumber_);
	}
}



// This is recalculation of BattleInstance stats only
// recalculation of origin stats is not included
//
// Recalculation of life determines life delta
// that will be applied to remaining life of figures
void BattleInstance::recalculateStats()
{
	int lifeBefore = stats_.getLife();
	if(lifeBefore <= 0)
	{
		cerr << "Life of "<< print()<< " is 0."<<endl;
		return;
	}
	stats_.clearStats();
	stats_ = origin_->getBasicStats();
	enchantments_.addStats(&stats_,figuresNumber_);
	inventory_.addStats(&stats_,figuresNumber_);
	skills_.addStats(&stats_);
	if(sideEnchantments_)
	{
		sideEnchantments_->addStats(&stats_,figuresNumber_);
	}
	if(affectingAction_)
	{
	// modify or set everything but initiative, because initiative was already set
	// on the stage of loading parameters
	int initiative = stats_.getInitiative();

   stats_.addStats( affectingAction_->getModifyingStats());
   stats_.replaceStats( affectingAction_->getNonCumulativeStats());

	 stats_.setInitiative(initiative);
	}
// Recalculate actual remaining health of figures;
	int lifeAfter = stats_.getLife();
	int actualOld = 0;

	for(vector <int>::iterator iter = figures_.begin(); iter != figures_.end();
					 ++iter)
	{
		actualOld = *iter;
		*iter = (actualOld * lifeAfter + lifeBefore/2) /lifeBefore;
		if (*iter <= 0) // figure will not die from recalculation
			*iter = 1;
	}
}




void BattleInstance::addSideEnchantment(EnchantmentElement * data, bool targetSide)
{
	// attacker casting on the same side or defender on the opposote
	if((isAttacker() && targetSide) || (isDefender() && !targetSide))
	{
		battleField_->getCombatEngine()->addAttackerSideEnchantment(data);
	}
	else
	{
		battleField_->getCombatEngine()->addDefenderSideEnchantment(data);
	}
}


// How do you remove permanent enchantments?
void BattleInstance::removeSideEnchantment(EnchantmentElement * data, bool targetSide)
{
	// attacker casting on the same side or defender on the opposote
	if((isAttacker() && targetSide) || (isDefender() && !targetSide))
	{
		battleField_->getCombatEngine()->removeAttackerSideEnchantment(data);
	}
	else
	{
		battleField_->getCombatEngine()->removeDefenderSideEnchantment(data);
	}
}




void BattleInstance::removeAllSideEnchantment(bool targetSide)
{
	// attacker casting on the same side or defender on the opposote
	if((isAttacker() && targetSide) || (isDefender() && !targetSide))
	{
		battleField_->getCombatEngine()->removeAllAttackerSideEnchantment();
	}
	else
	{
		battleField_->getCombatEngine()->removeAllDefenderSideEnchantment();
	}
}







void BattleInstance::addActionExperience(SkillRule * skill)
{
	int experiencePerAction = 1;
	SkillIterator iter;
  for (iter = actionExperience_.begin(); iter != actionExperience_.end(); iter++)
		{
			if( (*iter).getSkill() == skill)
					{
						(*iter).setExpPoints((*iter).getExpPoints() + experiencePerAction);
						return;
					}
		}
	actionExperience_.push_back(SkillElement(skill,experiencePerAction));
}



// Calculate toatal experience gained over battle
void BattleInstance::addFinalExperience(int survived, int totalEnemies, int survivedEnemies)
{
	if (survived == 0)
		return;
	int maxExperience = (15 * (totalEnemies - survivedEnemies) + survivedEnemies);
	maxExperience += survived/2; // for fair rounding
	maxExperience /= survived;
  maxExperience *= BasicLearningStrategy::getPointsPerDay();

	if(combatExperience_ > 20 )
		combatExperience_ = 20;
	combatExperience_ *= BasicLearningStrategy::getPointsPerDay()/2;

	if(combatExperience_ > maxExperience )
		combatExperience_ = maxExperience;

	if(combatExperience_)
		origin_->addSkill(combatSkill,combatExperience_);
  	combatReportFile << combatExperience_ << " of combat experience was added to " <<origin_ <<endl;

	if(parryExperience_ > 20 )
		 parryExperience_= 20;
	 parryExperience_*= BasicLearningStrategy::getPointsPerDay()/2;
	if(parryExperience_ > maxExperience )
		 parryExperience_= maxExperience;

	if(parryExperience_)
	{
		origin_->addSkill(parrySkill,parryExperience_);
  		combatReportFile << parryExperience_ << " of parry experience  was added to "
		 <<origin_ <<endl;
	}

	if(meleeExperience_)
	{
		if(meleeExperience_ > 20 )
		 	meleeExperience_= 20;
	 	meleeExperience_*= BasicLearningStrategy::getPointsPerDay()/2;

		vector < InventoryElement > weapons = origin_->getSlotContent(weaponSlot);
  	vector < InventoryElement >::iterator iter;
  	for( iter = weapons.begin(); iter != weapons.end(); ++iter)
  	{
    	SkillRule * skill = meleeSkill;
			BasicCondition * equipCondition =
			 	(*iter).getItemType()->demandsEquipCondition();
			if(equipCondition)
			{
				Rule * rule = equipCondition->getSubject();
				skill = dynamic_cast<SkillRule *>(rule);
				if(skill == 0)
					skill = meleeSkill;//default
			}
			int num = (*iter).getEquipedNumber();
			origin_->addSkill(skill,meleeExperience_ * num /figuresNumber_);
  	combatReportFile << meleeExperience_ * num /figuresNumber_ << " of " <<*skill <<" experience  was added to " <<origin_ <<endl;

  	}
	}

	if(missileExperience_ > 20 )
		 missileExperience_= 20;
	 missileExperience_*= BasicLearningStrategy::getPointsPerDay()/2;


	if(missileExperience_)
	{
		origin_->addSkill(missileSkill,missileExperience_);
  		combatReportFile << missileExperience_ << " of archery experience  was added to "
		 <<origin_ <<endl;
	}
 // add all action-related experience
  for (vector <SkillElement>::iterator iter = actionExperience_.begin();
					iter != actionExperience_.end(); ++iter)
	{
		int experience = (*iter).getExpPoints();
		if(experience > 20 )
		 	experience = 20;
	 	experience*= BasicLearningStrategy::getPointsPerDay()/2;
		origin_->addSkill((*iter).getSkill(),experience);

	}
}



bool BattleInstance::isFlying()
{
	return (origin_->getCapacity(flyingMode) >= origin_->getWeight());
}


// Set movement to FLEE, combat to default MELEE
void BattleInstance::rout()
{
	if(fanatic_)
		return;
	routed_ = true;
	combatOrders_.clear();
	if (Roll_1Dx(3))
		combatOrders_.push_back(new CombatOrderLine(string("PARRY"),origin_));
	combatOrders_.push_back(new CombatOrderLine(string("FLEE"),origin_));
}



bool BattleInstance::isProcessingRepeatingOrder()
{
	if(repetitionCounter_)
		return true;
	else
		return false;
}






void BattleInstance::processMultiHitting(int hitNumbers)
{
	if(repetitionCounter_)
		{
			repetitionCounter_--;
			if(repetitionCounter_ <= 0) // This was last time
					currentlyRepeatingOrder_ = 0;
			else
			getBattleField()->getCombatEngine()->extendMinInitiative();
		}
	else // First time. Should set counter and order
		{
			repetitionCounter_ = hitNumbers - 1;
			currentlyRepeatingOrder_ = currentOrder_;
			getBattleField()->getCombatEngine()->extendMinInitiative();
		}
}



// Removes unit from the battlefield

void BattleInstance::fleeAway()
{
	battleField_->removeEntity(origin_);
	fled_ = true;
	// Report
  int initiative = getBattleField()->getCombatEngine()->
	getCurrentInitiativeSegment();

 	new BinaryMessage(combatFleeReporter,
				new IntegerData(initiative), origin_) >> *report_;

  combatReportFile << origin_ << " fled from the battlefield."<<endl;
}



bool BattleInstance::isLeader()
{
	return race_->isDescendantFrom(&sampleLeaderRaceRule);
}
