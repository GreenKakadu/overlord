/***************************************************************************
                          UnstackOrder.cpp 
                             -------------------
    begin                : Mon Mar 3 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "UnstackOrder.h"
#include "Entity.h"
#include "UnitEntity.h"
#include "UnaryPattern.h"
extern Reporter *	unstackReporter;

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



ORDER_STATUS UnstackOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
  UnitEntity * unit = dynamic_cast<UnitEntity *>(entity);
  assert(unit);

  UnitEntity * formerLeader = unit->getLeader();    
  if(unit->unstack())
     {
      UnaryPattern *  unstackMessage = new UnaryPattern(unstackReporter, unit);
		  unit->addReport(unstackMessage, entity->getCurrentOrder(),0 );
      if(formerLeader)
		  formerLeader->addReport(unstackMessage, entity->getCurrentOrder(),0  );
      }
   // already unstacked     
         return SUCCESS;
}
