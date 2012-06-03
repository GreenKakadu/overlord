/***************************************************************************
                             YieldOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "YieldOrder.h"
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

extern ReportPattern * noOwnYieldTitleReporter;
extern ReportPattern * noYieldTitleReporter;
extern ReportPattern * publicYieldTitleReporter;
extern ReportPattern * privateYieldTitleReporter;

YieldOrder * instantiateYieldOrder = new YieldOrder();

YieldOrder::YieldOrder(){
  keyword_ = "Yield";
  registerOrder_();
  description = string("YIELD title-tag location-id \n") +
  "Immediate, leader only, one-shot.  Executes if the unit holds a title.\n" +
  "The title becomes unclaimed.\n";
  orderType_   = IMMEDIATE_ORDER;
}

STATUS YieldOrder::loadParameters(Parser * parser,
                            ParameterList &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, gameFacade->titles, "title-tag", parameters))
            return IO_ERROR;
    if(!parseGameDataParameter(entity,  parser, gameFacade->locations, "location id", parameters))
            return IO_ERROR;
  return OK;

}



ORDER_STATUS YieldOrder::process (Entity * entity, ParameterList &parameters)
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
      unit->addReport(new BinaryMessage(noYieldTitleReporter,titleType ,unit->getLocation()));
	    return INVALID;
    }
    if(unit != title->getTitleHolder())
    {
      unit->addReport(new UnaryMessage(noOwnYieldTitleReporter,title));
	    return FAILURE;
    }

      unit->addReport(new UnaryMessage(privateYieldTitleReporter,title));
      unit->getLocation()->addReport(new BinaryMessage(publicYieldTitleReporter,unit,title));


    unit->removeTitle(title);
	return SUCCESS;
}

