/***************************************************************************
                          MarketRequest.cpp 
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
#include "UnitEntity.h"
#include "ItemRule.h"
#include "IntegerData.h"
#include "ItemElementData.h"
#include "MarketRequest.h"
#include "QuartenaryPattern.h"
extern Reporter * buyReporter;
extern Reporter * sellReporter;
extern ItemRule * cash;
MarketRequest::MarketRequest(UnitEntity * unit, Order * orderId,
                              int amount,
                              ItemRule * item,
                              int price, MARKET_OFFER type): BasicCompetitiveRequest( unit, orderId)
{
  amount_ = amount;
  item_ = item;
  price_ = price;
  type_ = type;
}



MarketRequest::~MarketRequest(){
}

string MarketRequest::print()
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
    
  return  unit_->printName() + " requests to " + operationName + buffer1 +
          " of " + item_->printName() + " for " + buffer2  + " coins\n";
}

AbstractData * MarketRequest::getType() const
{
  return item_;
}
bool MarketRequest::isEqualTo(BasicCompetitiveRequest * request)
{

//  cout << "Comparing "<< this->getType()->printName() << " and " <<request->getType()->printName()<<endl;
//  cout << "MarketRequest::Comparing: "<< this->print() << " and " <<request->print()<<endl;
//  if (this->getType())
//     cout << this->getType()->printName();
//      else cout <<" 0 ";
//  if (request->getType())
//     cout << request->getType()->printName();
//      else cout <<" 0 ";
//  if (this->getType() == request->getType())
//      cout << " True "<<endl;
//      else
//      cout << " false "<<endl;
  return (this->getType() == request->getType());
}
bool MarketRequest::isBuying() const
{
  return (type_ == BUY);
}

bool MarketRequest::isSelling() const
{
  return (type_ == SELL);
}

bool MarketRequest::isValid()
{
  if(unit_ == 0)
    return false;
  if(unit_->getLocation()== 0)  // Dead
    return false;
  if (type_ == BUY)
  {
    if (unit_->hasMoney() >= price_ * amount_)
      return true;
    else
      return false;  
  } 
  if (type_ == SELL)
  {
    if(item_ == 0)
      return false;
    if ( unit_->hasItem(item_) >= amount_)
      return true;
    else
      return false;    
  }
      return false;
}
//bool MarketRequest::isGreaterThan(BasicCompetitiveRequest& req)
////bool MarketRequest::isGreaterThan(MarketRequest& request)
//{
//  MarketRequest * request = dynamic_cast<MarketRequest *>(&req);
//  assert(request);
//  if(price_ < request->getPrice())
//    return false;
//  if(price_ > request->getPrice())
//    return true;
//  // apply marketing skill here
//    return true;
//}
void MarketRequest::answerMarketRequest(int price, int amount)
{
  if (type_ == BUY)
  {
    if (unit_->isTraced())
      cout <<"== TRACING " << unit_->printName()<< " buys " << amount << " of " <<  item_->getName() << " for " << price << " coins.\n";
    unit_->addToInventory(item_,amount);
    assert(unit_->takeFromInventoryExactly(cash, price * amount));
    // report
    unit_->addReport(new QuartenaryPattern(buyReporter, unit_,
                    new ItemElementData(item_ , amount),
                    new IntegerData(price),new IntegerData(price * amount)));
    // finish order processing  updateOrderResults
     orderId_->completeProcessing(unit_,amount);
    return;
  }
  if (type_ == SELL)
  {
    if (unit_->isTraced())
        cout <<"== TRACING " << unit_->printName()<< " sells " << amount << " of " <<  item_->getName() << " for " << price << " coins.\n";
    assert(unit_->takeFromInventoryExactly(item_,amount));
    unit_->addToInventory(cash, price * amount);
    unit_->addReport(new QuartenaryPattern(sellReporter, unit_,
                    new ItemElementData(item_ , amount),
                    new IntegerData(price),new IntegerData(price * amount)));
     orderId_->completeProcessing(unit_,amount);
    return;
  }
}
