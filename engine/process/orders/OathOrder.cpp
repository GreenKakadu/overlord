/***************************************************************************
                             OathOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "OathOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "FactionEntity.h"
#include "TokenEntity.h"
#include "SimpleMessage.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
extern EntitiesCollection <UnitEntity>      units;
extern EntitiesCollection <FactionEntity>  factions;
extern ReportPattern *	missingParameterReporter;

OathOrder * instantiateOathOrder = new OathOrder();

OathOrder::OathOrder(){
  keyword_ = "Oath";
  registerOrder_();
  description = string("OATH  unit-id | faction-id \n") +
  "Immediate, one-shot.  This order executes immediately when you encounter the\n" +
  "target unit or any unit of target faction, and transfer's the loyalty of the\n" +
  "executing unit to that unit's faction, if different from your. If target faction\n" +
  "is not NPC this faction should be at least friendly to you. \n" +
  "The unit remains under your control, and\n" +
  "must still be paid for at end of turn, after which the unit will report to\n" +
  "the new faction.\n";

  orderType_   = IMMEDIATE_ORDER;
  mayInterrupt_ = true;
}

STATUS OathOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

    if(
    !parseOptionalGameDataParameter(entity,  parser, units,  parameters) &&
    !parseOptionalGameDataParameter(entity,  parser, factions,  parameters)
      )
      {
       entity->addReport(new BinaryMessage(missingParameterReporter,
 					new StringData(keyword_), new StringData("unit or faction id")));
        return IO_ERROR;
      }
  return OK;
// check faction or unit

}



ORDER_STATUS OathOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
  TokenEntity * unit = dynamic_cast<TokenEntity *>(entity);
  assert(unit);
  FactionEntity * faction = dynamic_cast<FactionEntity *>(parameters[0]);
  if(faction == 0)
    {
        TokenEntity * recipient = DOWNCAST_ENTITY<TokenEntity>(parameters[0]);
        assert(recipient);
        faction = recipient->getFaction();
    }

  return unit->oath(faction);
  	// controlled monsters?
}




