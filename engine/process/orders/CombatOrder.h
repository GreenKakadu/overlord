/***************************************************************************
                          CombatOrder.h
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef COMBAT_ORDER_H
#define COMBAT_ORDER_H

#include "OrderPrototype.h"
#include <string>
/**
  *@author Alex Dribin
  */
class ReportPattern;
class AbstractData;
class TokenEntity;

class CombatOrder : public OrderPrototype  {
public:
	CombatOrder();
	~CombatOrder(){}
  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
  static bool checkCombatAction(string & keyword,TokenEntity * entity, bool noReport = true);
    protected:
};

#endif
