/***************************************************************************
                          CombatOrderProcessor.h
    Determine which orders to precess, maintain order list                .
                             -------------------
    begin                : Sun Oct 31 13:35:00 IST 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#ifndef COMBAT_ORDER_PROCESSOR_H
#define COMBAT_ORDER_PROCESSOR_H
#include "OverlordTypes.h"
#include "CombatOrderLine.h"
#include "OrderProcessor.h"
using namespace std;
class TokenEntity;


class CombatOrderProcessor : public  OrderProcessor
{
    public:
			CombatOrderProcessor(){}
			~CombatOrderProcessor(){}
   static bool      process(TokenEntity * entity, ProcessingMode * processingMode, int currentRound);
   static bool      processOrderResults(TokenEntity * entity,
	 		ORDER_STATUS result, CombatOrderIterator & currentIterator,
			int currentRound);
   static void 	   postProcessOrder(TokenEntity * entity, ORDER_STATUS result, CombatOrderIterator  iter);
	  protected:
    private:
};

#endif







