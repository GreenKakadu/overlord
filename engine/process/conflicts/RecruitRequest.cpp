/***************************************************************************
                          RecruitRequest.cpp 
                             -------------------
    begin                : Wed Jul 2 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "RecruitRequest.h"
#include "RaceRule.h"
#include "IntegerData.h"
#include "RaceElement.h"
#include "QuartenaryPattern.h"
#include "BinaryPattern.h"

extern Reporter * recruiterReporter;
extern Reporter * recruitedReporter;

RecruitRequest::RecruitRequest(UnitEntity * unit, OrderLine * orderId,
                              int amount, RaceRule * race, int price,
                              UnitEntity * targetUnit):
                  MarketRequest( unit, orderId, amount, 0, price,BUY)
{
  race_ = race;
  targetUnit_ = targetUnit;
}

RecruitRequest::~RecruitRequest(){
}

string RecruitRequest::print()
{
  return  unit_->print() + " requests to recruit " + longtostr(amount_) +
          " of " + race_->print() + " for " + longtostr(price_)  + " coins\n";
}

bool RecruitRequest::isValid()
{
  if(unit_ == 0)
    return false;

  if(targetUnit_ == 0)
    return false;

  if(unit_->getLocation()== 0)  // Dead
    return false;

  if(targetUnit_->getLocation()== 0)  // Dead
    return false;


    if (unit_->hasMoney() >= price_ * amount_)
      return true;
    else
      return false;
  
}

AbstractData * RecruitRequest::getType() const
{
  return race_;
}
void RecruitRequest::answerMarketRequest(int price, int amount)
{

  if (unit_->isTraced())
      cout<<"== TRACING "  << unit_->print()<< " recruits " << amount << " of " <<  race_->getName() << " for " << price << " coins.\n";
   targetUnit_ ->addNewFigures(amount);
    int taken = unit_->takeFromInventory(cash, price * amount); //pay
    assert(taken == price * amount);
    // report to targetUnit_
//QQQ
    targetUnit_ ->addReport(new BinaryPattern(recruitedReporter,
                            new RaceElement(race_ , amount), targetUnit_));
    unit_->addReport(new QuartenaryPattern(recruiterReporter, unit_,
                    new RaceElement(race_ , amount),
                    new IntegerData(price),new IntegerData(price * amount)));
    // finish order processing  updateOrderResults
     orderId_->completeProcessing(unit_,amount);
    return;

}
