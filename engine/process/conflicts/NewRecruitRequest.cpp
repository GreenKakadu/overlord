/***************************************************************************
                          NewRecruitRequest.cpp
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
#include "IntegerData.h"
#include "RaceElement.h"
#include "NewRecruitRequest.h"
#include "NewEntityPlaceholder.h"
#include "RaceRule.h"
#include "UnitEntity.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
#include "EntitiesCollection.h"
#include "SimpleMessage.h"
#include "BinaryMessage.h"
#include "QuartenaryMessage.h"
extern EntitiesCollection <UnitEntity>      units;
extern ReportPattern * recruiterReporter;
extern ReportPattern * newRecruitReporter;
extern ReportPattern *	 recruitingNotPermittedReporter;
extern ReportPattern * cannotPayRecruitReporter ;

NewRecruitRequest::NewRecruitRequest(UnitEntity * unit, OrderLine * orderId,
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
  return  unit_->print() + " requests to recruit into new unit" + longtostr(amount_) +
          " of " + race_->print() + " for " + longtostr(price_)  + " coins\n";
}



bool NewRecruitRequest::isValid()
{
  if(unit_ == 0)
    return false;

  if(targetUnit_ == 0)
    return false;

  LocationEntity * location = unit_->getLocation();
  if(location== 0)  // Dead
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

AbstractData * NewRecruitRequest::getType() const
{
  return race_;
}



void NewRecruitRequest::answerMarketRequest(int price, int amount)
{

 /*   if (unit_->isTraced())
        cout <<"== TRACING "  << unit_->print()<< " recruits into new unit " << amount << " of " <<  race_->getName() << " for " << price << " coins.\n";
 */    
  if(unit_->hasMoney() < price * amount)
  {
    unit_ ->addReport(new SimpleMessage( cannotPayRecruitReporter));
    return;
  }
      int taken = unit_->takeFromInventory(cash, price * amount); //pay
      assert(taken == price * amount);

		 // create new unit and assign it to placeholder  targetUnit_
			UnitEntity * newUnit = 0;
      TokenEntity * temp = targetUnit_->getNewEntity();
		 if(temp)
		 {
       newUnit = dynamic_cast<UnitEntity *>(temp);
		 }

 		if(newUnit == 0) // Something wrong with placeholder
    		newUnit   = new UnitEntity(unit_);

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
        cout  <<"== TRACING " << "New unit created: "<<newUnit->print() <<" \n";

    // report new unit created
//QQQ
    newUnit ->addReport(new BinaryMessage (newRecruitReporter,
                        newUnit, new RaceElement (race_ , amount)));

    unit_->addReport(new QuartenaryMessage(recruiterReporter, unit_,
                    new RaceElement(race_ , amount),
                    new IntegerData(price),new IntegerData(price * amount)));
    // finish order processing  updateOrderResults
     orderId_->completeProcessing(unit_,amount);
    return;

}

// UnitEntity * createNewUnit(UnitEntity * parentUnit, , int amount, RaceRule * race, NewEntityPlaceholder * target)
// {
// 		 // create new unit and assign it to placeholder  targetUnit_
// 			UnitEntity * newUnit = 0;
//       TokenEntity * temp = targetUnit_->getNewEntity();
// 		 if(temp)
// 		 {
//        newUnit = dynamic_cast<UnitEntity *>(temp);
// 		 }
// 
//  		if(newUnit == 0) // Something wrong with placeholder
//     		newUnit   = new UnitEntity(unit_);
// 
//       if(units.addNew(newUnit) != OK)
//       {
//         cout << "Failed to add new unit \n";
//         return 0;
//       }
//       target->setRealEntity(newUnit);
//        unit_->getFaction()->addUnit(newUnit);
//        unit_->getLocation()->addUnit(newUnit);
//        newUnit->setRace(race,amount);
//        newUnit->recalculateStats();
//     // finish order processing  updateOrderResults
//  }
