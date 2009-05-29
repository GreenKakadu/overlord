/***************************************************************************
                          UnstackOrder.cpp
                             -------------------
    begin                : Mon Mar 3 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "UnstackOrder.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "UnaryMessage.h"
extern ReportPattern *	unstackReporter;

//UnstackOrder instantiateUnstackOrder;
UnstackOrder * instantiateUnstackOrder = new UnstackOrder();

UnstackOrder::UnstackOrder(){
   keyword_ = "unstack";
  registerOrder_();
  description = string("UNSTACK \n") +
  "Immediate, one-shot.  This order always executes.  When a unit is in a stack,\n" +
  "but not leading the entire stack, the unit departs from the stack.  If\n" +
  "leading a substack, it will become the leader of the new stack.  If not in\n" +
  "any stack, does nothing.\n";
  orderType_   = IMMEDIATE_ORDER;
}



ORDER_STATUS UnstackOrder::process (Entity * entity, ParameterList &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);

//   if(unit->isTraced())
//   {
//     cout << unit->print()<< " is trying to unstack"<<endl;
//   }
    UnitEntity * formerLeader = unit->getLeader();
  if(unit->unstack())
     {
      UnaryMessage *  unstackMessage = new UnaryMessage(unstackReporter, unit);
		  unit->addReport(unstackMessage, entity->getCurrentOrder(),0 );
      if(formerLeader)
		  formerLeader->addReport(unstackMessage, entity->getCurrentOrder(),0  );
      }
   // already unstacked
         return SUCCESS;
}
