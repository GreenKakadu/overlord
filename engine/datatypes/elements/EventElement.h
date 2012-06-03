/***************************************************************************
                          EventElement.h  -
Element containing pointer to report and to entity, that delivered this report
Used for report creation.
                             -------------------
    begin                : April 5, 2011, 5:35 PM
    copyright            : (C) 2011 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef EVENTELEMENT_H
#define	EVENTELEMENT_H

#include <stdlib.h>
#include <string>
#include <iostream>
#include "Event.h"
#include "Element.h"
#include "OrderLine.h"

using namespace std;
typedef   Element2<Event  , OrderLine *> BasicEventElement;

class EventElement  : public  BasicEventElement  {
public:
	EventElement(Event * event, OrderLine * orderId)  : BasicEventElement (event,orderId){}
	virtual ~EventElement(){}

//  void printEvent(ReportPrinter &out) const
//			{
//        out << "Day " << parameter2_ << " ";
//        out.incr_indent();	out <<(*rule_); out.decr_indent();
//        }

        inline Event * getEvent() const {return rule_;}
	inline OrderLine *    getOrder() const {return parameter1_;}
        vector <AbstractData *>  aPrint()
        {
          return rule_->aPrint();
        }

private:
    void save(ostream & ){} // disabled

};
typedef vector< EventElement>::iterator  EventElementIterator;
#endif	/* EVENTELEMENT_H */

