/***************************************************************************
                          ResourceCompetitiveRequest.cpp 
                             -------------------
    begin                : Fri Apr 18 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "ResourceCompetitiveRequest.h"
#include "ItemElementData.h"
#include "ItemElement.h"
#include "AbstractData.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "BinaryPattern.h"
extern Reporter * harvestReporter;

ResourceCompetitiveRequest::ResourceCompetitiveRequest(UnitEntity * unit, Order * orderId, ItemRule * resource,  Rational& amount)  :BasicCompetitiveRequest(unit, orderId)
{
   resourceType_ = resource;
  amount_ = amount;  
}



ResourceCompetitiveRequest::~ResourceCompetitiveRequest()
{
}


Rational ResourceCompetitiveRequest::getValue() const
{
  return amount_;
}

//ItemElement * ResourceCompetitiveRequest::getResource() const
//{
//  return resource_;
//}
//

AbstractData * ResourceCompetitiveRequest::getType() const
{
    return resourceType_;
}



Rational ResourceCompetitiveRequest::getTotalAvailableValue() const
{
    return unit_->getLocation()->getAvailableResource(resourceType_);
}



void ResourceCompetitiveRequest::answerRequest(Rational& answer)
{
  unit_->getLocation()->harvestResource(resourceType_, answer);
  Rational hadBefore = unit_->getItemAmount(resourceType_);
  unit_->addToInventory(resourceType_, answer);
  Rational nowHas = hadBefore + answer;
  int added = nowHas.getValue() - hadBefore.getValue();

  if(added !=0)
  {
//  cout << unit_->printName()<<" harvests " << answer << " " << resourceType_->printName() << " at "<<  unit_->getLocation()->printName() <<endl;
  unit_->addReport(
    new BinaryPattern(harvestReporter, unit_,
    new ItemElementData(resourceType_,added)) );
  unit_->getLocation()->addReport(
    new BinaryPattern(harvestReporter, unit_,
    new ItemElementData(resourceType_,added))
        /*, 0, observation condition*/);
  }
     orderId_->completeProcessing(unit_,added);
}
