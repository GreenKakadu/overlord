/***************************************************************************
                          LocalMarketRequest.h
                             -------------------
    begin                : Tue Jun 24 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef LOCAL_MARKET_REQUEST_H
#define LOCAL_MARKET_REQUEST_H

#include "MarketRequest.h"
class FactionEntity;
/**
  *@author Alex Dribin
  */

class LocalMarketRequest : public MarketRequest  {
public: 
	LocalMarketRequest(int amount, ItemRule * item,  int price, MARKET_OFFER type);
	~LocalMarketRequest();
   string print();
   virtual vector <AbstractData *> aPrint();
   bool isValid();
   virtual void  produceFactionReport(FactionEntity * faction, ostream &out);
   void free();
    void answerMarketRequest(int price, int  amount);
   virtual void save(ostream &out);
protected:
   int initialAmount_;
   int value_;
};

#endif
