/***************************************************************************
                      BattleUnit.cpp
               Instance taking part in combat
                          ------------------
    begin                : Mon Nov 01 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "BattleUnit.h"
#include "UnitEntity.h"

BattleUnit::BattleUnit(UnitEntity * origin, CombatReport * report): BattleInstance(origin,report)
{
	positionRank_ = 0;
  positionFile_ = 0;
	origin_ = origin;
	// assign orders
}



BattleUnit::~BattleUnit()
{
}



bool BattleUnit::isAlive()
{
	return getLife();
}



int BattleUnit::getLife()
{
	return origin_->getLife();
}



int BattleUnit::getFigures()
{
	return origin_->getFiguresNumber();
}
