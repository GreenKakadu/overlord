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
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "TertiaryPattern.h"
#include "EntitiesCollection.h"
extern EntitiesCollection <UnitEntity>      units;
extern VarietiesCollection <StanceVariety>    stances;
extern Reporter *	invalidParameterReporter;
extern Reporter *	stackingUnacceptableReporter;
extern Reporter *	stackReporter;
extern Reporter *	unstackReporter;

StackOrder::StackOrder(){
   keyword_ = "stack";
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
         entity->addReport(new TertiaryPattern(invalidParameterReporter, new StringData(keyword_), new StringData(tag), new StringData("unit id")));
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


ORDER_STATUS StackOrder::process (Entity * entity, vector <AbstractData *>  &parameters, Order * orderId)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);

  UnitEntity * formerLeader = unit->getLeader();

  if (parameters.size() == 0) // This means unstack
    {
      if(unit->unstack())
        {
          entity->addReport(new   ReportRecord(new UnaryPattern(unstackReporter, unit)) );
		      formerLeader->addReport(new   ReportRecord(new UnaryPattern(unstackReporter, unit)) );
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
   if((*(leader->getFaction()->getStance(unit)) >= *allied) ||(leader->isAccepting(unit)))
      {
        if(unit->unstack());
        {
          entity->addReport(new   ReportRecord(new UnaryPattern(unstackReporter, unit)) );
          if(formerLeader)
		          formerLeader->addReport(new   ReportRecord(new UnaryPattern(unstackReporter, unit)) );
        }

        
        stack(unit,leader );
        
          unit->addReport(new   ReportRecord(new BinaryPattern(stackReporter, unit, leader)) );
          leader->addReport(new   ReportRecord(new BinaryPattern(stackReporter, unit, leader)) );

 		      return SUCCESS;
      }
    else // rejected
     {
       unit->addReport ( new   ReportRecord(new BinaryPattern(stackingUnacceptableReporter, leader , unit)));
      leader->addReport( new   ReportRecord(new BinaryPattern(stackingUnacceptableReporter, leader , unit)));
 		  return INVALID;
      }

                                    

}
