/***************************************************************************
                          MarketStrategy.h
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

#ifndef MARKET_STRATEGY_H
#define MARKET_STRATEGY_H
#include <vector>
#include "Strategy.h"
class MarketConflict;
class MarketRequest;
class LocalMarketRequest;
class LocationEntity;
class FactionEntity;
class UnitEntity;
class ItemRule;
class RaceRule;

/**
  *@author Alex Dribin
  */

class MarketStrategy : public Strategy  {
public: 
	MarketStrategy( const string & keyword, GameData * parent): Strategy(keyword, parent){};
	MarketStrategy( const MarketStrategy * prototype );
	~MarketStrategy(){}
   STATUS     initialize      ( Parser *parser);
   GameData * createInstanceOfSelf();
   void addRequest(MarketRequest * request);
   void process();
   void dailyPreProcess();
   void  save(ostream &out);
   void produceFactionReport(FactionEntity * faction, ostream &out);
   vector<AbstractArray> aPrintReport();
   vector<AbstractArray> aPrintRecruitReport();
   vector<AbstractArray> aPrintSaleReport();
   vector<AbstractArray> aPrintBuyReport();
   void setLocation(LocationEntity * location);
   void setMerchantPrince(UnitEntity *prince){ merchantPrince_ = prince;}
   UnitEntity * getMerchantPrince() const {return merchantPrince_;}
   int getLocalBuyPrice(ItemRule * item);
   int getLocalSellPrice(ItemRule * item);
   int getLocalRecruitPrice(RaceRule * race);
   void extractAndAddKnowledge(FactionEntity * recipient, int parameter = 0);
   inline vector <LocalMarketRequest *> getAllBuyRequests(){return buyRequests_;}
   inline vector <LocalMarketRequest *> getAllSellRequests(){return sellRequests_;}
   inline vector <LocalMarketRequest *> getAllRecruitRequests(){return recruitRequests_;}
protected:
   MarketConflict * marketConflict_;
   vector <LocalMarketRequest *> sellRequests_;
   vector <LocalMarketRequest *> buyRequests_;
   vector <LocalMarketRequest *> recruitRequests_;
   LocationEntity * location_;
   UnitEntity * merchantPrince_;
};
typedef vector<LocalMarketRequest *>::iterator RequestIterator;
extern MarketStrategy     sampleMarket;
#endif
