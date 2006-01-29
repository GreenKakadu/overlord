/***************************************************************************
                          AdvanceCombatOrder.h
                             -------------------
    begin                : Thu Nov 09 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef ADVANCE_COMBAT_ORDER_H
#define ADVANCE_COMBAT_ORDER_H

#include "CombatOrderPrototype.h"

/**
  *@author Alex Dribin
  */
//class ReportPattern;
class AbstractData;

class AdvanceCombatOrder : public CombatOrderPrototype  {
public:
	AdvanceCombatOrder();
	~AdvanceCombatOrder(){}
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
    protected:
};

#endif
