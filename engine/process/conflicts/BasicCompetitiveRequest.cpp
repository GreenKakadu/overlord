/***************************************************************************
                          BasicCompetitiveRequest.cpp 
                             -------------------
    begin                : Fri Apr 18 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include <typeinfo>
#include "BasicCompetitiveRequest.h"
#include "UnitEntity.h"

BasicCompetitiveRequest::BasicCompetitiveRequest(UnitEntity * unit, Order * orderId)
{
  unit_ = unit;
  orderId_ =  orderId;
}



BasicCompetitiveRequest::~BasicCompetitiveRequest(){
}



void BasicCompetitiveRequest::free()
{
  delete this;
}


bool BasicCompetitiveRequest::isEqualTo(BasicCompetitiveRequest * request)
{
  if(typeid(*request) != typeid(*this))
    return false;
  return (this->getType() == request->getType());
}
bool BasicCompetitiveRequest::isValid()
{
  if(unit_== 0)
    return false;
  if(unit_->getLocation()== 0)  // Dead
    return false;
  return true;
}
string BasicCompetitiveRequest::print()
{
  return  string("Request for ") + unit_->printName();
}
