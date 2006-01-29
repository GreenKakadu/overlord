/***************************************************************************
                          ParryCombatOrder.h
                             -------------------
    begin                : Thu Nov 16 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef PARRY_COMBAT_ORDER_H
#define PARRY_COMBAT_ORDER_H

#include "CombatOrderPrototype.h"

/**
  *@author Alex Dribin
  */
//class ReportPattern;
class AbstractData;

class ParryCombatOrder : public CombatOrderPrototype  {
public:
	ParryCombatOrder();
	~ParryCombatOrder(){}
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
    protected:
};

#endif
