/***************************************************************************
                             AcceptOrder.cpp 
                             -------------------
    begin                : Thu Mar 6 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "AcceptOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "TertiaryPattern.h"
#include "EntitiesCollection.h"
extern EntitiesCollection <UnitEntity>      units;
extern Reporter *	invalidParameterReporter;
extern Reporter *	missingParameterReporter;
extern Reporter * acceptOwnReporter;
extern Reporter *	acceptReporter; 

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
                            vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsTokenEntity(entity))
            return IO_ERROR;

    if(!parseGameDataParameter(entity,  parser, units, "unit id", parameters))
            return IO_ERROR;

  return OK;

}



ORDER_STATUS AcceptOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
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
    unit->addReport(new UnaryPattern(acceptOwnReporter, follower) );
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
     unit->addReport(new UnaryPattern(acceptReporter, follower) );
      }
	return FAILURE;
}
