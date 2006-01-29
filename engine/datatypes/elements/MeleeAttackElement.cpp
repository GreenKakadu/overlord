/***************************************************************************
                          MeleeAttackElement.cpp
                             -------------------
    begin                : 30 Mar 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "MeleeAttackElement.h"
#include "BattleInstance.h"

MeleeAttackElement::MeleeAttackElement(BattleInstance * targ, int numStrikes, int dam, int died)
{
	target = targ;
	hits = numStrikes;
	damage = dam;
	killed = died;
}


MeleeAttackElement::MeleeAttackElement()
{
	target = 0;
	hits = 0;
	damage = 0;
	killed = 0;
}


MeleeAttackElement::~MeleeAttackElement(){
}

const MeleeAttackElement operator + (const MeleeAttackElement& leftArgument, const MeleeAttackElement& rightArgument)
{
	return MeleeAttackElement(leftArgument.target,
	leftArgument.hits + rightArgument.hits,
	leftArgument.damage + rightArgument.damage,
	leftArgument.killed + rightArgument.killed);
}


