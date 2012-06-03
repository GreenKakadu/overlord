/***************************************************************************
                          WithdrawOrder.cpp  -  description
                             -------------------
    begin                : Tue Nov 26 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "WithdrawOrder.h"
#include "ReportPattern.h"
#include "StringData.h"
#include "TertiaryMessage.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "SimpleMessage.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
#include "RulesCollection.h"
#include "TerrainRule.h"
#include "ItemRule.h"
#include "ItemElement.h"
#include "IntegerData.h"
#include "StringData.h"
const UINT WithdrawOrder:: WITHDRAW_RESTRICTED_REPORT_FLAG= 0x01;



extern ReportPattern * withdrawInvalidReporter;
extern ReportPattern *	invalidParameterReporter;
extern ReportPattern * withdrawRestrictedReporter;
extern ReportPattern * withdrawUnitReporter;
extern ReportPattern * withdrawFactionReporter;
extern ReportPattern * withdrawFundEmptyReporter;

//WithdrawOrder instantiateWithdrawOrder;
WithdrawOrder * instantiateWithdrawOrder = new WithdrawOrder();

WithdrawOrder::WithdrawOrder(){
  keyword_ = "withdraw";
  registerOrder_();
  description = string("WITHDRAW  amount [item]\n") +
 string( "Immediate.  This order executes automatically if you are in a city location.\n") +
  "It withdraws as many coins (or items if specified) as possible from your faction funds, up to the\n" +
  "amount indicated.  If zero is specified, no coins are withdrawn; to withdraw\n" +
  "all, specify a very large amount.\n" +
  "\n" +
  "Withdrawing is not mandatory; any time a unit in a city requires coins for\n" +
  "an action, it will withdraw automatically from the faction funds to cover\n" +
  "any lack of coins.\n";
  orderType_   = IMMEDIATE_ORDER;
  mayInterrupt_ = true;
}



STATUS
WithdrawOrder::loadParameters(Parser * parser, ParameterList &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;
  if (!parseIntegerParameter(parser, parameters))
      {
        entity->addReport(new TertiaryMessage(invalidParameterReporter,
                                new StringData(keyword_),
                                new StringData(parser->getWord()),
                                new StringData(" integer ")));
        return IO_ERROR;
      }
  parseOptionalGameDataParameter(entity, parser, gameFacade->items, parameters);
  return OK;

}



ORDER_STATUS
WithdrawOrder::process (Entity * entity, ParameterList &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);
  OrderLine * orderId = unit->getCurrentOrder();
// May withdraw only in city (or bank)
//   if(unit->getLocation()->getTerrain() != terrains["city"])
   if(unit->getLocation()->getTerrain() != gameFacade->terrains.findByTag("city",false))
   {
     if(!orderId->getReportingFlag(WITHDRAW_RESTRICTED_REPORT_FLAG ))
      {
        unit->addReport(new SimpleMessage(withdrawRestrictedReporter));
        orderId->setReportingFlag(WITHDRAW_RESTRICTED_REPORT_FLAG );
      }
 	    return FAILURE;
   }
   orderId->clearReportingFlag(WITHDRAW_RESTRICTED_REPORT_FLAG);

   IntegerData * par       =  dynamic_cast<IntegerData *>(parameters[0]);
      assert(par);
   int amount                 =  par->getValue();
   ItemRule * item;
   if(parameters.size() > 1)
   {
    item = dynamic_cast <ItemRule *>(parameters[1]);
   }
    else
      item = cash;
   if(item == 0)
      {
        unit->addReport(new UnaryMessage(withdrawInvalidReporter, parameters[1]));
      return INVALID;

      }
// if number is negative and in the bank and have that number of money may deposit // obsolete
   if(amount < 0)
    {
      cout << "deposits are not supported yet" << endl;
      return INVALID;
    }

    int realAmount = unit->getFaction()->withdraw(item,amount);
    if(realAmount == 0)
    {
        unit->addReport(new UnaryMessage(withdrawInvalidReporter, item));
      return INVALID;
      }
    unit->addToInventory(item,realAmount);
//QQQ
        unit->addReport(new UnaryMessage(withdrawUnitReporter, new ItemElement(item, realAmount)));
        unit->getFaction()->addReport(new BinaryMessage(withdrawFactionReporter, new ItemElement(item, realAmount), unit));
    if( amount > realAmount)
      {
        unit->getFaction()->addReport(new UnaryMessage(withdrawFundEmptyReporter, item));
      }
 	return SUCCESS;
}
