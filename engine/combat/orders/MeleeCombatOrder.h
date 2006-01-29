/***************************************************************************
                          MeleeCombatOrder.h
                             -------------------
    begin                : Thu Nov 16 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef MELEE_COMBAT_ORDER_H
#define MELEE_COMBAT_ORDER_H

#include "CombatOrderPrototype.h"


/**
  *@author Alex Dribin
  */

class AbstractData;

class MeleeCombatOrder : public CombatOrderPrototype  {
public:
	MeleeCombatOrder();
	~MeleeCombatOrder(){}
  STATUS loadParameters(Parser *parser,ParameterList &parameters,Entity *entity);
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
	bool evaluate(Entity * entity, ParameterList  &parameters);

	protected:
};

#endif
