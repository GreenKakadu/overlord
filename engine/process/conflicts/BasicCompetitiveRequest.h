/***************************************************************************
                          BasicCompetitiveRequest.h 
                             -------------------
    begin                : Fri Apr 18 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef BASIC_COMPETITIVE_REQUEST_H
#define BASIC_COMPETITIVE_REQUEST_H
#include <iostream>
#include "RationalNumber.h"
#include "SystemTypes.h"
#include "BasicRequest.h"
using namespace std;


/**
  *@author Alex Dribin
  Abstract class providing generic interface to competitive requests
  */
class UnitEntity;
class AbstractData;
class OrderLine;

class BasicCompetitiveRequest : public BasicRequest{
public: 
	BasicCompetitiveRequest(UnitEntity * unit, OrderLine * orderId):BasicRequest(unit,orderId){}
	virtual ~BasicCompetitiveRequest(){}
  inline virtual RationalNumber  getValue() const {cout << "---- basic get"<<endl;return 0;}
         virtual void setValue(const RationalNumber& value)  {}
  virtual inline void answerRequest(RationalNumber& answer) {}
  virtual inline RationalNumber getTotalAvailableValue() const {return 0;}
protected:

};

#endif
