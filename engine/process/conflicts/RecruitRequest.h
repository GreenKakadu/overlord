/***************************************************************************
                          RecruitRequest.h
                             -------------------
    begin                : Wed Jul 2 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#ifndef RECRUIT_REQUEST_H
#define RECRUIT_REQUEST_H

#include "MarketRequest.h"
class RaceRule;

/**
  *@author Alex Dribin
  */

class RecruitRequest : public MarketRequest  {
public: 
	RecruitRequest(UnitEntity * unit, Order * orderId, int amount, 
                              RaceRule * race, int price, UnitEntity * targetUnit);
	~RecruitRequest();
 AbstractData * getType() const;
 void answerMarketRequest(int price, int amount);
 string print();
 bool isValid();
protected:
  RaceRule * race_;
  UnitEntity * targetUnit_;
};

#endif
