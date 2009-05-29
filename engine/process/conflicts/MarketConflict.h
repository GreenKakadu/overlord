/***************************************************************************
                          MarketConflict.h
                             -------------------
    begin                : Wed Jun 18 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef MARKET_CONFLICT_H
#define MARKET_CONFLICT_H

#include "BasicConflict.h"
#include "OverlordTypes.h"
class LocationEntity;
class MarketRequest;
class Parser;

/**
  *@author Alex Dribin
  */

class MarketConflict : public BasicConflict  {
public: 
	MarketConflict(LocationEntity * location);
	~MarketConflict();
  void addRequest(BasicCompetitiveRequest *request);
  void resolve (vector  <BasicCompetitiveRequest *> & currentList);
   int maySellByPrice(vector  <MarketRequest *> & sellOffers, int currentPrice);
   int mayBuyByPrice(vector  <MarketRequest *> & buyOffers, int currentPrice);
   int calculateFinalPrice(vector  <MarketRequest *> & buyOffers,
                           vector  <MarketRequest *> & sellOffers);
protected:  
    vector  <MarketRequest *> buyRequests_;
    vector  <MarketRequest *> sellRequests_;
};
bool offerNotLessThan(const MarketRequest* request1,
                      const MarketRequest* request2);
inline int MIN(int a, int b) { return a > b ? b : a;}

#endif
