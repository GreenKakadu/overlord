/***************************************************************************
                          ProcessingMode.h  -
       Defines the mode of order procession (that may be simulation mode,
       testing mode, processing of immediate orders or day-long orders only)
                             -------------------
    begin                : Thu Nov 28 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef PROCESSING_MODE_H
#define PROCESSING_MODE_H
enum order_type {
  DEFAULT                        = 0,
  INSTANT_ORDER      = 1,
  IMMEDIATE_ORDER      = 2,
  STACK_ORDER    = 3,
  DAY_LONG_ORDER = 4
                   };

typedef enum order_type ORDER_TYPE;


class ProcessingMode {
public: 
	ProcessingMode(const ORDER_TYPE type);
	~ProcessingMode();
  /** Checks for order of specified type may be executed  */
  bool mayExecute(ORDER_TYPE type) const;
  /** In Repetive mode orders processing should be repeated if at least one of orders
was successfully executed.
In Non-Repetitive mode only one attempt of order execution will be done. */
  bool isRepetitive();
protected:
	ORDER_TYPE orderType_;
};

#endif
