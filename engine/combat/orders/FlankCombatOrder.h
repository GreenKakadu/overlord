/***************************************************************************
                          FlankCombatOrder.h
                             -------------------
    begin                : Wed Apr 20 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef FLANK_COMBAT_ORDER_H
#define FLANK_COMBAT_ORDER_H

#include "CombatOrderPrototype.h"

/**
  *@author Alex Dribin
  */
//class ReportPattern;
class AbstractData;

class FlankCombatOrder : public CombatOrderPrototype  {
public:
	FlankCombatOrder();
	~FlankCombatOrder(){}
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
    protected:
};

#endif
