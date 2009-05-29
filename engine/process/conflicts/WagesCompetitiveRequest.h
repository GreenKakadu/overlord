/***************************************************************************
                          WagesCompetitiveRequest.h
                             -------------------
    begin                : Fri Apr 25 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
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
class AbstractData;
class ReportPattern;
class ItemRule;

/**
  *@author Alex Dribin
  */

class WagesCompetitiveRequest : public BasicCompetitiveRequest  {
public:
//	WagesCompetitiveRequest(UnitEntity * unit, int requestedSalary);
	WagesCompetitiveRequest(UnitEntity * unit, OrderLine * orderId, RationalNumber& amount);
	~WagesCompetitiveRequest();
//   bool operator ==  (WagesCompetitiveRequest request);
//   inline int getSalaryRequest() const {return requestedSalary_;}
   RationalNumber getValue() const;
   void setValue(const RationalNumber& value);
   AbstractData * getType() const;
  RationalNumber getTotalAvailableValue() const;
  void answerRequest(RationalNumber& answer);
  static ReportPattern * workPublicReporter;
  static ReportPattern * workPrivateReporter;
protected:
  RationalNumber resourceAmount_;
//  int requestedSalary_;
};

#endif
