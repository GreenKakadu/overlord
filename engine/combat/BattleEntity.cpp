/***************************************************************************
                           BattleEntity.cpp
                             -------------------
    begin                : May 22 2006
    copyright            : (C) 2006 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "BattleEntity.h"
#include "TargetOrder.h"
#include "CombatOrderLine.h"
#include "RaceRule.h"
#include "BattleInstance.h"
#include "BattleField.h"

BattleEntity sampleBattleEntity  ("BATTLE_ENTITY",  &sampleTokenEntity);

BattleEntity::BattleEntity (const string & keyword, GameData * parent ) : UnitEntity(keyword, parent)
{
  //race_ = 0;
//	figures_ = 0;
}



BattleEntity::BattleEntity(const BattleEntity * prototype): UnitEntity(prototype)
{

  //race_ = 0;
//	figures_ = 0;
}







STATUS BattleEntity::initialize        ( Parser *parser )
{


 if (parser->matchKeyword("TARGET"))
    {
      string tag = parser->getWord();
      if (tag.size() != 0)
        {
          target_ = TargetOrder::findTarget(tag);
        }
      return OK;
    }

  if (parser->matchKeyword("COMBAT_ORDER"))
 	{
	  combatOrders_.push_back(new CombatOrderLine(parser->getText(),this));
 	}
   if (parser->matchKeyword("COMBAT"))
    {
            addCombatSetting(parser->getText());
    }

 skills_.initialize(parser);
 inventory_.initialize(parser);
  combatTactics_.initialize(parser);
  return Entity::initialize(parser);

}



// Simplified Copy constructor
BattleEntity * BattleEntity::makeCopy()
{
	BattleEntity * newCopy = new BattleEntity(this);

  newCopy->getAllInventory() = getAllInventory();
  newCopy->getAllSkills() = getAllSkills();

  newCopy->getStats()->addStats(getStats());

  newCopy->getCombatOrderList() = getCombatOrderList();
  // copy race and figures
  newCopy->setRace(getRace(),getFiguresNumber());
//  if(getRace())
//  {
//  cout<<" Creating new BattleEntity "<< getRace()->print()<< " "<<getFiguresNumber()<<" dam: " <<getDamage() ;//<< " -> "<<newCopy->getDamage() <<endl;
//
//  }
//  else
//  {
//  cout<<" Creating new BattleEntity NO RACE! "<<getFiguresNumber()<<endl;
//
//  }
  //newCopy->setFigures(getFigures());
	
	newCopy->setCombatFile(getCombatFile());
	newCopy->setCombatRank(getCombatRank());
	newCopy->setCombatMove(getCombatMove());
	newCopy->setCombatStance(getCombatStance());

  newCopy->setTarget(getTarget());
  newCopy->recalculateStats();
//  if(getRace())
//  {
//  cout<< " -> "<<newCopy->getDamage() <<endl;
//
//  }
  //newCopy->traced_ = true;//isTraced();
  return newCopy;
}



// Mana DO NOT need speciat treat. Stats contain max mana and actual mana is in mana_
void BattleEntity::recalculateStats()
{
  stats.clearStats();
 // Race
  stats.addStats(getRace()->getStats());
 // skills
	skills_.addStats(&stats);
	inventory_.addStats(&stats,getFiguresNumber());
}

