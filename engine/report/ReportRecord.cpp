/***************************************************************************
                          ReportRecord.cpp  -  description
                             -------------------
    begin                : Sun Dec 8 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "ReportRecord.h"
#include "ReportMessage.h"
extern int currentDay;

ReportRecord::ReportRecord(ReportMessage * report, OrderLine *  order, BasicCondition * condition)
{
 observationCriteria_ = condition;
 reportMessage = report;
 orderId =  order;
}



ReportRecord::~ReportRecord()
{
//  cout<<"********* ReportRecord deleted: "; reportMessage->printReport(cout);
}



/** No descriptions */
bool ReportRecord::observableBy(TokenEntity * unit) //const
{
  if(observationCriteria_)
    return(observationCriteria_->isSatisfied(unit));
   else
   return true;
}

