/***************************************************************************
                          ReportElement.h  -
Element containing pointer to report and to entity, that delivered this report
Used for report creation.
                             -------------------
    begin                : Wed Dec 11 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef REPORT_ELEMENT_H
#define REPORT_ELEMENT_H
#include <stdlib.h>
#include <string>
#include <iostream>
#include "ReportMessage.h"
#include "Entity.h"
#include "Element.h"

using namespace std;
typedef   Element3<ReportMessage , Entity* , int> BasicReportElement;

class ReportElement : public  BasicReportElement  {
public:
	ReportElement(ReportMessage * pattern, Entity* destination, int time = currentDay)  : BasicReportElement (pattern,destination,time)
			{}
	~ReportElement(){}
  void printReport(ReportPrinter &out) const
			{
        out << "Day " << parameter2_ << " ";
        out.incr_indent();	out <<(*rule_); out.decr_indent();
        }
  inline ReportMessage * getRecord() const {return rule_;}
	inline Entity *        getDestination() const {return parameter1_;}
private:
    void save(ostream & out){} // disabled

};
typedef vector< ReportElement *>::iterator  ReportIterator;



//#ifndef REPORTELEMENTEXPLICIT_H
//#include "ReportElement.cpp"
//#endif

#endif
