/***************************************************************************
                             CancelOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "CancelOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "UnaryMessage.h"
#include "LocationEntity.h"
#include "TitleRule.h"
#include "TitleElement.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
extern EntitiesCollection <UnitEntity>      units;
extern RulesCollection <TitleRule>      titles;
extern EntitiesCollection <LocationEntity>      locations;
extern ReportPattern * publicCancelTitleReporter;
extern ReportPattern * privateCancelTitleReporter;
extern ReportPattern * cannotCancelReporter;
extern ReportPattern * noCancelOwnReporter;
extern ReportPattern * noCancelTitleReporter;
CancelOrder * instantiateCancelOrder = new CancelOrder();
const unsigned CancelOrder::CANNOT_CANCEL_OWN_REPORT_FLAG = 0x01;
const unsigned CancelOrder::CANNOT_CANCEL_REPORT_FLAG = 0x02;

CancelOrder::CancelOrder(){
  keyword_ = "Cancel";
  registerOrder_();
  description = string("CANCEL title-tag location-id \n") +
  "Immediate, title holder only.  Executes as soon as the unit is at the\n" +
  "required location.  Attempts to revoke the title currently held at the\n" +
  "location, if the unit issuing the CANCEL has the title necessary to revoke\n" +
  "the title.\n";
  orderType_   = IMMEDIATE_ORDER;
}

STATUS CancelOrder::loadParameters(Parser * parser,
                            ParameterList &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, titles, "title-tag", parameters))
            return IO_ERROR;
    if(!parseGameDataParameter(entity,  parser, locations, "location id", parameters))
            return IO_ERROR;
  return OK;

}



ORDER_STATUS CancelOrder::process (Entity * entity, ParameterList &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
  if(parameters.size() <2)
  	return INVALID;

	TitleRule * titleType   =  dynamic_cast<TitleRule *>(parameters[0]);
  if( titleType == 0)
    {
	    return INVALID;
    }
	LocationEntity * location   =  dynamic_cast<LocationEntity *>(parameters[1]);
  if( location == 0)
    {
	    return INVALID;
    }
  TitleElement * title = location->findTitle(titleType);
  if(title == 0)
    {
      unit->addReport(new BinaryMessage(noCancelTitleReporter,titleType ,unit->getLocation()));
	    return INVALID;
    }

    if(unit == title->getTitleHolder())
    {
      if(!unit->getCurrentOrder()->getReportingFlag(CANNOT_CANCEL_OWN_REPORT_FLAG ))
	    {
        unit->addReport(new UnaryMessage(noCancelOwnReporter,title));
        unit->getCurrentOrder()->setReportingFlag(CANNOT_CANCEL_OWN_REPORT_FLAG);
     }
	      return FAILURE;
    }
    unit->getCurrentOrder()->clearReportingFlag(CANNOT_CANCEL_OWN_REPORT_FLAG);

    if(!unit->mayCancelTitle(title))
    {
      if(!unit->getCurrentOrder()->getReportingFlag(CANNOT_CANCEL_REPORT_FLAG ))
	    {
        unit->addReport(new UnaryMessage(cannotCancelReporter,title));
        unit->getCurrentOrder()->setReportingFlag(CANNOT_CANCEL_REPORT_FLAG );
     }
	    return FAILURE;
    }
    else
    unit->getCurrentOrder()->clearReportingFlag(CANNOT_CANCEL_REPORT_FLAG);


    unit->removeTitle(title);
    unit->addReport(new UnaryMessage(privateCancelTitleReporter,title));
    unit->getLocation()->addReport(new UnaryMessage(publicCancelTitleReporter,title));

	return SUCCESS;

}

