/***************************************************************************
                          GiveOrder.h  -  description
                             -------------------
    begin                : Tue Jan 7 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef GIVE_ORDER_H
#define GIVE_ORDER_H

#include "OrderPrototype.h"
class Reporter;

class GiveOrder : public OrderPrototype
{
    public:
  GiveOrder();

  STATUS loadParameters(Parser * parser, vector <AbstractData *>  &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, vector <AbstractData *>  &parameters, Order * orderId);
  /** No descriptions */
    protected:
    private:
//  static const UINT NO_GIFTS_REPORT_FLAG;



};
#endif

