/***************************************************************************
                          ReportRecord.cpp  -  description
                             -------------------
    begin                : Sun Dec 8 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "ReportRecord.h"
extern int currentDay;

ReportRecord::ReportRecord(ReportPattern * report, Order *  order, BasicCondition * condition)
{
 observationCriteria_ = condition;
 reportMessage = report;
 orderId =  order;
//	cout << ".....Report created...... |"<< reportMessage->print(cout);
}

ReportRecord::~ReportRecord(){}
/** No descriptions */
//void ReportRecord::setReport( char *  record){
// report_  = (record);///*const_cast<char *>*/ (record.c_str()); //
//}



/** No descriptions */
bool ReportRecord::observableBy(UnitEntity * unit) const
{
  if(observationCriteria_)
    return(observationCriteria_->isSatisfied(unit));
   else
   return true; 
}



/** No descriptions */
//void ReportRecord::print(ostream &out){
//out <<  "Day " <<day_ << " : " <<report_ << endl;
//}
