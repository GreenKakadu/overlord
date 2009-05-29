/***************************************************************************
                          ReportRecord.h
                             -------------------
    begin                : Sun Dec 8 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef REPORT_RECORD_H
#define REPORT_RECORD_H
#include <string>
#include "BasicCondition.h"
using namespace std;

/**Contains information to be used for report creation
  *@author Alex Dribin
  */
class ReportMessage;
class OrderLine;
class BasicCondition;
class TokenEntity;

class ReportRecord {
public:
	ReportRecord(ReportMessage * report, OrderLine *  orderId = 0, BasicCondition * condition = 0);
	~ReportRecord();
  bool observableBy(TokenEntity * unit) /*const*/;
	ReportMessage * reportMessage;
  OrderLine *  orderId;

 protected:

 BasicCondition  * observationCriteria_;

};

#endif
