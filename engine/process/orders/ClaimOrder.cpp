/***************************************************************************
                          ClaimOrder.cpp
                             -------------------
    begin                : Mon Nov 3 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
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
#include "SimpleMessage.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "QuartenaryMessage.h"

ClaimOrder * instantiateClaimOrder = new ClaimOrder();
extern RulesCollection <TitleRule>      titles;
extern ReportPattern * noTitleReporter;
extern ReportPattern * noTitleConditionReporter;
extern ReportPattern * cantPayForTitleReporter;
extern ReportPattern * noFreeTitleReporter;
extern ReportPattern * contestTitleReporter;
extern ReportPattern * claimTitleReporter;
extern ReportPattern * cantContestFromAllyReporter;

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

    fullDayOrder_= true;
    orderType_   = IMMEDIATE_ORDER;
}



STATUS ClaimOrder::loadParameters(Parser * parser,
                            ParameterList &parameters, Entity * entity )
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



ORDER_STATUS ClaimOrder::process (Entity * entity, ParameterList &parameters)
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
      unit->addReport(new BinaryMessage(noTitleReporter,titleType ,unit->getLocation()));
	    return INVALID;
    }

  if ( !title->getClaimingCondition()->isSatisfied(unit))
    {
      unit->addReport(new TertiaryMessage(noTitleConditionReporter, unit,
                      title->getClaimingCondition(),titleType));
	    return FAILURE;
    }

  if(!unit->mayPay(title->getTitle()->getCost()))
    {
      unit->addReport(new UnaryMessage(cantPayForTitleReporter,titleType));
	    return FAILURE;
    }

  UnitEntity * titleHolder = title->getTitleHolder();
  if(titleHolder != 0)
    {
     if(claimFreeTitleOnly)
      {
        unit->addReport(new TertiaryMessage(noFreeTitleReporter,titleType, title->getTitleLocation(), titleHolder));
	      return FAILURE;
      }
      if( *(unit->getFaction()->getStance(titleHolder)) >= *alliedStance )
      {
        unit->addReport(new SimpleMessage(cantContestFromAllyReporter));
        return INVALID;
      }


      unit->addReport(new QuartenaryMessage(contestTitleReporter,unit,titleType, title->getTitleLocation(), titleHolder));
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
      unit->addReport(new TertiaryMessage(claimTitleReporter, unit,titleType,title->getTitleLocation()));
	return SUCCESS;
}
