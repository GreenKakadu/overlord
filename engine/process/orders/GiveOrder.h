/***************************************************************************
                          GiveOrder.h  -  description
                             -------------------
    begin                : Tue Jan 7 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef GIVE_ORDER_H
#define GIVE_ORDER_H

#include "OrderPrototype.h"
class ReportPattern;

class GiveOrder : public OrderPrototype
{
    public:
  GiveOrder();

  STATUS loadParameters(Parser * parser, ParameterList &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, ParameterList &parameters);
  /** No descriptions */
    protected:
    private:
//  static const UINT NO_GIFTS_REPORT_FLAG;



};
#endif

