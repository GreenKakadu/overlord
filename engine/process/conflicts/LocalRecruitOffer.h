/***************************************************************************
                          LocalRecruitOffer.h
                             -------------------
    begin                : Wed Jul 2 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#ifndef LOCAL_RECRUIT_OFFER_H
#define LOCAL_RECRUIT_OFFER_H

#include "LocalMarketRequest.h"
class RaceRule;

/**
  *@author Alex Dribin
  */

class LocalRecruitOffer : public LocalMarketRequest  {
public: 
	LocalRecruitOffer(int amount, RaceRule * race, int price);
	~LocalRecruitOffer();
   AbstractData * getType() const;
   string print();
   vector <AbstractData *> aPrint();
   void produceFactionReport(FactionEntity * faction, ostream &out);
   void answerMarketRequest(int price, int  amount);
   void save(ostream &out);
protected:
  RaceRule * race_;
};

#endif
