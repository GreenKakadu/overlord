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
using namespace std;


/**
  *@author Alex Dribin
  */
class UnitEntity;
class AbstractData;
class OrderLine;

class BasicCompetitiveRequest {
public: 
	BasicCompetitiveRequest(UnitEntity * unit, OrderLine * orderId);
	virtual ~BasicCompetitiveRequest();
  virtual string print();
  inline UnitEntity * getRequestingUnit() const {return unit_;}
//   virtual bool operator ==  (BasicCompetitiveRequest request);
  virtual bool isEqualTo (BasicCompetitiveRequest * request);
  inline virtual RationalNumber  getValue() const {cout << "---- basic get"<<endl;return 0;}
         virtual void setValue(const RationalNumber& value)  {}
  virtual inline AbstractData * getType() const {return 0;}
  virtual inline void answerRequest(RationalNumber& answer) {}
  virtual void free();
  virtual inline RationalNumber getTotalAvailableValue() const {return 0;}
  virtual  bool isValid();
//  virtual inline bool isGreaterThan(BasicCompetitiveRequest& request) {return true;}
protected:
  UnitEntity * unit_;
  OrderLine * orderId_;

};

#endif
