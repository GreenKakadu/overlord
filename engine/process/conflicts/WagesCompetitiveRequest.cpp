/***************************************************************************
                          WagesCompetitiveRequest.cpp 
                             -------------------
    begin                : Fri Apr 25 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include <typeinfo> 
#include "WagesCompetitiveRequest.h"
#include "AbstractData.h"
#include "IntegerData.h"
#include "ItemRule.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"
extern RulesCollection    <ItemRule>    items;
Reporter * WagesCompetitiveRequest::workPublicReporter = new Reporter(""," is working.");
Reporter * WagesCompetitiveRequest::workPrivateReporter = new Reporter("Working at "," earning "," coins.");


WagesCompetitiveRequest::WagesCompetitiveRequest(UnitEntity * unit, OrderLine * orderId, RationalNumber& amount) :BasicCompetitiveRequest(unit,0)
{
  resourceAmount_ =  amount;
}



WagesCompetitiveRequest::~WagesCompetitiveRequest()
{
//  if(resource_) delete resource_;
}


bool WagesCompetitiveRequest::isEqualTo (BasicCompetitiveRequest * request)
{
  if(typeid(*request) != typeid(*this))
    return false;
  return true;
}



RationalNumber WagesCompetitiveRequest::getValue() const
{
  return resourceAmount_;
}

void WagesCompetitiveRequest::setValue(const RationalNumber& value)
{
  resourceAmount_ = value;
}


AbstractData * WagesCompetitiveRequest::getType() const
{
  return cash;
}



RationalNumber WagesCompetitiveRequest::getTotalAvailableValue() const
{
    return  unit_->getLocation()->getWages() * 50 ; // Temp
}



void WagesCompetitiveRequest::answerRequest(RationalNumber& answer)
{
//  int answer =  scale_down(scaledAnswer);
  unit_->addToInventory( cash,answer);
  unit_->addReport(new BinaryPattern(workPrivateReporter, unit_->getLocation(), new IntegerData(answer.getValue())));
  unit_->getLocation()->addReport(new UnaryPattern(workPublicReporter, unit_));
//  cout << unit_->print() << " is working at "<< unit_->getLocation()->print()<<"earning "<< answer<< " coins.\n";

}
