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
#include "ReportPattern.h"
#include "Entity.h"
#include "Element.h"

using namespace std;
typedef   Element3<ReportPattern , Entity* , int> BasicReportElement;

class ReportElement : public  BasicReportElement  {
public:
	ReportElement(ReportPattern * pattern, Entity* destination, int time = currentDay)  : BasicReportElement (pattern,destination,time)
			{}
	~ReportElement(){/*getRecord()->clean();*/}
  void print(ReportPrinter &out) const
			{
        out << "Day " << parameter2_ << " ";
        out.incr_indent();	rule_->print(out); out.decr_indent();
//        cout << "HHHHHHHHHH Day " << endl;//parameter2_<< " ";//rule_->print(cout);
        }
//  void printDebug(ostream &out) const
//			{
//        out << "Day " << parameter2_ << " "; rule_->print(out);
//      }
  inline ReportPattern * getRecord() const {return rule_;}
	inline Entity *        getDestination() const {return parameter1_;}

};
typedef vector< ReportElement *>::iterator  ReportIterator;



//#ifndef REPORTELEMENTEXPLICIT_H
//#include "ReportElement.cpp"
//#endif

#endif
