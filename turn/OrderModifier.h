/***************************************************************************
                          OrderModifier.h 
                    .
                             -------------------
    begin                : Tue Nov  5 11:46:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef ORDER_MODIFIER_H
#define ORDER_MODIFIER_H
#include <iostream>
#include "Parser.h"

enum modifier_type {
  NONE               = 0,
  PERMANENT_ORDER    = 1,
  DAY_SPECIFIC_ORDER = 2,
  IF_CONDITION       = 3,
  WHILE_CONDITION    = 4,
  COUNTER_CONDITION  = 5,
  NO_ERROR_REPORT    = 6,
  NO_NORMAL_REPORT    = 7
                   };

typedef enum modifier_type MODIFIER_TYPE;

#define PERMANENT_ORDER_SYMBOL '@'
#define DAY_SPECIFIC_ORDER_SYMBOL 'd'
#define IF_CONDITION_SYMBOL '-'
#define WHILE_CONDITION_SYMBOL '+'
#define NO_ERROR_REPORT_SYMBOL '!'
#define NO_NORMAL_REPORT_SYMBOL '|'

class OrderModifier
{
    public:
       OrderModifier(MODIFIER_TYPE  type, int  value);
       ~OrderModifier();
       MODIFIER_TYPE getType() const;
       int getCounter() const;
       bool decreaseCounter();
       void save(ostream &out);
       bool isControlModifier() const;
       bool isReportModifier() const;
 static bool fetchModifier(Parser * parser, MODIFIER_TYPE & type, int & value);
    protected:
       MODIFIER_TYPE type_;
       int counter_;
    private:
};
#endif

