/***************************************************************************
                          BuildUsingStrategy.h 
                             -------------------
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef BUILD_USING_H
#define BUILD_USING_H

#include "BasicUsingStrategy.h"

/**Building
  *@author Alex Dribin
  */

class BuildUsingStrategy : public BasicUsingStrategy  {
public: 
      BuildUsingStrategy ( const string & keyword, GameData * parent): BasicUsingStrategy(keyword, parent){}
      BuildUsingStrategy ( const BuildUsingStrategy * prototype ): BasicUsingStrategy(prototype){}
		 ~BuildUsingStrategy(){};
      GameData * createInstanceOfSelf();
      STATUS initialize        ( Parser *parser );
      bool use(UnitEntity * unit, Order * OrderId);
};

#endif
