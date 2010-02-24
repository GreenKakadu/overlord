/***************************************************************************
                          ProcessingMode.h  -
       Defines the mode of order procession (that may be simulation mode,
       testing mode, processing of immediate orders or day-long orders only)
                             -------------------
    begin                : Thu Nov 28 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#ifndef PROCESSING_MODE_H
#define PROCESSING_MODE_H
enum order_type {
  DEFAULT                        = 0,
  INSTANT_ORDER      = 1,
  IMMEDIATE_ORDER      = 2,
  REQUEST_ORDER      = 3,
  STACK_ORDER    = 4,
  DAY_LONG_ORDER = 5,
  COMBAT_ACTION_ORDER = 6,
  COMBAT_MOVEMENT_ORDER = 7,
  ORDER_TYPE_END = 10
                   };

typedef enum order_type ORDER_TYPE;


class ProcessingMode {
public: 
	ProcessingMode(const ORDER_TYPE type);
	ProcessingMode();
	~ProcessingMode();
  /** Checks for order of specified type may be executed  */
  bool mayExecute(ORDER_TYPE type) const;
  /** In Repetive mode orders processing should be repeated if at least one of orders
was successfully executed.
In Non-Repetitive mode only one attempt of order execution will be done. */
  bool isRepetitive();
	inline void setInitiative(int initiative){initiative_ = initiative;}
	inline  int getInitiative() const {return initiative_;}
	inline  void decreaseInitiative()  {initiative_--;}
	static const int MAX_INITIATIVE;
	static const int MIN_INITIATIVE;
protected:
	ORDER_TYPE orderType_;
	int initiative_;
};
extern ProcessingMode   immediateOrders;
extern ProcessingMode   requestOrders;
extern ProcessingMode   stackOrders;
extern ProcessingMode 	dayOrders;

#endif
