/***************************************************************************
                          NewRecruitRequest.h
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
#ifndef NEW_RECRUIT_REQUEST_H
#define NEW_RECRUIT_REQUEST_H

#include "MarketRequest.h"
class NewEntityPlaceholder;
class RaceRule;

/**
  *@author Alex Dribin
  */

class NewRecruitRequest : public MarketRequest  {
public: 
	NewRecruitRequest(UnitEntity * unit, Order * orderId, int amount,
                              RaceRule * race, int price, NewEntityPlaceholder * targetUnit);
	~NewRecruitRequest();
 AbstractData * getType() const;
 void answerMarketRequest(int price, int amount);
 string print();
 bool isValid();
protected:
  RaceRule * race_;
  NewEntityPlaceholder * targetUnit_;
};

#endif
