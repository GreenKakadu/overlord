/***************************************************************************
                          ReportRecord.cpp  -  description
                             -------------------
    begin                : Sun Dec 8 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "ReportRecord.h"
#include "ReportPattern.h"
extern int currentDay;

ReportRecord::ReportRecord(ReportPattern * report, Order *  order, BasicCondition * condition)
{
 observationCriteria_ = condition;
 reportMessage = report;
 orderId =  order;
//	cout << ".....Report created...... |"<< reportMessage->print(cout);
}



/*
 * Cleans report pattern parameters.
 * Cleaning deletes parameter objects if they are non-permanent
 */
ReportRecord::~ReportRecord()
{
//  reportMessage->clean();
}



/** No descriptions */
bool ReportRecord::observableBy(PhysicalEntity * unit) //const
{
  if(observationCriteria_)
    return(observationCriteria_->isSatisfied(unit));
   else
   return true; 
}

