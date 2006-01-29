//
// C++ Implementation: PillageRequest.cpp
//
// Description: Keeps information about requesting unit
//     and actions that should be performed after request resolution
//
//
// Author: Alex Dribin <alex@netvision.net.il>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "PillageRequest.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "OrderLine.h"
#include "TertiaryMessage.h"
#include "BinaryMessage.h"
#include "IntegerData.h"

ReportPattern  *	pillagingReporter = new
            ReportPattern("pillagingReporter");
ReportPattern  *	pillagingPublicReporter = new
            ReportPattern("pillagingPublicReporter");

void PillageRequest::answerRequest(const  int result)
{
	 if(result)
	 {
  	pillager_->addToInventory( cash, result);
  	pillager_->addReport(new TertiaryMessage(pillagingReporter,pillager_,
			pillager_->getLocation(), new IntegerData(result)));
  pillager_->getLocation()->addReport(new BinaryMessage (pillagingPublicReporter,
		pillager_->getLocation(), pillager_));
	 }

   	if(orderId_)
    	orderId_->completeProcessing(pillager_, result);
}



string PillageRequest::print()
{
  return  pillager_->print() + (" tries to pillage  ") ;
}



bool PillageRequest::isEqualTo (PillageRequest * request)
{
/*  return (getDefender()->getLocation() ==  request->getDefender()->getLocation());*/
return true;
}


bool PillageRequest::isValid()
{
  if(pillager_== 0)
    return false;

  if(!pillager_->isAlive())  // Dead
    return false;

  return true;
}



