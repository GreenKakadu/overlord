/***************************************************************************
                          RecruitOrder.cpp
                             -------------------
    begin                : Thu Jun 5 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "RaceElement.h"
#include "IntegerData.h"
#include "RecruitOrder.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "QuartenaryMessage.h"
#include "EntitiesCollection.h"
#include "RulesCollection.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "LocationEntity.h"
#include "RaceRule.h"
#include "LeaderRaceRule.h"
#include "RecruitRequest.h"
#include "NewRecruitRequest.h"
const UINT RecruitOrder:: INVALID_RECRUIT_REPORT_FLAG = 0x01;
extern ReportPattern * unableRecruitReporter;
extern ReportPattern * recruitInvalidReporter;
extern ReportPattern * recruitForeignUnitReporter;
extern ReportPattern * recruitMaxUnitSizeReporter;
extern ReportPattern * recruitMixedRaceReporter;
extern ReportPattern * unrecruitableRaceReporter;

//RecruitOrder instantiateRecruitOrder;
RecruitOrder * instantiateRecruitOrder = new RecruitOrder();

RecruitOrder::RecruitOrder()
{
  keyword_ = "recruit";
  registerOrder_();
  description = string("RECRUIT unit-id number race price-per-figure \n") +
  "Immediate, leader only.  Attempts to recruit from the local population up to\n" +
  "the number of figures indicated, spending the designated amount per figure,\n" +
  "merging these into the specified unit.  If a new unit ID was specified, the\n" +
  "unit will be created stacked beneath the leader.  The order executes at the\n" +
  "end of the day, as soon as it is possible to recruit at least one of the\n" +
  "designated type of figure.\n" +
   "\n" +
  "The number specified may be 0.  0 is treated as \"as much as possible\", given\n" +
  "the available recruits in the month and the cash in possession of the unit.\n" +
  "Faction funds are not considered.\n" +
   "\n" +
  "The amount specified may be 0.  0 is automatically replaced by the price\n" +
  "indicated in the report.\n" +
   "\n" +
  "Any newly recruited figure will rejoin his unit\n" +
  "(which will be created if necessary) at the end of the day.\n";

  orderType_   = IMMEDIATE_ORDER;
}



STATUS
RecruitOrder::loadParameters(Parser * parser, ParameterList &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;
    if(!parseGameDataParameter(entity, parser, gameFacade->units, "unit id", parameters))
            return IO_ERROR;

    if(!parseIntegerParameter(parser, parameters))
            return IO_ERROR;

    if(!parseGameDataParameter(entity, parser, gameFacade->races, "race tag", parameters))
            return IO_ERROR;

    parseIntegerParameter(parser, parameters);

  return OK;

}



ORDER_STATUS
RecruitOrder::process (Entity * entity, vector < AbstractData*>  &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
  OrderLine * orderId = unit->getCurrentOrder();

  if(!unit->getRace()->mayRectuit())
  {
   // entity not able to recruit (not a leader)
        unit->addReport(new UnaryMessage(unableRecruitReporter,unit->getRace()));
		  return INVALID;
  }
  UnitEntity * newUnit;
  RaceRule * race = dynamic_cast<RaceRule *>(parameters[2]);
     if(race == 0)
   {
     unit->addReport(new UnaryMessage(unrecruitableRaceReporter,parameters[2]));
		  return INVALID;
    }

   IntegerData * par1       =  dynamic_cast<IntegerData *>(parameters[1]);
      assert(par1);
  int number = par1->getValue();

   IntegerData * par3       =  dynamic_cast<IntegerData *>(parameters[3]);
      assert(par3);
  int price = par3->getValue();
  if(price == 0)
	{
		price = unit->getLocation()->getLocalRecruitPrice(race);		
	}
  if(number ==0)
	{	
		if(price != 0)
			number = unit->hasMoney()/price;
	}

   // hostile guards
		//  return INVALID;
   // no race to recruit
		//  return INVALID;
  NewEntityPlaceholder * placeholder = dynamic_cast<NewEntityPlaceholder *>(parameters[0]);
  if (placeholder)  // Is it new entity?
  {
    Entity * realEntity = placeholder->getRealEntity();
    if(realEntity == 0)
    {

   // Submit request
  unit->getLocation()->addMarketRequest(new NewRecruitRequest(unit, unit->getCurrentOrder(), number,race,price,placeholder));

      return IN_PROGRESS;
    }
    else
    {
	    newUnit   =  DOWNCAST_ENTITY<UnitEntity>(parameters[0]);
    }
  }


  else // existing entity
  {
   newUnit  = dynamic_cast<UnitEntity *>(parameters[0]);
   if(!unit->mayInterract(newUnit))
   {
     if(!orderId->getReportingFlag(INVALID_RECRUIT_REPORT_FLAG ))
      {
        unit->addReport(new UnaryMessage(recruitInvalidReporter,parameters[0] ));
        orderId->setReportingFlag(INVALID_RECRUIT_REPORT_FLAG );
      }
      return FAILURE;
    }
   orderId->clearReportingFlag(INVALID_RECRUIT_REPORT_FLAG);


   if(unit->getFaction() != newUnit ->getFaction())
   {
      unit->addReport(new UnaryMessage(recruitForeignUnitReporter,newUnit));
		  return INVALID;
    }


   if(race != newUnit ->getRace())
   {
      unit->addReport(new UnaryMessage(recruitMixedRaceReporter,newUnit));
		  return INVALID;
    }
   // one leader per unit

   if(race->isDescendantFrom(&sampleLeaderRaceRule))
   {
      unit->addReport(new BinaryMessage(recruitMaxUnitSizeReporter,newUnit,
                      new RaceElement(race, 1)));
		  return INVALID;
    }

  }

   // Submit request
  unit->getLocation()->addMarketRequest(new RecruitRequest(unit, unit->getCurrentOrder(), number,race,price,newUnit));

      return IN_PROGRESS;
}



ORDER_STATUS
RecruitOrder::completeOrderProcessing (Entity * entity, ParameterList &parameters, int result)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);

  IntegerData * par1  = dynamic_cast<IntegerData *>(parameters[1]);
  assert(par1);

  int amount = par1->getValue();


  if ( amount > result)
  {
    par1->setValue(amount - result);
    entity->updateOrderResults(FAILURE);
//  cout << "Saving order for "<< unit->print() <<"=[ ";
//  orderId->save(cout);
    return FAILURE;
  }
  entity->updateOrderResults(SUCCESS);
//  cout << "Order completed for "<< unit->print() <<".\n";
  return SUCCESS;
}
