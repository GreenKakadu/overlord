/***************************************************************************
                          ClaimOrder.cpp 
                             -------------------
    begin                : Mon Nov 3 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "ClaimOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "FactionEntity.h"
#include "TitleRule.h"
#include "ItemRule.h"
#include "TitleElement.h"
#include "RulesCollection.h"
#include "UnitEntity.h"
#include "StanceVariety.h"
#include "LocationEntity.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "TertiaryPattern.h"
#include "QuartenaryPattern.h"

ClaimOrder * instantiateClaimOrder = new ClaimOrder();
extern RulesCollection <TitleRule>      titles;
extern Reporter * noTitleReporter; 
extern Reporter * noTitleConditionReporter;
extern Reporter * cantPayForTitleReporter;  
extern Reporter * noFreeTitleReporter;
extern Reporter * contestTitleReporter;
extern Reporter * claimTitleReporter;
      
ClaimOrder::ClaimOrder(){
  keyword_ = "claim";
  registerOrder_();
  description = string("CLAIM title [FREE]  \n") +
  "Full day, leader only.  Executes as soon as the title is available at the\n" +
  "current location.  Attempts to claim the specified title for the leader.  \n" +
  "The order succeed if the title is free, the challenger has the required \n" +
  "experience in skill, or succeed in challenging the current title holder\n" +
  "under the rules for titles.\n"+
  "\n" +
  "The FREE option prevents the order from executing unless the title is not\n" +
  "already claimed.\n" +
  "\n" +
  "Trying to CLAIM the title of Overlord if the title is held results in failure\n" +
  "and the Imperial faction [npc1] declaring you ENNEMY.\n";
  
    orderType_   = DAY_LONG_ORDER;
}



STATUS ClaimOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, titles, "title tag", parameters))
            return IO_ERROR;
        
  if(parser ->matchKeyword("FREE"))
		{
      parameters.push_back( new StringData ("FREE"));
		}
  return OK;
}



ORDER_STATUS ClaimOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
	TitleRule * titleType   =  dynamic_cast<TitleRule *>(parameters[0]);
  if( titleType == 0)
    {
	    return INVALID;
    }
  bool claimFreeTitleOnly = false;  
  if (parameters.size() > 1)
    {
      string par = parameters[1]->print();
      if(par == string("FREE"))
        {
           claimFreeTitleOnly = true;
           cout<< "Claiming free title only set \n";
          }
    }

  TitleElement * title = unit->getLocation()->findTitle(titleType);
  if(title == 0)
    {
      unit->addReport(new BinaryPattern(noTitleReporter,titleType ,unit->getLocation()));
	    return INVALID;
    }

  if ( !title->getClaimingCondition()->isSatisfied(unit))
    {
      unit->addReport(new TertiaryPattern(noTitleConditionReporter, unit,
                      title->getClaimingCondition(),titleType));
	    return FAILURE;
    }

  if(!unit->mayPay(title->getTitle()->getCost()))
    {
      unit->addReport(new UnaryPattern(cantPayForTitleReporter,titleType));
	    return FAILURE;
    }

  UnitEntity * titleHolder = title->getTitleHolder();
  if(titleHolder != 0)
    {
     if(claimFreeTitleOnly)
      {
        unit->addReport(new TertiaryPattern(noFreeTitleReporter,titleType, title->getTitleLocation(), titleHolder));
	      return FAILURE;
      }
      if( *(unit->getFaction()->getStance(titleHolder)) >= *alliedStance )
      {
        // report can't contest alies use ... instead
	      return INVALID;
      }
      
      
      unit->addReport(new QuartenaryPattern(contestTitleReporter,unit,titleType, title->getTitleLocation(), titleHolder));
      if(!title->contest(unit))  
      {        
        unit->pay(title->getTitle()->getCost()/2);
	      return SUCCESS;
      }
      else
      {
        titleHolder->addToInventory(cash,title->getTitle()->getCost()/2);
        titleHolder->removeTitle(title);
      }
     }
      unit->claimTitle(title);
      unit->addReport(new TertiaryPattern(claimTitleReporter, unit,titleType,title->getTitleLocation()));
	return SUCCESS;
}
