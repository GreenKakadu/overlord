/***************************************************************************
                          BasicOrderSynchronizationRequest.h
                             -------------------
    begin                : Sun Dec 28 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef BASIC_ORDER_SYNCHRONIZATION_REQUEST_H
#define BASIC_ORDER_SYNCHRONIZATION_REQUEST_H
class OrderPrototype;

/**Basic interface for order synchronization.
  *@author Alex Dribin
  */
enum  order_sync_request_status {
   ORDER_SYNC_START                          = 0,
   ORDER_SYNC_WAIT                           = 1,
   ORDER_SYNC_DONE                           = 2,
   ORDER_SYNC_REQUEST_STATUS_END              = 10
                };

typedef order_sync_request_status ORDER_SYNC_REQUEST_STATUS;
class BasicOrderSynchronizationRequest {
public: 
	BasicOrderSynchronizationRequest(OrderPrototype * orderType){orderType_ = orderType;}
	virtual ~BasicOrderSynchronizationRequest(){}
  inline const OrderPrototype * getType() const {return orderType_;}
  inline bool syncDone() const {return status_ == ORDER_SYNC_DONE;}
  inline void setSyncDone() {status_ = ORDER_SYNC_DONE;}
  inline ORDER_SYNC_REQUEST_STATUS getStatus() const{return status_;}
  inline void setStatus(ORDER_SYNC_REQUEST_STATUS status) { status_ = status;}
         bool isEqual(BasicOrderSynchronizationRequest  * request);
  virtual bool isMatch(BasicOrderSynchronizationRequest  * request){return false;}
protected:
 const OrderPrototype * orderType_;
 ORDER_SYNC_REQUEST_STATUS status_;

};

#endif
