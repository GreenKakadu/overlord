/***************************************************************************
                          NewRecruitRequest.cpp 
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
#include "IntegerData.h"
#include "RaceElementData.h"
#include "NewRecruitRequest.h"
#include "NewEntityPlaceholder.h"
#include "RaceRule.h"
#include "UnitEntity.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
#include "EntitiesCollection.h"
#include "BinaryPattern.h"
#include "QuartenaryPattern.h"
extern EntitiesCollection <UnitEntity>      units;
extern Reporter * recruiterReporter;
extern Reporter * newRecruitReporter;

NewRecruitRequest::NewRecruitRequest(UnitEntity * unit, Order * orderId,
                              int amount, RaceRule * race, int price,
                              NewEntityPlaceholder * targetUnit):
                  MarketRequest( unit, orderId, amount, 0, price,BUY)
{
  race_ = race;
  targetUnit_ = targetUnit;
}
NewRecruitRequest::~NewRecruitRequest(){
}
string NewRecruitRequest::print()
{
  char buffer1[12];
  char buffer2[12];
  longtostr(amount_, buffer1);
  longtostr(price_, buffer2);

  return  unit_->printName() + " requests to recruit into new unit" + buffer1 +
          " of " + race_->printName() + " for " + buffer2  + " coins\n";
}

bool NewRecruitRequest::isValid()
{
  if(unit_ == 0)
    return false;

  if(unit_->getLocation()== 0)  // Dead
    return false;

  if(targetUnit_ == 0)
    return false;

    if (unit_->hasMoney() >= price_ * amount_)
      return true;
    else
      return false;

}

AbstractData * NewRecruitRequest::getType() const
{
  return race_;
}



void NewRecruitRequest::answerMarketRequest(int price, int amount)
{

    if (unit_->isTraced())
        cout <<"== TRACING "  << unit_->printName()<< " recruits into new unit " << amount << " of " <<  race_->getName() << " for " << price << " coins.\n";
    assert(unit_->takeFromInventoryExactly(cash, price * amount));
     // create new unit and assign it to placeholder  targetUnit_
     UnitEntity * newUnit   = new UnitEntity(unit_);
      if(units.addNew(newUnit) != OK)
      {
        cout << "Failed to add new unit \n";
        return;
      }
      targetUnit_->setRealEntity(newUnit);
       unit_->getFaction()->addUnit(newUnit);
       unit_->getLocation()->addUnit(newUnit);
       newUnit->setRace(race_,amount);
       newUnit->recalculateStats();
	     if (unit_->isTraced())
        cout  <<"== TRACING " << "New unit created: "<<newUnit->printName() <<" \n";
       
    // report new unit created
    newUnit ->addReport(new BinaryPattern (newRecruitReporter,
                        newUnit, new RaceElementData (race_ , amount)));
                            
    unit_->addReport(new QuartenaryPattern(recruiterReporter, unit_,
                    new RaceElementData(race_ , amount),
                    new IntegerData(price),new IntegerData(price * amount)));
    // finish order processing  updateOrderResults
     orderId_->completeProcessing(unit_,amount);
    return;

}
