/***************************************************************************
                             YieldOrder.cpp 
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "YieldOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "TitleRule.h"
#include "TitleElement.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "TertiaryPattern.h"
#include "EntitiesCollection.h"
#include "RulesCollection.h"
extern RulesCollection <TitleRule>      titles;
extern EntitiesCollection <LocationEntity>      locations;
extern Reporter * noOwnYieldTitleReporter;
extern Reporter * noYieldTitleReporter;
extern Reporter * publicYieldTitleReporter;
extern Reporter * privateYieldTitleReporter;

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
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, titles, "title-tag", parameters))
            return IO_ERROR;
    if(!parseGameDataParameter(entity,  parser, locations, "location id", parameters))
            return IO_ERROR;
  return OK;

}



ORDER_STATUS YieldOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
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
      unit->addReport(new BinaryPattern(noYieldTitleReporter,titleType ,unit->getLocation()));
	    return INVALID;
    }
    if(unit != title->getTitleHolder())
    {
      unit->addReport(new UnaryPattern(noOwnYieldTitleReporter,title));
	    return FAILURE;
    }
    
      unit->addReport(new UnaryPattern(privateYieldTitleReporter,title));
      unit->getLocation()->addReport(new BinaryPattern(publicYieldTitleReporter,unit,title));
    
    
    unit->removeTitle(title);
	return SUCCESS;
}

