//
// GuardingRequest.h
//
// Description: Keeps information about requesting unit
//     and actions that should be performed after request resolution
//
// Author: Alex Dribin <alex@netvision.net.il>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GUARDING_REQUEST_H
#define GUARDING_REQUEST_H
#include <string>
#include <vector>
#include "OverlordTypes.h"
using namespace std;
/**
Keeps information about requesting unit and actions that should be performed after request resolution*/
class TokenEntity;
class OrderLine;


class GuardingRequest 
{
public:
GuardingRequest(TokenEntity * guard, OrderLine * orderId,
	void (*funPtr)(const TokenEntity * guard, ORDER_STATUS result))
     {guard_ = guard; orderId_ = orderId; postGuardProcessing_ = funPtr;}
  virtual ~GuardingRequest(){}
  virtual void answerRequest(const ORDER_STATUS result);
  virtual  string print();
  virtual  bool isEqualTo (GuardingRequest * request);
  virtual inline void free(){delete this;}
  virtual   bool isValid();
  virtual inline TokenEntity *  getGuard() const {return guard_;}
  virtual inline OrderLine *  getOrderId() const {return orderId_;}
protected:
  void (*postGuardProcessing_)(const TokenEntity * guard, ORDER_STATUS result);
  TokenEntity * guard_;
  OrderLine * orderId_;

private:

};

typedef  vector  < GuardingRequest*>::iterator GuardingRequestIterator;
#endif
