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
#include "ItemElement.h"
#include "IntegerData.h"
#include "MarketRequest.h"
#include "QuartenaryMessage.h"
extern ReportPattern * buyReporter;
extern ReportPattern * sellReporter;
extern ItemRule * cash;
MarketRequest::MarketRequest(UnitEntity * unit, OrderLine * orderId,
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


  return  unit_->print() + " requests to " + operationName + longtostr(amount_) +
          " of " + item_->print() + " for " + longtostr(price_)  + " coins\n";
}

AbstractData * MarketRequest::getType() const
{
  return item_;
}
bool MarketRequest::isEqualTo(BasicCompetitiveRequest * request)
{

//  cout << "Comparing "<< this->getType()->print() << " and " <<request->getType()->print()<<endl;
//  cout << "MarketRequest::Comparing: "<< this->print() << " and " <<request->print()<<endl;
//  if (this->getType())
//     cout << this->getType()->print();
//      else cout <<" 0 ";
//  if (request->getType())
//     cout << request->getType()->print();
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
      cout <<"== TRACING " << unit_->print()<< " buys " << amount << " of " <<  item_->getName() << " for " << price << " coins.\n";
    unit_->addToInventory(item_,amount);
    int taken = unit_->takeFromInventory(cash, price * amount);  // pay
    assert(taken == price * amount);
    // report
//QQQ
    unit_->addReport(new QuartenaryMessage(buyReporter, unit_,
                    new ItemElement(item_ , amount),
                    new IntegerData(price),new IntegerData(price * amount)));
    // finish order processing  updateOrderResults
     orderId_->completeProcessing(unit_,amount);
    return;
  }
  if (type_ == SELL)
  {
    if (unit_->isTraced())
        cout <<"== TRACING " << unit_->print()<< " sells " << amount << " of " <<  item_->getName() << " for " << price << " coins.\n";
    int taken = unit_->takeFromInventory(item_,amount);
    assert(taken == amount);
    unit_->addToInventory(cash, price * amount);
//QQQ
    unit_->addReport(new QuartenaryMessage(sellReporter, unit_,
                    new ItemElement(item_ , amount),
                    new IntegerData(price),new IntegerData(price * amount)));
     orderId_->completeProcessing(unit_,amount);
    return;
  }
}
