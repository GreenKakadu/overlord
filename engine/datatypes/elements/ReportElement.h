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
	~ReportElement(){}
//  /** No descriptions */
  void print(ostream &out) const
			{	out << "Day " << parameter2_ << " ";	rule_->print(out);}
  /** No descriptions */
  inline ReportPattern * getRecord() const {return rule_;}
	inline Entity *        getDestination() const {return parameter1_;}

};


/** No descriptions */


//#ifndef REPORTELEMENTEXPLICIT_H
//#include "ReportElement.cpp"
//#endif

#endif
