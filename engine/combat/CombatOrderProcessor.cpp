/***************************************************************************
                          CombatOrderProcessor.cpp
    Determine which orders to precess, maintain order list                .
                             -------------------
    begin                : Sun Oct 31 13:35:00 IST 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "CombatOrderProcessor.h"
#include "TokenEntity.h"
#include "ProcessingMode.h"
#include "BattleInstance.h"
#include <algorithm>
extern bool testMode;
extern ofstream combatReportFile;
//  Processes all possible ( at this phase) orders for Entity.
//  Each order processed only once
CombatOrderProcessor combatOrderProcessor;

bool CombatOrderProcessor::process(TokenEntity * entity, ProcessingMode * processingMode, int currentRound)

{
    bool orderWasExecuted = false;
    CombatOrderIterator currentIterator;
    ORDER_STATUS result;
    BattleInstance * instance = entity->getBattleInstantiation();

    if (!instance->isAlive())
        return false;
    if (instance->isFled())
        return false;
#ifdef TEST_MODE
    if (testMode) cout << "Processing orders for Entity " << entity->print() << endl;
#endif

    // We may have mandatory order like multi-hit order

    if (instance->isProcessingRepeatingOrder())
    {
        CombatOrderLine *current =instance->getCurrentlyRepeatingOrder();
//        result = instance->getCurrentlyRepeatingOrder()
        result = current->process(processingMode, entity, currentRound);
        //no processing results yet
        // Repeating order counter is updated during processing so
        // now we can check either it was last cycle
        if (instance->isProcessingRepeatingOrder())
        {
            return true;
        }
        //if this is a last cycle CurrentlyRepeatingOrder is 0
        // using old value to find it in orders list

        currentIterator = std::find((instance->getCombatOrderList()).begin(),
                                (instance->getCombatOrderList()).end(),
                                current);

       if(currentIterator!=(instance->getCombatOrderList()).end())
       {
            orderWasExecuted = processOrderResults(entity, result, currentIterator,
                                currentRound);
       }
       else
       {
           cout<<"Error: Can't find in "<<entity->print() <<" orders list order";
                   current->printOrderLine(cout);
           cout<<endl;
       }
    }
    else
    {

        if (entity->isTraced())
        {
            combatReportFile << "Processing orders for Entity "
                    << entity->print() << endl;
        }

        for (currentIterator = (instance->getCombatOrderList()).begin();
                currentIterator != (instance->getCombatOrderList()).end();)
        {
            if (entity->isTraced())
            {
                combatReportFile << "==== Trying to process ";
                (*currentIterator)->save(combatReportFile);
            }

            if ((*currentIterator)->ifConditionLevel > 0)
            {
                if (entity->isTraced())
                {
                    combatReportFile << "                     ifConditionLevel "
                            <<(*currentIterator)->ifConditionLevel <<endl;

                }
                currentIterator++;
                continue;
            }
            if ((*currentIterator)->ifStatementLevel > 0)
            {
                if (entity->isTraced())
                {
                    combatReportFile << "                     ifStatementLevel "
                            <<(*currentIterator)->ifStatementLevel <<endl;

                }
                currentIterator++;
                continue;
            }
            result = (*currentIterator) ->process(processingMode, entity, currentRound);

            if (instance->isProcessingRepeatingOrder()) // To be Repeated (Multi-hit)
            {
                return true;
            }

            if (result == SUSPENDED) // second pass needed.
            {
                return true;
            }

            orderWasExecuted = processOrderResults(entity, result, currentIterator,
                    currentRound);
        }// End of orders loop
    }
    if (!orderWasExecuted)
    {
        ; // process default order for this Entity(processingMode,this,cout)
    }
    return orderWasExecuted;
}



// Determine  orders that should be deleted from list
// Decrease repetition counters
// Mark full-round orders

bool CombatOrderProcessor::processOrderResults(TokenEntity * entity, ORDER_STATUS result, CombatOrderIterator & currentIterator, int currentRound)
{
    BattleInstance * instance = entity->getBattleInstantiation();
    assert(result != SUSPENDED);

    bool orderWasExecuted = false;
    if ((result == SUCCESS) || (result == IN_PROGRESS))
    {
        instance->setRoundFlags((*currentIterator)->getOrderType(), currentRound);
    }

    if (result != FAILURE)
    {
        orderWasExecuted = true;
    }

    switch (result)
    {
        case SUCCESS:
        {
#ifdef TEST_MODE
            if (testMode) cout << "==== Result of order processing is Success" << endl;
#endif
            postProcessOrder(entity, result, currentIterator);
            //              if((*currentIterator)->getCompletionFlag())
            //	      				{
            //				    		 delete (*currentIterator);
            //		    				 currentIterator = (instance->getCombatOrderList()).
            //								 																	erase(currentIterator);
            //				 				 break;
            //		  					}
            if ((*currentIterator) -> repetitionCounter() > 1)
            {
                (*currentIterator)->decrementRepetitionCounter();
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
                //				    		 delete (*currentIterator);
                currentIterator = (instance->getCombatOrderList()).
                        erase(currentIterator);
            }
            break;
        }
        case FAILURE:
        {
#ifdef TEST_MODE
            if (testMode) cout << "==== Result of order processing is Failure" << endl;
#endif
            currentIterator++;
            break;
        }
        case WAITING:
        {
#ifdef TEST_MODE
            if (testMode) cout << "==== Result of order processing is Waiting for initiative" << endl;
#endif
            currentIterator++;
            break;
        }
        case INVALID:
        {
#ifdef TEST_MODE
            if (testMode) cout << "==== Result of order processing is Invalid" << endl;
#endif
            postProcessOrder(entity, result, currentIterator);

            (*currentIterator) -> ~CombatOrderLine();
            currentIterator = (instance->getCombatOrderList())
                    .erase(currentIterator);
            break;
        }//end of INVALID case
        case IN_PROGRESS:
        {
#ifdef TEST_MODE
            if (testMode) cout << "==== Order is in progress" << endl;
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
                //				    		 delete (*currentIterator);
                currentIterator = (instance->getCombatOrderList())
                        .erase(currentIterator);
            }
            if ((*currentIterator) -> repetitionCounter() > 1)
            {
                (*currentIterator)->decrementRepetitionCounter();
                currentIterator++;
                break;
            }

            break;
        }
        case SUSPENDED:
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
CombatOrderProcessor::postProcessOrder(TokenEntity * entity, ORDER_STATUS result, CombatOrderIterator iter)

{
    BattleInstance * instance = entity->getBattleInstantiation();
    CombatOrderIterator currentIterator = iter;
    currentIterator++;
    for (; currentIterator != (instance->getCombatOrderList()).end();)
    {
        if (!((*currentIterator)->whileCondition()) &&
                !((*currentIterator)->ifConditionLevel > 0))
            return;
#ifdef TEST_MODE
        if (testMode)
        {
            cout << "====+++ Post-processing ";
            (*currentIterator)->save(cout);
        }
#endif
        switch (result)
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
                    if (testMode) cout << "====+++ Order deleted (condition failed)" << endl;
#endif
                    //delete (*currentIterator);
                    currentIterator = (instance->getCombatOrderList())
                            .erase(currentIterator);
                } else
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
                        if (testMode) cout << "====+++ Order deleted (impossible conditions)" << endl;
#endif
                        (*currentIterator) -> ~CombatOrderLine();
                        currentIterator = (instance->getCombatOrderList())
                                .erase(currentIterator);
                    } else
                        currentIterator++;
                    break;
                }

                if ((*currentIterator)->ifConditionLevel > 0) // Delete order
                {
#ifdef TEST_MODE
                    if (testMode) cout << "====+++ Order deleted (condition failed)" << endl;
#endif
                    (*currentIterator) -> ~CombatOrderLine();
                    currentIterator = (instance->getCombatOrderList())
                            .erase(currentIterator);
                }
                break;
            }
            case FAILURE:
            case WAITING:
            case IN_PROGRESS:
            case SUSPENDED:
                break;
        } // switch

    }// end of for cycle
}







