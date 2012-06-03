/***************************************************************************
                          CombatActionStrategy.cpp
                             -------------------
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "GameFacade.h"
#include "CombatActionStrategy.h"
#include "CombatTargetVariety.h"
#include "TokenEntity.h"
#include "ActionRule.h"
#include "RulesCollection.h"
#include "BattleInstance.h"
#include "reporting.h"

CombatActionStrategy      sampleCombatAction("COMBAT_ACTION",&sampleGameData);
ReportPattern * noTargetsReporter= new ReportPattern("noTargetsReporter");

GameData * CombatActionStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<CombatActionStrategy> (this);
}




CombatActionStrategy::CombatActionStrategy ( const string & keyword, GameData * parent): Strategy(keyword, parent)
{
	range_ = 0;
	action_ = 0;
	target_ = 0;
        mana_ = 0;
        nonCumulativeStats.clearStats(VERY_BIG_NUMBER);
        expGainingSkill_ =0;
}



CombatActionStrategy::CombatActionStrategy ( const CombatActionStrategy * prototype ): Strategy(prototype)
{
  range_ = 0;
  action_ = 0;
  target_ = 0;
  mana_ = 0;
  tag_ = "CombatActionStrategy";
  nonCumulativeStats.clearStats(VERY_BIG_NUMBER);
  expGainingSkill_ =0;
}


			STATUS
CombatActionStrategy::initialize        ( Parser *parser )
{
  if (parser->matchKeyword ("TARGET") )
    {
			target_ = gameFacade->combatTargets[parser->getWord()];
                        if(target_==0)
                        {
                          cout << "Empty target in "<< parser->report()<<endl;
                        }
      return OK;
    }
  if (parser->matchKeyword ("EFFECT") )
    {
			action_ = gameFacade->fx_actions[parser->getWord()];
                        if(action_==0)
                        {
                          cout << "Empty effect in "<< parser->report()<<endl;
                        }
			return OK;
    }

	if (parser->matchKeyword ("RANGE") )
    {
			range_ = parser->getInteger();
			return OK;
    }
	if (parser->matchKeyword ("USE_MANA") )
    {
			mana_ = parser->getInteger();
			return OK;
    }

	if (parser->matchKeyword ("CONSUME") )
    {
			if(parser->matchElement())
			  resources_.push_back(new ItemElement(parser));
      return OK;
    }

	if (parser->matchKeyword ("BONUS") )
    {
     modifyingStats.initialize(parser);
    }
	else
		{
     nonCumulativeStats.initialize(parser);
		}
     return OK;

}



void CombatActionStrategy::save(ostream &out)
{

    if(target_) out<<"COMBAT "<< "TARGET"<<" "<<target_->getTag() <<endl;
    if(action_)out<<"COMBAT "<< "EFFECT"<<" "<<action_->getTag() <<endl;
    if(range_)out<<"COMBAT "<< "RANGE"<<" "<<range_ <<endl;
    if(mana_)out<<"COMBAT "<<"USE_MANA"<<" "<< mana_<<endl;
    for(vector <ItemElement *>::iterator iter =resources_.begin(); iter != resources_.end();++iter)
    {
      out<<"COMBAT "<<"CONSUME ";
      (*iter)->save(out);
    }
    modifyingStats.save(out,"COMBAT BONUS ",0);
    nonCumulativeStats.save(out,"COMBAT ",VERY_BIG_NUMBER);
}



// Default action is to do nothing.
void		CombatActionStrategy::performAction(BattleInstance * , BattleTargets & , CombatReport * )
{
}


// get all potential targets
BattleTargets CombatActionStrategy::getPotentialTargets(
		BattleInstance * battleInstance, CombatReport * report)
{
  BattleTargets potentialTargets;
  bool isTraced = battleInstance->getOrigin()->isTraced();

  if(isTraced)
  {
      cout<< battleInstance->getOrigin()<<" applying "<< this->print()<<" targets "<<target_->print()<<endl;
  }
	BattleField * battleField = battleInstance->getBattleField();
//	CombatReport * report = battleField->getCombatEngine()->getCombatReport();

		if(target_ == battleTargetSelf)
		{
      potentialTargets.push_back(BattleTargetElement(battleInstance,SELF,0));
		}

		else if (target_ == battleTargetAll)// Dummy. Target should not be empty
		{
      potentialTargets.push_back(BattleTargetElement(battleInstance,SELF,0));
		}

		else if (target_ == battleTargetOwnSide)// Dummy. Target should not be empty
		{
      potentialTargets.push_back(BattleTargetElement(battleInstance,SELF,0));
		}

		else if (target_ == battleTargetOppositeSide)// Dummy. Target should not be empty
		{
      potentialTargets.push_back(BattleTargetElement(battleInstance,SELF,0));;
		}

		else if (target_ == battleTargetOpposing)
		{
			battleField->addAllEnemiesAtRange(battleInstance, potentialTargets, range_);
      processStealthTargets(potentialTargets,battleInstance);
		}

		else if (target_ == battleTargetFriend)
		{
			battleField->addAllFriendsAtRange(battleInstance, potentialTargets, range_);
		}

		else if (target_ == battleTargetFriendlyLeader)
		{
			battleField->addAllFriendsAtRange(battleInstance, potentialTargets, range_);
			// Now erase all non-leaders
			for(vector <BattleTargetElement>::iterator iter =potentialTargets.begin();
					iter != potentialTargets.end(); )
				{
					if(! (*iter).instance_->isLeader())
		 				{
							potentialTargets.erase(iter);
							continue;
		 				}
					++iter;
				}
		}

		else if (target_ == battleTargetOpposingLeader)
		{
			battleField->addAllEnemiesAtRange(battleInstance, potentialTargets, range_);
      processStealthTargets(potentialTargets,battleInstance);
			// Now erase all non-leaders
			for(vector <BattleTargetElement>::iterator iter =potentialTargets.begin();
					iter != potentialTargets.end(); )
				{
					if(! (*iter).instance_->isLeader())
		 				{
							potentialTargets.erase(iter);
							continue;
		 				}
					++iter;
				}
		}

	if(potentialTargets.empty()) // No targets. Report
	{
		new UnaryMessage(noTargetsReporter,battleInstance->getOrigin()) >>*report ;
		return potentialTargets;
	}



  std::sort(potentialTargets.begin(),potentialTargets.end(),
		&attackPreference);

   combatReportFile << "-------- Targets after sorting: (";
	for(vector <BattleTargetElement>::iterator iter =potentialTargets.begin();
	iter != potentialTargets.end(); ++iter)
	{
   combatReportFile <<(*iter).instance_->print()<< " ";
	}
	combatReportFile<< ")"<<endl;

	return potentialTargets;
}

void CombatActionStrategy::processStealthTargets(BattleTargets & potentialTargets,
		 BattleInstance * battleInstance)
{
	int stealthBonus;


	for(vector <BattleTargetElement>::iterator iter =potentialTargets.begin();
	iter != potentialTargets.end(); )
	{
	 stealthBonus = (*iter).instance_->getStealth() -
	 	battleInstance->getObservation();
	 if(stealthBonus > 0)
	 {
	 	 if(stealthBonus > Roll_1Dx(10))
		 	{
				potentialTargets.erase(iter);
				continue;
		 	}
		}
		++iter;
	}
}

//--  Performs attacks against all potential targets untill all of them are dead
//--   or maximum hits
MeleeReport CombatActionStrategy::makeAttack(BattleInstance * battleInstance,
		BattleTargets & potentialTargets, CombatReport * report)
{
  BattleInstance * target;
  int targetSize;
  MeleeReport attacksList;
  MeleeReport attacks;

// apply changes to stats if any (recalculate stats)

  battleInstance->recalculateStats();

  for(vector <BattleTargetElement>::iterator iter =potentialTargets.begin();
  iter != potentialTargets.end(); ++iter)
  {
    attacksList.push_back(MeleeAttackElement((*iter).instance_,0,0,0));// for finding target index only
    attacks.push_back(MeleeAttackElement());// Only targets that were subject of attacks should be listed
  }

  int totalHits =battleInstance->getFiguresNumber();


      while (totalHits)
      {
        if(potentialTargets.empty())
                      break;
        for(vector <BattleTargetElement>::iterator iter =potentialTargets.begin();
            iter != potentialTargets.end(); )
        {
            target = (*iter).instance_;
            targetSize = target->getFiguresNumber();
            unsigned int i; // index of attackMessage related to current target
            for (i= 0; i< attacksList.size(); ++i)
            {
              if(attacksList[i].target == target)
                    break;
            }

            attacks[i].target = target;
            MeleeAttackElement attackResults(0,0,0,0);
            if(totalHits > targetSize)
                    {
                        attackResults = attack(battleInstance, target, targetSize);
                        attacks[i] = attacks[i] + attackResults;
                        totalHits -=targetSize;
                        if(attackResults.killed >= targetSize)
                        {
                                potentialTargets.erase(iter);
                                continue;
                        }
                    }
            else
                    {
                        attackResults = attack(battleInstance, target, totalHits);
                        attacks[i] = attacks[i] + attackResults;
                        totalHits = 0;
                        if(attackResults.killed >= targetSize)
                        {
                                potentialTargets.erase(iter);
                                continue;
                        }
                        break;
                    }
            ++iter;
        }
      }
// Was it multihit?

    int hitNumbers = battleInstance->getHitNumbers();
		if(hitNumbers > 1)
		{
			battleInstance->processMultiHitting(hitNumbers);
		}
	return attacks;
}



MeleeAttackElement CombatActionStrategy::attack(BattleInstance * battleInstance, BattleInstance * target, int totalHits)
{
	cerr << "Illegal call to virtual function CombatActionStrategy::attack\n";
	return MeleeAttackElement(target, 0, 0, 0);
}



bool CombatActionStrategy::attackPreference( const BattleTargetElement & trg1, const BattleTargetElement & trg2)
{

/*// Sort by number times being attacked
	if(trg1.instance_->getAttackedOnRound() && !trg2.instance_->getAttackedOnRound())
		return false;
	if(!trg1.instance_->getAttackedOnRound() &&  trg2.instance_->getAttackedOnRound())
		return true;*/

// Sort by stealth
	if(trg1.instance_->getStealth() < trg2.instance_->getStealth())
		return true;
	if(trg1.instance_->getStealth() > trg2.instance_->getStealth())
		return false;

// Sort by direction
	if((trg1.battleDirection_ == FORWARD) && (trg2.battleDirection_ != FORWARD))
		return true;
	if((trg2.battleDirection_ == FORWARD) && (trg1.battleDirection_ != FORWARD))
		return false;
	if((trg1.battleDirection_ == LEFT) && (trg2.battleDirection_ != LEFT))
		return true;
	if((trg2.battleDirection_ == LEFT) && (trg1.battleDirection_ != LEFT))
		return false;
	if((trg1.battleDirection_ == RIGHT) && (trg2.battleDirection_ != RIGHT))
		return true;
	if((trg2.battleDirection_ == RIGHT) && (trg1.battleDirection_ != RIGHT))
		return false;

// sort by range
	if(trg1.range_ < trg2.range_)
		return true;

		return false;
}



int CombatActionStrategy::calculateHitNumber(int numStrikes, int att, int def)
{

	if(att <= 0)
		{
			return 0; // Never hits
		}
	if(def <= 0)
		{
			return  numStrikes; // Allways hits
		}

	int hitNums =0;
	int hitChance = 0;

	if(att > def)
	{
		hitChance = att - def + 2; // chance: (att-def)+1/((att - def + 1) + 1)
		for (int i = 0; i < numStrikes; ++i)
		{
 			if ( Roll_1Dx(hitChance)  != 0)
				hitNums++;
		}
		return hitNums;
	}
	else
	{
		hitChance = def - att + 2;
		for (int i = 0; i < numStrikes; ++i)
		{

			if ( Roll_1Dx(hitChance) == 0)
			{
				hitNums++;
			}
		}
		return hitNums;
	}
}



int CombatActionStrategy::getInitiative()
{
	int initiative = nonCumulativeStats.getInitiative();
        if(initiative == VERY_BIG_NUMBER)
	{
	initiative = modifyingStats.getInitiative();
	}
	return initiative;
}



CombatActionStrategy * CombatActionStrategy::cloneSelf()
{
 CombatActionStrategy * copyOfSelf = new CombatActionStrategy(keyword_,parent_);
 *copyOfSelf = *this;
 return copyOfSelf;
}


bool CombatActionStrategy::mayApplyAction(BattleInstance * battleInstance)
{
  if(mana_)
  {
      if(battleInstance->getMana() < mana_ )
      {
          return false;
      }

  }

    if (battleInstance->isTraced())
    {
        combatReportFile << "== TRACING " << battleInstance->print() << " resourse check.\n";

    }

    for (vector <ItemElement *>::iterator iter = resources_.begin(); iter != resources_.end(); ++iter)
    {
        if(battleInstance->hasItem((*iter)->getItemType()) < (*iter)->getItemNumber())
        {
            if (battleInstance->isTraced())
            {
                combatReportFile << "== TRACING " << battleInstance->print() << " has "
                        << battleInstance->hasItem((*iter)->getItemType()) << " of "
                        << (*iter)->getItemType()->printTag()
                        << " ==> required for applying " << print() << "\n";
            }
        return false;
        }
    }
    return true;
}

void CombatActionStrategy::consumeActionResources(BattleInstance * battleInstance)
{
    if (mana_)
    {
        battleInstance->setMana(battleInstance->getMana() - mana_);
    }

    for (vector <ItemElement *>::iterator iter = resources_.begin(); iter != resources_.end(); ++iter)
    {
        battleInstance->takeItemOut((*iter)->getItemType(), (*iter)->getItemNumber());
    }
}



void    CombatActionStrategy::extractKnowledge (Entity * recipient, int parameter)
{
  if(action_)
  {
    if(recipient->addKnowledge(action_))
      action_->extractKnowledge(recipient);
  }
  for(vector <ItemElement *>::iterator iter = resources_.begin(); iter != resources_.end(); ++iter)
    {
      if(recipient->addKnowledge((*iter)->getItemType()))
        (*iter)->getItemType()->extractKnowledge(recipient);
    }
//  if(expGainingSkill_) //causes loop
//     expGainingSkill_->extractKnowledge(recipient);

}