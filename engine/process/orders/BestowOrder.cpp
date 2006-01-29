/***************************************************************************
                             BestowOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "BestowOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "TitleRule.h"
#include "TitleElement.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
#include "RulesCollection.h"
extern EntitiesCollection <UnitEntity>      units;
extern RulesCollection <TitleRule>      titles;
extern EntitiesCollection <LocationEntity>      locations;
const unsigned BestowOrder::BESTOW_CONDITION_REPORT_FLAG = 0x01;
const unsigned BestowOrder::CANNOT_BESTOW_SELF_REPORT_FLAG = 0x02;

extern ReportPattern * noBestowTitleReporter;
extern ReportPattern * noBestowTitleConditionReporter;
extern ReportPattern * publicBestowReporter;
extern ReportPattern * privateBestowReporter;
extern ReportPattern * noOwnBestowTitleReporter;
extern ReportPattern * receiveTitleReporter;

BestowOrder * instantiateBestowOrder = new BestowOrder();

BestowOrder::BestowOrder(){
  keyword_ = "Bestow";
  registerOrder_();
  description = string("BESTOW title-tag location-id unit-id \n") +
  "Immediate, leader only, one-shot.  Executes as soon as the designated unit is\n" +
  "present at the same location, and the unit holds a title.  The title deeds are\n" +
  "transferred to the other unit.\n";

  orderType_   = IMMEDIATE_ORDER;
}

STATUS BestowOrder::loadParameters(Parser * parser,
                            ParameterList &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, titles, "title-tag", parameters))
            return IO_ERROR;
    if(!parseGameDataParameter(entity,  parser, locations, "location id", parameters))
            return IO_ERROR;
    if(!parseGameDataParameter(entity,  parser, units, "unit id", parameters))
            return IO_ERROR;
  return OK;

}



ORDER_STATUS BestowOrder::process (Entity * entity, ParameterList &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
  if(parameters.size() <3)
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
      unit->addReport(new BinaryMessage(noBestowTitleReporter,titleType ,unit->getLocation()));
	    return INVALID;
    }

  // Check that this is your title!
    if(unit != title->getTitleHolder())
    {
      if(!unit->getCurrentOrder()->getReportingFlag(CANNOT_BESTOW_SELF_REPORT_FLAG))
	    {
      unit->addReport(new UnaryMessage(noOwnBestowTitleReporter,title));
        unit->getCurrentOrder()->setReportingFlag(CANNOT_BESTOW_SELF_REPORT_FLAG);
     }
	    return FAILURE;
    }
    unit->getCurrentOrder()->clearReportingFlag(CANNOT_BESTOW_SELF_REPORT_FLAG);

	UnitEntity * recipient   =  DOWNCAST_ENTITY<UnitEntity>(parameters[2]);
  if( recipient == 0)
    {
	    return INVALID;
    }

  if ( !title->getClaimingCondition()->isSatisfied(recipient))
    {
      if(!unit->getCurrentOrder()->getReportingFlag(BESTOW_CONDITION_REPORT_FLAG))
	    {
      unit->addReport(new TertiaryMessage(noBestowTitleConditionReporter,
			  recipient, title->getClaimingCondition(),titleType));
        unit->getCurrentOrder()->setReportingFlag(BESTOW_CONDITION_REPORT_FLAG);
     }
	    return FAILURE;
    }
    unit->getCurrentOrder()->clearReportingFlag(BESTOW_CONDITION_REPORT_FLAG);

    unit->removeTitle(title);
    title->setTitleHolder(recipient);
    recipient->addTitle(title);
  	title->activateClaimingEffects();
    unit->addReport(new BinaryMessage(privateBestowReporter,title, recipient));
    recipient->addReport(new BinaryMessage(receiveTitleReporter,title, unit));

    unit->getLocation()->addReport(new TertiaryMessage(publicBestowReporter, entity,
        					title,recipient));

	return SUCCESS;
}

