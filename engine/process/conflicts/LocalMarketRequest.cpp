/***************************************************************************
                          LocalMarketRequest.cpp 
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
#include "LocalMarketRequest.h"
#include "ItemRule.h"
#include "FactionEntity.h"
#include "ItemElement.h"

extern void longtostr(unsigned long u, char *out);

LocalMarketRequest::LocalMarketRequest(int amount,ItemRule * item,
  int price, MARKET_OFFER type): MarketRequest(0,0,amount,item,price,type)
{
  initialAmount_ = amount;
  value_ = 0;
}



LocalMarketRequest::~LocalMarketRequest(){
}



string LocalMarketRequest::print()
{
   string operationName;
  if (type_ == BUY)
  {
    operationName = " buy ";
  }
  else
    operationName = " sell ";
  char buffer1[12];
  char buffer2[12];
  longtostr(amount_, buffer1);
  longtostr(price_, buffer2);

  return  string("Local request to ") + operationName + buffer1 +
          " of " + item_->printName() + " for " + buffer2  + "coins\n";
}


void  LocalMarketRequest::report(FactionEntity * faction, ostream &out)
{
  out << ItemElement(item_,initialAmount_).printName() << " at $"<< price_;
//  out << amount_ << " " << item_->printName() << " at $"<< price_;
  faction->addKnowledge(item_);
}

// Local request is allways valid
bool LocalMarketRequest::isValid()
{
  
  return amount_;
}

void LocalMarketRequest::free()
{
  // Local request are not deleted after first processing
}


void LocalMarketRequest::answerMarketRequest(int price, int  amount)
{
  amount_ -= amount;
  value_ += amount * price;
//cout <<" report  local market operations: ("<<amount_<<")["<< print()<<"] left.\n";
}



void  LocalMarketRequest::save(ostream &out)
{
  if (type_ == BUY)
  {
    out << "BUY ";
   }
  else
    {
    out << "SELL ";
   }
    out <<initialAmount_<<" "<<item_->getTag()<<" "<<price_<<endl;
}
