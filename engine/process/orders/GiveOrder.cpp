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
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "QuartenaryMessage.h"
#include "EntitiesCollection.h"
#include "RulesCollection.h"
#include "ItemRule.h"
#include "StanceVariety.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "FactionEntity.h"

//GiveOrder instantiateGiveOrder;
GiveOrder * instantiateGiveOrder = new GiveOrder();

extern EntitiesCollection <UnitEntity>      units;
extern RulesCollection <ItemRule>      items;
extern ReportPattern *	giveRejectedReporter;
extern ReportPattern *	giveReporter;
extern ReportPattern *	receiveReporter;
//const UINT GiveOrder::NO_GIFTS_REPORT_FLAG = 0x01;

GiveOrder::GiveOrder()
{
  keyword_ = "give";
  registerOrder_();
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
GiveOrder::process (Entity * entity, vector < AbstractData*>  &parameters)
{

  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);

	UnitEntity * recipient   =  DOWNCAST_ENTITY<UnitEntity>(parameters[0]);

  ItemRule * item          =  dynamic_cast<ItemRule *>(parameters[1]);
  assert(item);

  int given =   getIntegerParameter(parameters,2);

  int kept =   getIntegerParameter(parameters,3);

  OrderLine * orderId = entity->getCurrentOrder();


 if (!unit->mayInterract(recipient)) // Not In the same place or can't see
	  return FAILURE;

 if(*(recipient->getFaction()->getStance(unit)) < *friendlyStance)
      {
        // not accepting. Reports to both sides
      UnaryMessage * giveRejectedMessage = new UnaryMessage(giveRejectedReporter, recipient);
      unit->addReport(giveRejectedMessage,orderId,0);
      recipient->addReport(giveRejectedMessage,orderId,0);
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


        if (!unit->isSilent() && unit->getCurrentOrder()->isNormalReportEnabled()   )
	  {
      unit->addReport( new QuartenaryMessage(giveReporter, unit, new IntegerData(reallyGiven), item, recipient),orderId,0);
     }

        if (!recipient->isSilent())
	  {
      recipient->addReport( new QuartenaryMessage(receiveReporter, recipient , new IntegerData(reallyGiven), item, unit),orderId,0);
	  }

    if(given > reallyGiven)
    {
        IntegerData * par       =  dynamic_cast<IntegerData *>(parameters[2]);
        assert(par);
        par->setValue(given - reallyGiven);
        return IN_PROGRESS;
    }
        return SUCCESS;

}


