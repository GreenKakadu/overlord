/***************************************************************************
                          ReportRecord.h 
                             -------------------
    begin                : Sun Dec 8 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef REPORT_RECORD_H
#define REPORT_RECORD_H
#include <string>
#include "BasicCondition.h"
using namespace std;

/**Contains information to be used for report creation
  *@author Alex Dribin
  */
class ReportPattern;
class OrderLine;
class BasicCondition;
class TokenEntity;

class ReportRecord {
public: 
	ReportRecord(ReportPattern * report, OrderLine *  orderId = 0, BasicCondition * condition = 0);
	~ReportRecord();
  bool observableBy(TokenEntity * unit) /*const*/;
	ReportPattern * reportMessage;
  OrderLine *  orderId;

 protected:

 BasicCondition  * observationCriteria_;

};

#endif
