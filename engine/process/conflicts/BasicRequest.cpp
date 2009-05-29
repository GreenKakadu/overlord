/***************************************************************************
                          BasicRequest.cpp
                             -------------------
    begin                : Fri Apr 18 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include <typeinfo>
#include "BasicRequest.h"
#include "UnitEntity.h"



bool BasicRequest::isEqualTo(BasicRequest * request)
{
  if(typeid(*request) != typeid(*this))
    return false;
  return (this->getType() == request->getType());
}



bool BasicRequest::isValid()
{
  if(unit_== 0)
    return false;
  if(unit_->getLocation()== 0)  // Dead
    return false;
  return true;
}



string BasicRequest::print()
{
  return  string("Request for ") + unit_->print();
}
