/***************************************************************************
                          CombatUsingStrategy.h 
                             -------------------
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef COMBAT_USING_H
#define COMBAT_USING_H

#include "BasicUsingStrategy.h"

/**
  *@author Alex Dribin
  */

class CombatUsingStrategy : public BasicUsingStrategy  {
public: 
      CombatUsingStrategy ( const string & keyword, GameData * parent): BasicUsingStrategy(keyword, parent){}
      CombatUsingStrategy ( const CombatUsingStrategy * prototype ): BasicUsingStrategy(prototype){}
		 ~CombatUsingStrategy(){};
      GameData * createInstanceOfSelf();
      STATUS initialize        ( Parser *parser );
      bool use(UnitEntity * unit, Order * OrderId);
};

#endif
