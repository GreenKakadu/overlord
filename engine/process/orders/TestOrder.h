/***************************************************************************
                          TestOrder.h  -
								Order for engine testing
                             -------------------
    begin                : Tue Nov 26 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef TEST_ORDER_H
#define TEST_ORDER_H

#include "OrderPrototype.h"
class Reporter;
class Entity;

class TestOrder : public OrderPrototype  {
public: 
	TestOrder();
	~TestOrder();

  STATUS loadParameters(Parser * parser, vector <AbstractData *>  &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, vector <AbstractData *>  &parameters, Order * orderId);
 /** Creates report for  Entity for which this order executed */
  ReportRecord * createPrivateReport(REPORT_TYPE type, Order * orderId);
/** Creates report for second-party Entities */
  ReportRecord * createTargetReport(REPORT_TYPE type, Order * orderId);
  /** Creates report for third-party Entities */
  ReportRecord * createPublicReport(REPORT_TYPE type, Order * orderId);
    protected:
  	Entity * entity_;
	Reporter * publicReporter_;
	Reporter * invalidReporter_ ;
	Reporter * doneReporter_;
	Reporter * failureReporter_;
};
#endif
