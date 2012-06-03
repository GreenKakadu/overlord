/***************************************************************************
                          LocalMarketRequest.cpp 
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
#include "LocalMarketRequest.h"
#include "ItemRule.h"
#include "FactionEntity.h"
#include "ItemElement.h"

extern string longtostr(long u);

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

  return  string("Local request to ") + operationName + longtostr(amount_) +
          " of " + item_->print() + " for " + longtostr(price_)  + " coins\n";
}

vector <AbstractData *> LocalMarketRequest::aPrint()
{
    vector <AbstractData *> out;
    out.push_back(new ItemElement(item_,initialAmount_));
    out.push_back(new StringData(" at $"));
    out.push_back(new IntegerData(price_));
    out.push_back(new StringData("."));
    return out;
}

void  LocalMarketRequest::produceFactionReport(FactionEntity * faction, ostream &out)
{
  out << ItemElement(item_,initialAmount_).print() << " at $"<< price_;
//  out << amount_ << " " << item_->print() << " at $"<< price_;
  faction->addKnowledge(item_);
}

// Local request is allways valid
bool LocalMarketRequest::isValid()
{
    if(amount_)
        return true;
    else
        return false;
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
