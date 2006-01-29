//
// C++ Implementation: GuardingRequest.cpp
//
// Description: Keeps information about requesting unit
//     and actions that should be performed after request resolution
//
//
// Author: Alex Dribin <alex@localhost.localdomain>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "GuardingRequest.h"
#include "TokenEntity.h"
#include "LocationEntity.h"
#include "OrderLine.h"
#include "UnaryMessage.h"
ReportPattern  *	guardingReporter = new
            ReportPattern("guardingReporter");


void GuardingRequest::answerRequest(const  ORDER_STATUS result)
{
	 if(result == SUCCESS)
	 {
			guard_->setGuarding(true);
			guard_->addReport(new UnaryMessage
						 (guardingReporter,guard_->getLocation()),orderId_,0);
//	 	  cout <<"##         ##"<< *guard_ << " guards. " <<endl;
	 }
//	 else
//	 	 	cout <<"##         ##"<< *guard_ << " can't guard. " <<endl;

   if(orderId_)
    orderId_->completeProcessing(guard_,result);
   if(postGuardProcessing_)
    (*postGuardProcessing_)(guard_,result);
}



string GuardingRequest::print()
{
  return  guard_->print() + (" tries to guard(patrol)  ") ;
}



bool GuardingRequest::isEqualTo (GuardingRequest * request)
{
/*  return (getDefender()->getLocation() ==  request->getDefender()->getLocation());*/
return true;
}


bool GuardingRequest::isValid()
{
  if(guard_== 0)
    return false;

  if(!guard_->isAlive())  // Dead
    return false;

  return true;
}



