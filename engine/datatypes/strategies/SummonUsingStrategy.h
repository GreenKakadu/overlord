/***************************************************************************
                          SummonUsingStrategy.h 
                             -------------------
    begin                : Thu Feb 20 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef SUMMON_USING_H
#define SUMMON_USING_H

#include "BasicUsingStrategy.h"

/**Summoning
  *@author Alex Dribin
  */

class SummonUsingStrategy : public BasicUsingStrategy  {
public: 
      SummonUsingStrategy ( const string & keyword, GameData * parent): BasicUsingStrategy(keyword, parent){}
      SummonUsingStrategy ( const SummonUsingStrategy * prototype ): BasicUsingStrategy(prototype){}
		 ~SummonUsingStrategy(){};
};
extern SummonUsingStrategy      sampleSummonUsing;

#endif
