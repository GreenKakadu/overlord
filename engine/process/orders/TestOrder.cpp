/***************************************************************************
                          TestOrder.cpp  -  description
                             -------------------
    begin                : Tue Nov 26 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "IntegerData.h"
#include "Entity.h"
#include "TestOrder.h"
#include "UnaryPattern.h"

//TestOrder instantiateTestOrder;
TestOrder * instantiateTestOrder = new TestOrder();

TestOrder::TestOrder()
{
  keyword_ = "test";
  registerOrder_();
  description = "TEST <result> [0,1,2] \n";
  orderType_   = IMMEDIATE_ORDER;
// All reporters should be created here

	publicReporter_ = new Reporter("Apparently Test done for ","");
	invalidReporter_ = new Reporter("Invalid Test order for ","");
	doneReporter_ = new Reporter("Test done for ","");
	failureReporter_ = new Reporter("Test Failure for ","");
}



TestOrder::~TestOrder()
{
cout << "TEST Order prototype deleted" <<endl;
}



STATUS
TestOrder::loadParameters(Parser * parser, vector <AbstractData *>  &parameters, Entity * entity )
{
  parseIntegerParameter(parser, parameters);
  return OK;
}



ORDER_STATUS
TestOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
//  if ( !processingMode-> mayExecute(orderType_))
//		return FAILURE;
  Order * orderId = entity->getCurrentOrder();

  entity_ =  entity;
  IntegerData * par1       =  dynamic_cast<IntegerData *>(parameters[0]);
 
	switch(par1->getValue())
	{
		case 0:
				{
					entity_->addReport( createPrivateReport(DONE, orderId));
					entity_->getReportDestination()->addReport( createPublicReport(DONE, orderId));
					return SUCCESS;
				}
		case 1:
				{
					entity_->addReport( createPrivateReport(NOT_ACCEPTING, orderId));
					return FAILURE;
				}
		case 2:
			{
				entity_->addReport( createPrivateReport(INVALID_ORDER, orderId));
				return INVALID;
			}
	}
 	return FAILURE;
}
/** Creates report for  Entity for which this order executed */

ReportRecord * TestOrder::createPrivateReport( REPORT_TYPE type, Order * orderId)
{
	switch (type)
	{
		case INVALID_ORDER:
			{											
	UnaryPattern * Message = new UnaryPattern(invalidReporter_, entity_);
	ReportRecord * currentReport = new   ReportRecord(Message, orderId);
	return 	currentReport;					
								
				break;
			}
		case NOT_ACCEPTING:
			{
								

	UnaryPattern * Message = new UnaryPattern(failureReporter_, entity_);	
	ReportRecord * currentReport = new   ReportRecord(Message, orderId);
	return 	currentReport;					
								
 				break;
           }
 		case DONE:
			{
	UnaryPattern * Message = new UnaryPattern(doneReporter_, entity_);	
	ReportRecord * currentReport = new   ReportRecord(Message, orderId);
	return 	currentReport;					
					
				break;
			}
		default :
			{
			}
	}
	return 	0;//currentReport;					
}
/** Creates report for third-party Entities */
ReportRecord * TestOrder::createPublicReport(REPORT_TYPE type, Order * orderId)
{
	 if( type ==  DONE)
			{
	UnaryPattern * Message = new UnaryPattern(publicReporter_, entity_);	
	ReportRecord * currentReport = new   ReportRecord(Message, orderId);
	return 	currentReport;		
				
			}

	return 	0;		
}

 /** Creates report for  Entity for which is a target of order */
ReportRecord * TestOrder::createTargetReport(REPORT_TYPE type, Order * orderId)
{
	switch (type)
	{
		case NOT_ACCEPTING:
			{
	UnaryPattern * Message = new UnaryPattern(doneReporter_, entity_);	
	ReportRecord * currentReport = new   ReportRecord(Message, orderId);
	return 	currentReport;					
							
           }
 		case DONE:
			{	
	UnaryPattern * Message = new UnaryPattern(doneReporter_, entity_);	
	ReportRecord * currentReport = new   ReportRecord(Message, orderId);
	return 	currentReport;					
			}
		case INVALID_ORDER:
		default :
			{
			}
	}
	return 	0;	
}

