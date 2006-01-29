//
// PillageRequest.h
//
// Description: Keeps information about requesting unit
//     and actions that should be performed after request resolution
//
// Author: Alex Dribin <alex@netvision.net.il>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PILLAGE_REQUEST_H
#define PILLAGE_REQUEST_H
#include <string>
#include <vector>
#include "OverlordTypes.h"
using namespace std;
class UnitEntity;
class OrderLine;

class PillageRequest
{
public:
PillageRequest(UnitEntity * pillager, OrderLine * orderId)
     {pillager_ = pillager; orderId_ = orderId; }
  virtual ~PillageRequest(){}
  virtual void answerRequest(const int result);
  virtual  string print();
  virtual  bool isEqualTo (PillageRequest * request);
  virtual inline void free(){delete this;}
  virtual   bool isValid();
  virtual inline UnitEntity *  getPillager() const {return pillager_;}
  virtual inline OrderLine *  getOrderId() const {return orderId_;}
protected:
  UnitEntity * pillager_;
  OrderLine * orderId_;

private:

};


typedef  vector  < PillageRequest *>::iterator PillageRequestIterator;
#endif
