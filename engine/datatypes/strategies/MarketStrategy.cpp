/***************************************************************************
                          MarketStrategy.cpp 
                             -------------------
    begin                : Tue Jun 24 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "MarketStrategy.h"
#include "MarketConflict.h"
#include "LocalMarketRequest.h"
#include "ItemRule.h"
#include "RaceRule.h"
#include "RulesCollection.h"
#include "LocalRecruitOffer.h"
#include "FactionEntity.h"
#include "EntitiesCollection.h"
#include "UnitEntity.h"

MarketStrategy            sampleMarket            ("MARKET",             &sampleGameData);

MarketStrategy::MarketStrategy( const  MarketStrategy* prototype ) : Strategy(prototype)
{
   marketConflict_ = 0;
   location_ = 0;
   merchantPrince_ = 0;

}



GameData * MarketStrategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<MarketStrategy> (this);
}



STATUS
MarketStrategy::initialize        ( Parser *parser )
{
  if (parser->matchKeyword ("SELL") )
    {
      int amount = parser->getInteger();
      ItemRule * item = items[parser->getWord()];
      int price = parser->getInteger();
      if(amount && item && price)
      {
        LocalMarketRequest * newRequest = new LocalMarketRequest(amount,item,price,SELL);
        sellRequests_.push_back(newRequest);
//        marketConflict_->addRequest(newRequest);
      }
      return OK;
    }
  if (parser->matchKeyword ("BUY") )
    {
      int amount = parser->getInteger();
      ItemRule * item = items[parser->getWord()];
      int price = parser->getInteger();
      if(amount && item && price)
      {
        LocalMarketRequest * newRequest = new LocalMarketRequest(amount,item,price,BUY);
        buyRequests_.push_back(newRequest);
//        marketConflict_->addRequest(newRequest);
      }
      return OK;
    }
  if (parser->matchKeyword ("RECRUIT") )
    {
      int amount = parser->getInteger();
      RaceRule * race = races[parser->getWord()];
      int price = parser->getInteger();
      if(/*amount && */race && price)
      {
        LocalRecruitOffer * newRequest = new LocalRecruitOffer(amount,race,price);
        recruitRequests_.push_back(newRequest);
     }
      return OK;
    }
  if (parser->matchKeyword ("PRINCE") )
    {
      merchantPrince_ = units[parser->getWord()];
      return OK;
    }
      return OK;
}



void MarketStrategy::addRequest(MarketRequest * request)
{
        marketConflict_->addRequest(request);
}



void MarketStrategy::process()
{
  marketConflict_->process();
}



void  MarketStrategy::save(ostream &out)
{
 RequestIterator iter;
  out <<"MARKET_TYPE " << keyword_<<endl;
 for( iter = sellRequests_.begin(); iter != sellRequests_.end(); ++iter)
 {
   (*iter)->save(out);
 }

 for( iter = buyRequests_.begin(); iter != buyRequests_.end(); ++iter)
 {
   (*iter)->save(out);
 }

 for( iter = recruitRequests_.begin(); iter != recruitRequests_.end(); ++iter)
 {
   (*iter)->save(out);
 }

 if(merchantPrince_)
  out << "PRINCE "<< merchantPrince_->getTag()<<endl;
}
  


void MarketStrategy::dailyPreProcess()
{
 RequestIterator iter;
  // add local requests
 for( iter = sellRequests_.begin(); iter != sellRequests_.end(); ++iter)
 {
        marketConflict_->addRequest(*iter);
 }

 for( iter = buyRequests_.begin(); iter != buyRequests_.end(); ++iter)
 {
        marketConflict_->addRequest(*iter);
 }

 for( iter = recruitRequests_.begin(); iter != recruitRequests_.end(); ++iter)
 {
        marketConflict_->addRequest(*iter);
 }
}



void MarketStrategy::setLocation(LocationEntity * location)
{
  location_ = location;
  marketConflict_  = new MarketConflict(location_);
}



void  MarketStrategy::produceFactionReport(FactionEntity * faction, ostream &out)
{
 RequestIterator iter;
 bool isFirst = true;

 if(!recruitRequests_.empty())
 {
      out <<     "For hire: ";
  for( iter = recruitRequests_.begin(); iter != recruitRequests_.end(); ++iter)
    {
      if( isFirst)
      {
        isFirst = false;
      }
      else
        {
          out << ", ";
        }
        (*iter)->produceFactionReport(faction, out);
        
    }
  out << ".\n";
 }

 if(!sellRequests_.empty())
 {
    isFirst = true;
    out <<     "For sale: ";
  for( iter = sellRequests_.begin(); iter != sellRequests_.end(); ++iter)
    {
      if( isFirst)
      {
        isFirst = false;
      }
      else
        {
          out << ", ";
        }
        (*iter)->produceFactionReport(faction, out);
    }
  out << ".\n";
  }

 if(!buyRequests_.empty())
 {
  isFirst = true;
  out <<     "Buying: ";
  for( iter = buyRequests_.begin(); iter != buyRequests_.end(); ++iter)
    {
      if( isFirst)
      {
        isFirst = false;
      }
      else
        {
          out << ", ";
        }
        (*iter)->produceFactionReport(faction, out);
    }
  out << ".\n";
  }
 
}
