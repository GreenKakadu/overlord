/***************************************************************************
                          WagesCompetitiveRequest.h
                             -------------------
    begin                : Fri Apr 25 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef WAGES_COMPETITIVE_REQUEST_H
#define WAGES_COMPETITIVE_REQUEST_H

#include "BasicCompetitiveRequest.h"
//class ItemElement;
class AbstractData;
class Reporter;
class ItemRule;

/**
  *@author Alex Dribin
  */

class WagesCompetitiveRequest : public BasicCompetitiveRequest  {
public: 
//	WagesCompetitiveRequest(UnitEntity * unit, int requestedSalary);
//	WagesCompetitiveRequest(UnitEntity * unit, ItemElement * resource);
	WagesCompetitiveRequest(UnitEntity * unit, Order * orderId, Rational& amount);
	~WagesCompetitiveRequest();
//   bool operator ==  (WagesCompetitiveRequest request);
    bool isEqualTo (BasicCompetitiveRequest * request);
//   inline ItemElement * getResource() const {return resource_;}
//   inline int getSalaryRequest() const {return requestedSalary_;}
   Rational getValue() const;
   void setValue(const Rational& value);
   AbstractData * getType() const;
  Rational getTotalAvailableValue() const;
  void answerRequest(Rational& answer);
  static Reporter * workPublicReporter;
  static Reporter * workPrivateReporter;
protected:
//  ItemRule * resourceType_;
  Rational resourceAmount_;
//  int requestedSalary_;
};

#endif
