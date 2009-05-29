/***************************************************************************
                          RecruitRequest.cpp
                             -------------------
    begin                : Wed Jul 2 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "LocationEntity.h"
#include "FactionEntity.h"
#include "RecruitRequest.h"
#include "RaceRule.h"
#include "IntegerData.h"
#include "RaceElement.h"
#include "QuartenaryMessage.h"
#include "BinaryMessage.h"

extern ReportPattern * recruiterReporter;
extern ReportPattern * recruitedReporter;
ReportPattern *	 recruitingNotPermittedReporter  = new ReportPattern("recruitingNotPermittedReporter");

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

  LocationEntity * location = unit_->getLocation();
  if(location== 0)  // Dead
    return false;

  if(targetUnit_->getLocation()== 0)  // Dead
    return false;

  FactionEntity * owner = location->getRealOwner();
    if(owner)
    {
      if(!owner->stanceAtLeast(unit_,
              location->getOwnershipPolicy().getRecruitPermission(race_)))
              {
              unit_->addReport(new BinaryMessage(recruitingNotPermittedReporter,race_, location),0,0);
              return false;
              }
    }

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

/*  if (unit_->isTraced())
      cout<<"== TRACING "  << unit_->print()<< " recruits " << amount << " of " <<  race_->getName() << " for " << price << " coins.\n";*/
   targetUnit_ ->addNewFigures(amount);
    int taken = unit_->takeFromInventory(cash, price * amount); //pay
    assert(taken == price * amount);
    // report to targetUnit_
//QQQ
    targetUnit_ ->addReport(new BinaryMessage(recruitedReporter,
                            new RaceElement(race_ , amount), targetUnit_));
    unit_->addReport(new QuartenaryMessage(recruiterReporter, unit_,
                    new RaceElement(race_ , amount),
                    new IntegerData(price),new IntegerData(price * amount)));
    // finish order processing  updateOrderResults
     orderId_->completeProcessing(unit_,amount);
    return;

}
