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
#include "Rational.h"
#include "SystemTypes.h"
using namespace std;


/**
  *@author Alex Dribin
  */
class UnitEntity;
class AbstractData;
class Order;

class BasicCompetitiveRequest {
public: 
	BasicCompetitiveRequest(UnitEntity * unit, Order * orderId);
	virtual ~BasicCompetitiveRequest();
  virtual string print();
  inline UnitEntity * getRequestingUnit() const {return unit_;}
//   virtual bool operator ==  (BasicCompetitiveRequest request);
  virtual bool isEqualTo (BasicCompetitiveRequest * request);
  inline virtual Rational  getValue() const {cout << "---- basic get"<<endl;return 0;}
         virtual void setValue(const Rational& value)  {}
  virtual inline AbstractData * getType() const {return 0;}
  virtual inline void answerRequest(Rational& answer) {}
  virtual void free();
  virtual inline Rational getTotalAvailableValue() const {return 0;}
  virtual  bool isValid();
//  virtual inline bool isGreaterThan(BasicCompetitiveRequest& request) {return true;}
protected:
  UnitEntity * unit_;
  Order * orderId_;

};

#endif
