/***************************************************************************
                          DataManipulator.cpp  

                             -------------------
    begin                : Wen May 22 13:52:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include <functional>
#include <algorithm>
#include "Global.h"
#include "DataManipulator.h"

#include "orders/WithdrawOrder.h"
#include "orders/TestOrder.h"
#include "orders/MoveOrder.h"
#include "orders/AcceptOrder.h"
#include "orders/GiveOrder.h"
#include "orders/StudyOrder.h"
#include "orders/StackOrder.h"
#include "orders/UnstackOrder.h"
#include "orders/EquipOrder.h"
#include "general/DataStorageHandler.h"

#include "varieties/MovementVariety.h"


DataManipulator::DataManipulator()
{
// Find better place for this
//movementModes.push_back(walk);
//movementModes.push_back(ride);
//movementModes.push_back(swim);
//movementModes.push_back(fly);
//movementModes.push_back(climb);

   addVarieties  (&movementModes);
   addVarieties  (&directions);
   addVarieties  (&stances);
   addVarieties  (&equipments);
    
   addRules (&terrains);
   addRules (&titles);
   addRules (&skills);
   addRules (&items);
   addRules (&races);


//... add more collections
   addEntities  (&units);
   addEntities  (&factions);
   addEntities  (&locations);

   addOrderPrototypes(&orderPrototypesCollection);

}



DataManipulator::~DataManipulator()
{
}



void DataManipulator::addRules ( BasicRulesCollection  * collection )
{
		if(collection->getStatus() == OK)
			{
 				rules_.push_back(collection);
			}
}



void DataManipulator::addEntities ( BasicEntitiesCollection * collection)
{
		if(collection->getStatus()  == OK)
			{
 				entities_.push_back(collection);
			}
}


void DataManipulator::addVarieties ( BasicVarietiesCollection * collection)
{
		if(collection->getStatus()  == OK)
			{
 				varieties_.push_back(collection);
			}
}





STATUS DataManipulator::load()
{
  RulesCollectionIterator ruleIter;
  EntitiesCollectionIterator iter;
  VarietiesCollectionIterator varietyIter;
 STATUS status= OK;
 for (varietyIter = varieties_.begin(); varietyIter != varieties_.end(); varietyIter++)
   {
      if (ERROR == (*varietyIter)->getStorageHandler()->load())
			status = ERROR;
   }
 for (ruleIter = rules_.begin(); ruleIter != rules_.end(); ruleIter++)
   {
      if (ERROR == (*ruleIter)->getStorageHandler()->load())
			status = ERROR;
//      (*ruleIter)->getStorageHandler()->print();
   }
 for (iter = entities_.begin(); iter != entities_.end(); iter++)
   {
      if (ERROR == (*iter)->getStorageHandler()->load())
			status = ERROR;
 //     (*iter)->getStorageHandler()->print();
   }
 return status;
}
STATUS DataManipulator::save()
{
  EntitiesCollectionIterator iter;
 for (iter = entities_.begin(); iter != entities_.end(); iter++)
   {
      (*iter) ->getStorageHandler()->save();
   }
 return OK;
}

STATUS DataManipulator::initialize()
{
  RulesCollectionIterator ruleIter;
  EntitiesCollectionIterator iter;
  VarietiesCollectionIterator varietyIter;

  STATUS status= OK;
for (varietyIter = varieties_.begin(); varietyIter != varieties_.end(); varietyIter++)
   {
      if (ERROR == (*varietyIter)->getStorageHandler()->initializeData())
			status = ERROR;
   }

 for (ruleIter = rules_.begin(); ruleIter != rules_.end(); ruleIter++)
   {
      if (ERROR == (*ruleIter) ->getStorageHandler()->initializeData())
			status = ERROR;
   }

for (iter = entities_.begin(); iter != entities_.end(); iter++)
   {
      if (ERROR == (*iter) ->getStorageHandler()->initializeData())
			status = ERROR;
   }


 return status;
}
void DataManipulator::print()
{
  EntitiesCollectionIterator collIter;
  RulesCollectionIterator ruleIter;
  EntitiesIterator iter;
  for( ruleIter = rules_.begin(); ruleIter != rules_.end(); ruleIter++)
	   {
        for_each((*ruleIter)->begin(), (*ruleIter)->end(), mem_fun (&Rule::print) );
     }
  for( collIter = entities_.begin(); collIter != entities_.end(); collIter++)
	   {
      cout << "Collection  "<<    (*collIter)->printName()<<endl;
      for(iter = (*collIter)->begin(); iter !=(*collIter)->end(); iter++ )
        {
            if(*iter )
              cout << (*iter)->printName()<<endl;
        }
		}
}

void DataManipulator::processOrders(ProcessingMode * processingMode )
{
  vector <UnitEntity *>::iterator localIter;
  bool localChanges;
  int i;
  if(processingMode == 0)
    {
      cout << "Error: Unspecified processing mode" <<endl;
      return;
      }
// process orders for units per Location
      
   for ( i=0; i < locations.size();i++)
   {
    if(locations[i] ==0)
          continue;
    localChanges = true;
    while(localChanges)
    {        
      localChanges = false;
      for ( localIter = (locations[i])->unitsPresent.begin();
           localIter != (locations[i])->unitsPresent.end(); localIter++)
        {
          if(*localIter == 0)
              continue;
          if( (*localIter) ->process(processingMode))
            localChanges = true;            
        }
       if(!processingMode->isRepetitive())
            break;
#ifdef TEST_MODE
   if((testMode) &&(localChanges)) 	cout << " ==== One more round of processing " << endl;
#endif
     } 
  }
// For full day orders for all entities  process default order
  if(processingMode == &dayOrders)
    {
      EntitiesCollectionIterator collIter;
      EntitiesIterator iter;
      for( collIter = entities_.begin(); collIter != entities_.end(); collIter++)
	      {
          for(iter = (*collIter)->begin(); iter !=(*collIter)->end(); iter++ )
            {
                if(*iter )
                   (*iter)->defaultAction();
            }
		    }
    }

// process all Locations for Conflict Resolution for this Mode
}

STATUS DataManipulator::checkOrders()
{
//  EntitiesCollectionIterator collIter;
//  EntitiesIterator iter;
// detect faction
// check password
// detect unit
// check each line of orders
// produce report
return OK;
}


//void DataManipulator::saveOrders()
//{
//// save orders for all Entities
//}

void DataManipulator::prepareData()
{
  EntitiesCollectionIterator collIter;
  EntitiesIterator iter;
  STATUS status= OK;

  for( collIter = entities_.begin(); collIter != entities_.end(); collIter++)
	   {
      for(iter = (*collIter)->begin(); iter !=(*collIter)->end(); iter++ )
        {
     //(*iter) ->checkDataConsistency();	// Check for all important fields being initialized properly
     // Delete inconsistent data. (For instance Units with no Factions set)
            if(*iter )
              (*iter) ->loadOrders();
        }
		}

// Arrange orders in Units
  for( collIter = entities_.begin(); collIter != entities_.end(); collIter++)
	   {
      for(iter = (*collIter)->begin(); iter !=(*collIter)->end(); iter++ )
        {
            if(*iter )
            {
              if (ERROR == (*iter) ->prepareData())
			        status = ERROR;
             }
        }
		}

// Load press submissions
// Create press and pay rewards
// Weather and season changes
  if (status == ERROR)
	{
		cout << endl << "Inconsistent data. Turn processing stopped" << endl;
		exit(1);
	}
}
/** No descriptions */
void
DataManipulator::addOrderPrototypes(OrderPrototypesCollection *orderPrototypesCollection)
{
  orderPrototypesCollection ->   add(new MoveOrder);
  orderPrototypesCollection ->   add(new AcceptOrder);
  orderPrototypesCollection ->   add(new GiveOrder);
  orderPrototypesCollection ->   add(new StudyOrder);
  orderPrototypesCollection ->   add(new StackOrder);
  orderPrototypesCollection ->   add(new UnstackOrder);
  orderPrototypesCollection ->   add(new EquipOrder);
  orderPrototypesCollection ->   add(new WithdrawOrder);
  orderPrototypesCollection ->   add(new TestOrder);



}
/** Performs daily update of mana, time-lasting effects, item decay  */
void DataManipulator::dailyUpdate()
{
// cout <<"Manipulator Daily report"<<endl;
  EntitiesCollectionIterator collIter;
  EntitiesIterator iter;

  for( collIter = entities_.begin(); collIter != entities_.end(); collIter++)
	   {
      for(iter = (*collIter)->begin(); iter !=(*collIter)->end(); iter++ )
        {
            if(*iter )
              (*iter) ->dailyUpdate();
        }
		}
  for( collIter = entities_.begin(); collIter != entities_.end(); collIter++)
	   {
      for(iter = (*collIter)->begin(); iter !=(*collIter)->end(); iter++ )
        {
            if(*iter )
               (*iter) ->finalizeReports();
        }
		}
  for( collIter = entities_.begin(); collIter != entities_.end(); collIter++)
	   {
      for(iter = (*collIter)->begin(); iter !=(*collIter)->end(); iter++ )
        {
            if(*iter )
               (*iter) ->cleanReports();
        }
		}
}
/** Loading, initialization of data.
Consistency checks
Pre-calculations */
void DataManipulator::turnPreProcessing()
{
  EntitiesCollectionIterator collIter;
  EntitiesIterator iter;


 STATUS status= OK;

  if (load() == ERROR)
	{
		cout << endl << "Data loading failed. Turn processing stopped" << endl;
		exit(1);
	}

   if (initialize() == ERROR)
	{
		cout << endl << "Data initialization failed. Turn processing stopped" << endl;
		exit(1);
	}

  for( collIter = entities_.begin(); collIter != entities_.end(); collIter++)
	   {
      for(iter = (*collIter)->begin(); iter !=(*collIter)->end(); iter++ )
        {
            if(*iter )
           {
            if (ERROR == (*iter) ->dataConsistencyCheck())
			      status = ERROR;
           }
        }
		}
  if (status == ERROR)
	{
		cout << endl << "Inconsistent data. Turn processing stopped" << endl;
		exit(1);
	}

  for( collIter = entities_.begin(); collIter != entities_.end(); collIter++)
	   {
      for(iter = (*collIter)->begin(); iter !=(*collIter)->end(); iter++ )
        {
            if(*iter )
               (*iter) ->loadOrders();
        }
		}
  for( collIter = entities_.begin(); collIter != entities_.end(); collIter++)
	   {
      for(iter = (*collIter)->begin(); iter !=(*collIter)->end(); iter++ )
        {
            if(*iter )
               (*iter) ->prepareData();
        }
		}
}
