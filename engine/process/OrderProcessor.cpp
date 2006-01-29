/***************************************************************************
                          OrderProcessor.cpp
    Determine which orders to precess, maintain order list                .
                             -------------------
    begin                : Sun Oct 31 13:35:00 IST 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "OrderProcessor.h"
#include "Entity.h"
#include "ProcessingMode.h"
extern bool testMode;
//  Processes all possible ( at this phase) orders for Entity.
// Each order processed only once
OrderProcessor orderProcessor;
bool  OrderProcessor::process(Entity * entity, ProcessingMode * processingMode)

{
  bool orderWasExecuted = false;
  OrderIterator currentIterator ;
  ORDER_STATUS result;
#ifdef TEST_MODE
   if(testMode) 	cout<< "Processing orders for Entity " << entity->print() <<endl;
#endif

   for( currentIterator = (entity->getOrderList()).begin();
	                      currentIterator != (entity->getOrderList()).end();)
     {
			if( (*currentIterator)->ifConditionLevel > 0)
					{
						currentIterator++;
						continue;
					}
			if( (*currentIterator)->ifStatementLevel > 0)
					{
						currentIterator++;
						continue;
					}
      result = (*currentIterator) ->process(processingMode,  entity);
      if (result == SUSPENDED) // second pass needed
        return true;

      orderWasExecuted = processOrderResults(entity, result,currentIterator);
     }// End of orders loop
 if(!orderWasExecuted)
			; // process default order for this Entity(processingMode,this,cout)
   return orderWasExecuted;
}




bool OrderProcessor::processOrderResults(Entity * entity, ORDER_STATUS result, OrderIterator & currentIterator)
{
  assert (result != SUSPENDED);

  bool orderWasExecuted = false;
  if((result == SUCCESS) || (result == IN_PROGRESS))
  {
    if ((*currentIterator)->isFullDayOrder())
      {
//     cout << "Full-day order "; (*currentIterator)->save(cout); cout <<endl;
       entity->setLastOrder(*currentIterator);
       entity->setFullDayOrderFlag();
      }
  }

  if(result != FAILURE )
   {
		orderWasExecuted = true;
   }

  switch (result)
  {
					case SUCCESS:
	  				{
#ifdef TEST_MODE
   if(testMode) 	    cout << "==== Result of order processing is Success" << endl;
#endif
	    				postProcessOrder(entity, result, currentIterator);
              if((*currentIterator)->getCompletionFlag())
	      				{
				    		 delete (*currentIterator);
		    				 currentIterator = (entity->getOrderList()).erase(currentIterator);
				 				 break;
		  					}
	    				if ((*currentIterator) -> repetitionCounter() > 1)
	      				{
         					(*currentIterator)->decrementRepetitionCounter()  ;
									currentIterator++;
				 					break;
	      				}

	    				if ((*currentIterator)->isPermanent())
	      				{
									currentIterator++;
				 					break;
	      				}

	    				else
	      				{
				    		 delete (*currentIterator);
		    				 currentIterator = (entity->getOrderList()).erase(currentIterator);
		  					}
							break;
	  				}
			case FAILURE:
	  				{
#ifdef TEST_MODE
   if(testMode) 	    cout << "==== Result of order processing is Failure" << endl;
#endif
							currentIterator++;
	    				break;
	  				}
			case INVALID:
	  				{
#ifdef TEST_MODE
   if(testMode) 	    cout << "==== Result of order processing is Invalid" << endl;
#endif
	    				postProcessOrder(entity, result, currentIterator);

				    	(*currentIterator) -> ~OrderLine();
		    			currentIterator = (entity->getOrderList()).erase(currentIterator);
	    				break;
	  				}//end of INVALID case
			case IN_PROGRESS:
          {
#ifdef TEST_MODE
   if(testMode) 	    cout << "==== Order is in progress" << endl;
#endif
//              if((*currentIterator)->getCompletionFlag())
//	      				{
//				    			  delete (*currentIterator);
//		    					  currentIterator = orders_.erase(currentIterator);
//				 					  break;
//		  					}

          if ((*currentIterator)->isPermanent())
	      				{
									currentIterator++;
				 					break;
	      				}
	    				if ((*currentIterator) -> repetitionCounter() == 1)
	      				{
				    		 delete (*currentIterator);
		    				 currentIterator = (entity->getOrderList()).erase(currentIterator);
		  					}
	    				if ((*currentIterator) -> repetitionCounter() > 1)
	      				{
         					(*currentIterator)->decrementRepetitionCounter()  ;
									currentIterator++;
				 					break;
	      				}

	    				break;
            }
			case SUSPENDED:
			case WAITING: // Not used
	    				break;
	  //default:
				}// End of result switch
  return orderWasExecuted;
}


/*
 * When order completed  state of all conditional orders depending on it
 * should be reexamined:
 * For successfuly completed orders "-" modifiers should be removed and
 * orders with "+" removed.
 * For orders completed with invalid status  "+" modifiers should be
 * removed and orders with "-" removed.

 */
void
OrderProcessor::postProcessOrder(Entity * entity, ORDER_STATUS result, OrderIterator  iter)

{
  OrderIterator    currentIterator = iter;
  currentIterator++;
  for ( ; currentIterator != (entity->getOrderList()).end(); )
    {
			if( !((*currentIterator)->whileCondition()) && !((*currentIterator)->ifConditionLevel > 0))
			return;
#ifdef TEST_MODE
   if(testMode)
			{
				cout << "====+++ Post-processing " ;      (*currentIterator)->save(cout);
			}
#endif
      switch(result)
			{
				case SUCCESS:
	  			{
	    			if ((*currentIterator)->ifConditionLevel > 0)
	      			{
								(*currentIterator)->ifConditionLevel--;
							}
		// Checking next conditional modifier
		// if it is "+" Order will be deleted in the next "if" (combination "-+" is illegal (?))
		// otherwise put "else"

	    			if ((*currentIterator)->whileCondition()) // Delete order
	      			{
#ifdef TEST_MODE
   if(testMode) 		cout << "====+++ Order deleted (condition failed)"<<endl;
#endif
				    	delete (*currentIterator);
		    			currentIterator = (entity->getOrderList()).erase(currentIterator);
	      			}
	      		else
	      			{
		    				currentIterator++;
		    			}
	    		break;
	  		}
			case INVALID:
	  		{
	    		if ((*currentIterator)->whileCondition()) // +
	      		{
					 		(*currentIterator)->setWhileCondition(false);
								// Checking next conditional modifier
								// if any exists delete order
							if ((*currentIterator)->ifConditionLevel > 0) // Delete order (and Node)
		  					{
#ifdef TEST_MODE
   if(testMode) 		    cout << "====+++ Order deleted (impossible conditions)"<<endl;
#endif
		    					(*currentIterator)  ->  ~OrderLine();
		    						currentIterator = (entity->getOrderList()).erase(currentIterator);
		  					}
							else
		    						currentIterator++;
	    				break;
	      		}

	    		if ((*currentIterator)->ifConditionLevel > 0) // Delete order
	      		{
#ifdef TEST_MODE
   if(testMode) 		cout << "====+++ Order deleted (condition failed)"<<endl;
#endif
				    	(*currentIterator) -> ~OrderLine();
		    			currentIterator = (entity->getOrderList()).erase(currentIterator);
	      		}
	    		break;
	  		}
				case FAILURE:
				case IN_PROGRESS:
				case SUSPENDED:
			  case WAITING: // Not used
	    		break;
		} // switch

   }// end of for cycle
}







