/***************************************************************************
                          UseCombatOrder.h
                             -------------------
    begin                : Wed Apr 20 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef USE_COMBAT_ORDER_H
#define USE_COMBAT_ORDER_H

#include "CombatOrderPrototype.h"

/**
  *@author Alex Dribin
  */
//class ReportPattern;
class AbstractData;

class UseCombatOrder : public CombatOrderPrototype  {
public:
	UseCombatOrder();
	~UseCombatOrder(){}
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
    protected:
};

#endif
