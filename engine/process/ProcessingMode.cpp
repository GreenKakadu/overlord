/***************************************************************************
                          ProcessingMode.cpp  -  description
                             -------------------
    begin                : Thu Nov 28 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#include "ProcessingMode.h"

ProcessingMode::ProcessingMode(const ORDER_TYPE type)
{
   orderType_ =   type;
}
ProcessingMode::~ProcessingMode(){
}
/** Checks for order of specified type may be executed  */
bool ProcessingMode::mayExecute(ORDER_TYPE type) const
{
  if(type ==  INSTANT_ORDER)
     return true;
  else   
    return (type == orderType_);
}
/** In Repetive mode orders processing should be repeated if at least one of orders
was successfully executed.
In Non-Repetitive mode only one attempt of order execution will be done. */
bool ProcessingMode::isRepetitive()
{
	return ( orderType_ == IMMEDIATE_ORDER);	
}
