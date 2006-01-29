/***************************************************************************
                          OrderProcessor.h
    Determine which orders to precess, maintain order list                .
                             -------------------
    begin                : Sun Oct 31 13:35:00 IST 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef ORDER_PROCESSOR_H
#define ORDER_PROCESSOR_H
#include "OrderLine.h"
#include "OverlordTypes.h"

using namespace std;
typedef vector<OrderLine *>::iterator OrderIterator;


class OrderProcessor
{
    public:
			OrderProcessor(){}
			~OrderProcessor(){}
   static bool      process(Entity * entity, ProcessingMode * processingMode);
   static bool      processOrderResults(Entity * entity, ORDER_STATUS result,
                                    OrderIterator & currentIterator);
   static void 	   postProcessOrder(Entity * entity, ORDER_STATUS result, OrderIterator  iter);
	  protected:
    private:
};

#endif







