/***************************************************************************
                             MarchOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "MarchOrder.h"
#include "TokenEntity.h"

MarchOrder * instantiateMarchOrder = new MarchOrder();

MarchOrder::MarchOrder(){
  keyword_ = "March";
  registerOrder_();
  description = string("MARCH [direction|location-id] \n") +
  "Full-day, leader/creature-only, one-shot.  This orders is identical to MOVE,\n" +
  "except that, instead of retreating if a unit on PATROL attempts to prevent your\n" +
  "entry, you will enter a battle.\n";

    fullDayOrder_= true;
  orderType_   = STACK_ORDER;
}



ORDER_STATUS MarchOrder::process (Entity * entity, ParameterList &parameters)
{

  TokenEntity * tokenEntity = dynamic_cast<TokenEntity *>(entity);
  assert(tokenEntity);
  return move(tokenEntity,parameters[0], true);
}

