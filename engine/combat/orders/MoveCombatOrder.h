/***************************************************************************
                          MoveCombatOrder.h
                             -------------------
    begin                : Dec 08 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef MOVE_COMBAT_ORDER_H
#define MOVE_COMBAT_ORDER_H

#include "CombatOrderPrototype.h"

/**
  *@author Alex Dribin
  */
class ReportPattern;
//class AbstractData;

class MoveCombatOrder : public CombatOrderPrototype  {
public:
	MoveCombatOrder();
	~MoveCombatOrder(){}
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
    protected:
};

#endif
