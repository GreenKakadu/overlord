/***************************************************************************
                             AcceptOrder.cpp
                             -------------------
    begin                : Thu Mar 6 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "AcceptOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
extern EntitiesCollection <UnitEntity>      units;
extern ReportPattern *	invalidParameterReporter;
extern ReportPattern *	missingParameterReporter;
extern ReportPattern * acceptOwnReporter;
extern ReportPattern *	acceptReporter;

AcceptOrder * instantiateAcceptOrder = new AcceptOrder();

AcceptOrder::AcceptOrder(){
  keyword_ = "accept";
  registerOrder_();
  description = string("ACCEPT [unit-id] \n") +
  "Immediate, special.  This order executes when the designated unit attempts to\n" +
  "STACK itself under your leadership.  The unit is allowed to do so, regardless\n" +
  "of the stance toward its faction.\n";

  orderType_   = IMMEDIATE_ORDER;
  mayInterrupt_ = true;
}

STATUS AcceptOrder::loadParameters(Parser * parser,
                            ParameterList &parameters, Entity * entity )
{
   if(!entityIsTokenEntity(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, units, "unit id", parameters))
            return IO_ERROR;

  return OK;

}



ORDER_STATUS AcceptOrder::process (Entity * entity, ParameterList &parameters)
{
  TokenEntity * unit = dynamic_cast<TokenEntity *>(entity);
 assert(unit);
	UnitEntity * follower   =  DOWNCAST_ENTITY<UnitEntity>(parameters[0]);

  if ( follower == 0) // unit doesn't exist but we don't want to let player to know that
     {
 		  return FAILURE;
      }
  if(unit->getFaction() == follower->getFaction())
  {
    unit->addReport(new UnaryMessage(acceptOwnReporter, follower) );
		  return SUCCESS;
    }

  if(unit->isAccepted(follower))
    {
      unit->clearAccept(follower);
		  return SUCCESS;
    }
   if(unit->isAccepting(follower))
    {
		  return FAILURE;
    }
   else
    {
      unit->accept(follower);
     unit->addReport(new UnaryMessage(acceptReporter, follower) );
      }
	return FAILURE;
}
