/***************************************************************************
                          MarketRequest.h
                             -------------------
    begin                : Wed Jun 18 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef MARKET_REQUEST_H
#define MARKET_REQUEST_H

#include "BasicCompetitiveRequest.h"

/**
  *@author Alex Dribin
  */
class UnitEntity;
class ItemRule;
//#include "SystemTypes.h"
enum market_offer {BUY = 0, SELL  =1};
typedef enum market_offer MARKET_OFFER;


class MarketRequest : public BasicCompetitiveRequest  {
public: 
	MarketRequest(UnitEntity * unit, OrderLine * orderId, int amount, ItemRule * item,  int price, MARKET_OFFER type);
	~MarketRequest();
       string print();
 AbstractData * getType() const;
//  inline  Rational  getValue() const {cout << "---- basic get"<<endl;return amount_;}
        bool isSelling() const;
        bool isBuying() const;
        bool isValid();
//        bool isGreaterThan(BasicCompetitiveRequest& request);
//        bool isGreaterThan(MarketRequest& request);
 virtual void answerMarketRequest(int price, int amount);
 inline int getPrice() const {return price_;}
 inline int getAmount() const {return amount_;}
  virtual       bool isEqualTo (BasicCompetitiveRequest * request);
protected:
  int amount_;
  ItemRule * item_;
  int price_;
  MARKET_OFFER type_;
};

#endif
