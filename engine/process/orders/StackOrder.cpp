/***************************************************************************
                          StackOrder.cpp
                             -------------------
    begin                : Mon Mar 3 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "StackOrder.h"
#include "UnstackOrder.h"
#include "StringData.h"
#include "StanceVariety.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "FactionEntity.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
extern EntitiesCollection <UnitEntity>      units;
extern ReportPattern *	invalidParameterReporter;
extern ReportPattern *	stackingUnacceptableReporter;
extern ReportPattern *	stackReporter;
extern ReportPattern *	unstackReporter;

//StackOrder instantiateStackOrder;
StackOrder * instantiateStackOrder = new StackOrder();

StackOrder::StackOrder(){
   keyword_ = "stack";
  registerOrder_();
  description = string("STACK [unit-id] \n") +
  "Immediate, one-shot.  This order executes when a unit is at the same location\n" +
  "as the designed unit.  The unit UNSTACKs, if necessary, and stacks under the\n" +
  "leadership of the designated unit.  If the unit under which you are\n" +
  "attempting to stack doesn't consider you at least ally, this order will\n" +
  "fail unless the leader issued the ACCEPT order.  You are always visible to\n" +
  "the unit under which you are stacking.\n" +
  "\n" +
  "If no unit id is specified, the unit will UNSTACK, but will not stack under\n" +
  "anyone specific.\n";
  orderType_   = IMMEDIATE_ORDER;
}



STATUS StackOrder::loadParameters(Parser * parser, vector <AbstractData *>  &parameters, Entity * entity )
{
   if(!entityIsUnit(entity))
            return IO_ERROR;

   UnitEntity * leader;
   const string tag = parser->getWord();

   if (tag.size() == 0)  // Missing parameter
        {
          leader = 0;
          return OK;
        }

  if (!units.checkDataType(tag)) // this can't be a tag
				{
         entity->addReport(new TertiaryMessage(invalidParameterReporter, new StringData(keyword_), new StringData(tag), new StringData("unit id")));
         return IO_ERROR;
				}

    leader = units[tag];

  if( leader == 0) // item doesn't exist but we don't want to let player to know that
				{
          StringData * dummy = new StringData(tag);
   		     parameters.push_back(dummy);
				}
   else
				{
   		     parameters.push_back(leader);
				}

  return OK;
}



ORDER_STATUS StackOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);

  UnitEntity * formerLeader = unit->getLeader();

  OrderLine * orderId = entity->getCurrentOrder();
  UnaryMessage * unstackMessage = new UnaryMessage(unstackReporter, unit);
  if (parameters.size() == 0) // This means unstack
    {
      if(unit->unstack())
        {
          entity->addReport(unstackMessage,orderId,0 );
		      formerLeader->addReport(unstackMessage,orderId,0 );
          }

      return SUCCESS;
    }

//  UnitEntity * leader = dynamic_cast<UnitEntity *>(parameters[0]);
	UnitEntity * leader   =  DOWNCAST_ENTITY<UnitEntity>(parameters[0]);
  if(leader == 0)
   		  return FAILURE;

// unit and leader are at the same location
 if(!unit->mayInterract(leader))
   		  return FAILURE;
// leader is ally or accepts unit
//   cout << "Stance: "<< leader->getFaction()->getStance(unit)<<" "
//        <<leader->getFaction()->getStance(unit)->getCode()<< " vs "
//        <<stances["ally"]<<" "<<(stances["ally"])->getCode();
//   if (leader->isAccepting(unit))
//    cout << " accepting " <<endl;
//    else
//    cout << " not accepting " <<endl;
   if((*(leader->getFaction()->getStance(unit)) >= *alliedStance) ||(leader->isAccepting(unit)))
      {
        if(unit->unstack())
        {
          entity->addReport(unstackMessage,orderId,0  );
          if(formerLeader)
		          formerLeader->addReport(unstackMessage,orderId,0  );
        }


        stack(unit,leader );
           BinaryMessage * stackMessage = new BinaryMessage(stackReporter, unit, leader);
          unit->addReport(stackMessage,orderId,0);
          leader->addReport(stackMessage,orderId,0);

 		      return SUCCESS;
      }
    else // rejected
     {
        BinaryMessage * stackingUnacceptableMessage = new BinaryMessage(stackingUnacceptableReporter, leader , unit);
       unit->addReport (stackingUnacceptableMessage,orderId,0 );
      leader->addReport(stackingUnacceptableMessage,orderId,0 );
 		  return INVALID;
      }



}
