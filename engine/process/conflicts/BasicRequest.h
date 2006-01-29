/***************************************************************************
                          BasicRequest.h 
                             -------------------
    begin                : Fri Apr 18 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef BASIC_REQUEST_H
#define BASIC_REQUEST_H
#include <iostream>
#include "SystemTypes.h"
using namespace std;


/**
  *@author Alex Dribin
  */
class UnitEntity;
class AbstractData;
class OrderLine;

class BasicRequest {
public: 
	BasicRequest(UnitEntity * unit, OrderLine * orderId){unit_ = unit; orderId_ = orderId;}
	virtual ~BasicRequest(){}
  virtual  string print();
  inline UnitEntity * getRequestingUnit() const {return unit_;}
  virtual  bool isEqualTo (BasicRequest * request);
  virtual inline AbstractData * getType() const {return 0;}
  virtual inline void free(){delete this;}
  virtual   bool isValid();
protected:
  UnitEntity * unit_;
  OrderLine * orderId_;

};

#endif
