/***************************************************************************
                          GiveOrder.cpp  -  description
                             -------------------
    begin                : Tue Jan 7 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "GiveOrder.h"
#include "OrderLine.h"
#include "IntegerData.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "QuartenaryPattern.h"
#include "EntitiesCollection.h"
#include "RulesCollection.h"
#include "ItemRule.h"
#include "StanceVariety.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "FactionEntity.h"

extern EntitiesCollection <UnitEntity>      units;
extern RulesCollection <ItemRule>      items;
extern VarietiesCollection <StanceVariety>    stances;
extern Reporter *	giveRejectedReporter;
extern Reporter *	giveReporter;
extern Reporter *	receiveReporter;
//const UINT GiveOrder::NO_GIFTS_REPORT_FLAG = 0x01;

GiveOrder::GiveOrder()
{
  keyword_ = "give";
  description = string("GIVE unit-id item-tag [number [kept]] \n") +
  "Immediate.  Attempts to hand the required amount of items to the designated unit.\n" +
  "If no number is specified, attempts to give as much as possible.\n" +
  "  The order executes if the designated unit is there and the issuing unit has the items.\n" +
  "  If a number of items to be kept is specified, the unit must have at least the number \n" +
  "of items handed and the number kept in possession.  In that case, zero may be specified \n" +
  "as the number, and the unit will give all but the specified amount, as long as one item \n" +
  "can be given.\n";
  orderType_   = IMMEDIATE_ORDER;
}

STATUS
GiveOrder::loadParameters(Parser * parser, vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity, parser, units, "unit id", parameters))
            return IO_ERROR;
    if(!parseGameDataParameter(entity, parser, items, "item tag", parameters))
            return IO_ERROR;


    if(!parseIntegerParameter(parser, parameters))
            return IO_ERROR;

     parseIntegerParameter(parser, parameters);

  return OK;


}

//============================================================================
//
//      ORDER_STATUS process ()
//
//  Unit gives something to another unit. Both must be at the same location
// and the other must treat the first as friendly.
//
//============================================================================

ORDER_STATUS
GiveOrder::process (Entity * entity, vector < AbstractData*>  &parameters, Order * orderId)
{

  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);

	UnitEntity * recipient   =  DOWNCAST_ENTITY<UnitEntity>(parameters[0]);

  ItemRule * item          =  dynamic_cast<ItemRule *>(parameters[1]);
  assert(item);
  
  int given = 0;
  IntegerData * par2;  
  if(parameters.size() >2)
    {
      par2       =  dynamic_cast<IntegerData *>(parameters[2]);  
      assert(par2);
      given                =  par2->getValue();
    }
     
  int kept = 0;
  if(parameters.size() >3)
    {
      IntegerData * par3       =  dynamic_cast<IntegerData *>(parameters[3]);
      assert(par3);
      kept                 =  par3->getValue();
    }  



 if (!unit->mayInterract(recipient)) // Not In the same place or can't see
	  return FAILURE;

 if(*(recipient->getFaction()->getStance(unit)) < *(stances["friend"]))
      {
        // not accepting. Reports to both sides
      unit->addReport(new   ReportRecord(new UnaryPattern(giveRejectedReporter, recipient)) );
      recipient->addReport(new   ReportRecord(new UnaryPattern(giveRejectedReporter, recipient)) );
		  return INVALID;
      }
/*
 * Validate item amounts
 */
 	
        // get number of items in iventory unitItemPossesion
		 int unitItemPossesion = unit-> hasItem(item);
        if (!unitItemPossesion)
                return FAILURE;
     int reallyGiven = given;

        if (reallyGiven == 0)
                reallyGiven = unitItemPossesion - kept;
        if (reallyGiven + kept > unitItemPossesion)
                reallyGiven = unitItemPossesion - kept;
        if (reallyGiven <= 0)
                return FAILURE;

 /*
 * Carrying
 */
        unit->takeFromInventory(item,reallyGiven);
        recipient->addToInventory(item,reallyGiven);

//        if (item->getTag() == string("mana"))    // Mana shouldn't be an item
//                given = 0;

	
        if (!unit->isSilent() && orderId->isNormalReportEnabled()   )
	  {
//	    QuartenaryPattern * Message  = new QuartenaryPattern(giveReporter, unit, new IntegerData(reallyGiven), item, recipient);
	    ReportRecord * currentReport = new ReportRecord(new QuartenaryPattern(giveReporter, unit, new IntegerData(reallyGiven), item, recipient));
      unit->addReport( currentReport);
     }

        if (!recipient->isSilent())
	  {
//	    QuartenaryPattern * Message  = new QuartenaryPattern(receiveReporter, recipient , new IntegerData(reallyGiven), item, unit);
	    ReportRecord * currentReport = new ReportRecord(new QuartenaryPattern(receiveReporter, recipient , new IntegerData(reallyGiven), item, unit));
      recipient->addReport( currentReport);
	  }

    if(given > reallyGiven)
    {
        par2->setValue(given - reallyGiven);
        return IN_PROGRESS;
    }
        return SUCCESS;
	
}


