/***************************************************************************
                          DataManipulator.cpp

                             -------------------
    begin                : Wen May 22 13:52:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include <functional>
#include <algorithm>
#include "Global.h"
#include "DataManipulator.h"
#include "BasicCombatManager.h"
#include "WeatherRule.h"
#include "SeasonRule.h"
#include "DataStorageHandler.h"
#include "MovementVariety.h"
#include "BattleField.h"// temporary
#include "CombatTargetVariety.h"
#include "CombatManager.h"

DataManipulator::DataManipulator()
{

   addVarieties  (&movementModes);
   addVarieties  (&directions);
   addVarieties  (&stances);
   addVarieties  (&combatStances);
   addVarieties  (&combatMoves);
   addVarieties  (&combatRanks);
   addVarieties  (&combatFiles);
   addVarieties  (&equipments);
   addVarieties  (&construction_works);
   addVarieties  (&combatTargets);

   ruleIndex.addRules (&terrains);
   ruleIndex.addRules (&titles);
   ruleIndex.addRules (&skills);
   ruleIndex.addRules (&items);
   ruleIndex.addRules (&races);
   ruleIndex.addRules (&fx_actions);
   ruleIndex.addRules (&enchantments);
   ruleIndex.addRules (&constructions);
   ruleIndex.addRules (&weathers);
   ruleIndex.addRules (&seasons);


//... add more collections Order is important here: factions should be after units (disbanding)
   addEntities  (&units);
   addEntities  (&factions);
   addEntities  (&locations);
   addEntities  (&buildingsAndShips);
   addEntities  (&effects);

}



DataManipulator::~DataManipulator()
{
}




/*
 *
 */
void DataManipulator::addEntities ( BasicEntitiesCollection * collection)
{
		if(collection->getStatus()  == OK)
			{
 				entities_.push_back(collection);
			}
}



/*
 *
 */
void DataManipulator::addVarieties ( BasicVarietiesCollection * collection)
{
		if(collection->getStatus()  == OK)
			{
 				varieties_.push_back(collection);
			}
}





/*
 *
 */
STATUS DataManipulator::load()
{
  RulesCollectionIterator ruleIter;
  EntitiesCollectionIterator iter;
  VarietiesCollectionIterator varietyIter;
 STATUS status= OK;
 for (varietyIter = varieties_.begin(); varietyIter != varieties_.end(); varietyIter++)
   {
      if (IO_ERROR == (*varietyIter)->getStorageHandler()->load())
			status = IO_ERROR;
   }
 for (ruleIter = ruleIndex.getAllRules().begin(); ruleIter != ruleIndex.getAllRules().end(); ruleIter++)
   {
      if (IO_ERROR == (*ruleIter)->getStorageHandler()->load())
			status = IO_ERROR;
//      (*ruleIter)->getStorageHandler()->print();
   }
 for (iter = entities_.begin(); iter != entities_.end(); iter++)
   {
      if (IO_ERROR == (*iter)->getStorageHandler()->load())
			status = IO_ERROR;
 //     (*iter)->getStorageHandler()->print();
   }

// Define some global variables for special use

  cash = items["coin"];
    assert (cash);
  food = items["food"];
   assert(food);
  walkingMode = movementModes["walk"];
   assert(walkingMode);
  swimingMode = movementModes["swim"];
   assert(swimingMode);
  flyingMode = movementModes["fly"];
   assert(flyingMode);
  alliedStance =    stances["ally"];
   assert(alliedStance);
  friendlyStance =  stances["friend"];
   assert(friendlyStance);
  neutralStance =   stances["neutral"];
   assert(neutralStance);
  hostileStance =   stances["hostile"];
   assert(hostileStance);
  avoidStance = combatStances["avoid"];
   assert(avoidStance);
  defenceStance = combatStances["defend"];
   assert(defenceStance);
  attackStance = combatStances["attack"];
   assert(defenceStance);
  defaultCombatStance = avoidStance;
  defaultCombatMove = combatMoves["flee"];
  assert(defaultCombatMove);
  defaultCombatRank = combatRanks["rear"];
  assert(defaultCombatRank);
  defaultCombatFile = combatFiles["center"];
  assert(defaultCombatFile);

  strategySkill = skills["stra"];
  assert(strategySkill);
  tacticSkill = skills["tact"];
  assert(tacticSkill);
  combatSkill = skills["cmbt"];
  assert(combatSkill);
  ambushSkill = skills["ambu"];
  assert(ambushSkill);
 parrySkill= skills["parr"];
  assert(parrySkill);
 meleeSkill= skills["mele"];
  assert(meleeSkill);
 missileSkill= skills["arch"];
  assert(missileSkill);
  weaponSlot = equipments["equip_weapon"];
  assert(weaponSlot);

	frontCombatRank = combatRanks["front"];
  middleCombatRank = combatRanks["middle"];
  rearCombatRank = combatRanks["rear"];
  leftCombatFile = combatFiles["left"];
  centerCombatFile = combatFiles["center"];
  rightCombatFile = combatFiles["right"];


  battleTargetAll = combatTargets["BATTLEFIELD_TARGET_ALL"];
  battleTargetOwnSide = combatTargets["BATTLEFIELD_TARGET_OWN_SIDE"];
  battleTargetOppositeSide = combatTargets["BATTLEFIELD_TARGET_OPPOSITE_SIDE"];
  battleTargetFriend = combatTargets["BATTLEFIELD_TARGET_FRIENDLY"];
  battleTargetSelf = combatTargets["BATTLEFIELD_TARGET_SELF"];
  battleTargetFriendlyLeader = combatTargets["BATTLEFIELD_TARGET_FRIENDLY_LEADER"];
  battleTargetOpposingLeader = combatTargets["BATTLEFIELD_TARGET_OPPOSING_LEADER"];
  battleTargetOpposing = combatTargets["BATTLEFIELD_TARGET_OPPOSING"];




 return status;
}



/*
 *
 */
STATUS DataManipulator::save()
{
  EntitiesCollectionIterator iter;
  reportlist.open("reportlist.new");
 for (iter = entities_.begin(); iter != entities_.end(); iter++)
   {
      (*iter) ->getStorageHandler()->save();
   }
   reportlist.close();
 return OK;
}



/*
 *
 */
STATUS DataManipulator::initialize()
{
  RulesCollectionIterator ruleIter;
  EntitiesCollectionIterator iter;
  VarietiesCollectionIterator varietyIter;

  STATUS status= OK;
for (varietyIter = varieties_.begin(); varietyIter != varieties_.end(); varietyIter++)
   {
      if (IO_ERROR == (*varietyIter)->getStorageHandler()->initializeData())
			status = IO_ERROR;
   }

 for (ruleIter = ruleIndex.getAllRules().begin(); ruleIter != ruleIndex.getAllRules().end(); ruleIter++)
   {
      if (IO_ERROR == (*ruleIter) ->getStorageHandler()->initializeData())
			status = IO_ERROR;
   }

for (iter = entities_.begin(); iter != entities_.end(); iter++)
   {
      if (IO_ERROR == (*iter) ->getStorageHandler()->initializeData())
			status = IO_ERROR;
   }

//  combatManager->initialize();
 return status;
}



/*
 *
 */
void DataManipulator::printAllData()
{
  EntitiesCollectionIterator collIter;
  RulesCollectionIterator ruleIter;
  EntitiesIterator iter;
  for( ruleIter = ruleIndex.getAllRules().begin(); ruleIter != ruleIndex.getAllRules().end(); ruleIter++)
	   {
        for_each((*ruleIter)->begin(), (*ruleIter)->end(), mem_fun (&Rule::print) );
     }
  for( collIter = entities_.begin(); collIter != entities_.end(); collIter++)
	   {
      cout << "Collection  "<<    (*collIter)->print()<<endl;
      for(iter = (*collIter)->begin(); iter !=(*collIter)->end(); iter++ )
        {
            if(*iter )
              cout << (*iter)->print()<<endl;
        }
		}
}



/*
 *
 */
void DataManipulator::processOrders(ProcessingMode * processingMode )
{
  bool localChanges;
  long int i;
  if(processingMode == 0)
    {
      cout << "Error: Unspecified processing mode" <<endl;
      return;
      }
// process Faction orders

   for ( i=0; i < factions.size();i++)
   {
    if(factions[i] ==0)
          continue;

     (factions[i])->process(processingMode);
    }
// process orders for tokenEntities per Location

   for ( i=0; i < locations.size();i++)
   {
    if(locations[i] ==0)
          continue;
    localChanges = true;
    while(localChanges)
    {
      localChanges = false;
      for ( vector <UnitEntity *>::iterator localIter = (locations[i])->unitsPresent().begin();
           localIter != (locations[i])->unitsPresent().end(); localIter++)
        {

          if(*localIter == 0)
              continue;
	
          if((*localIter)->isHidden())
              continue;

	  if((*localIter)->isDisobeying())
              continue;

          if( (*localIter) ->process(processingMode))
            localChanges = true;
        }

      for ( vector <ConstructionEntity *>::iterator localIter = (locations[i])->constructionsPresent().begin();
           localIter != (locations[i])->constructionsPresent().end(); localIter++)
        {
          if(*localIter == 0)
              continue;

          if( (*localIter) ->process(processingMode))
            localChanges = true;

		(*localIter)->eraseAllRemovedUnits();
		(*localIter)->addAllAddedUnits();

        }
	// Only now deleted constructions and units may be erased
 	(locations[i])->eraseAllRemovedConstructions();
  	(locations[i])->eraseAllRemovedUnits();
	(locations[i])->addAllAddedUnits();
	(locations[i])->addAllAddedConstructions();
         if ((locations[i])->ordersToBeRepeated() )  // forced repetition regardless to mode and results
          {
            localChanges = true;
           }
         else
         {
            if(!processingMode->isRepetitive())
            break;
         }
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



/*
 *
 */
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



/** No descriptions */

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
              if (IO_ERROR == (*iter) ->prepareData())
			        status = IO_ERROR;
             }
        }
		}

// Load press submissions
// Create press and pay rewards
// Weather and season changes
  if (status == IO_ERROR)
	{
		cout << endl << "Inconsistent data. Turn processing stopped" << endl;
		exit(1);
	}
}






/*
 * Performs daily update of mana, time-lasting effects, item decay
 */
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
               (*iter) ->cleanPublicReports();
        }
		}
}



/*
 * Loading, initialization of data.
 * Consistency checks
 * Pre-calculations
 */
void DataManipulator::turnPreProcessing()
{
  EntitiesCollectionIterator collIter;
  EntitiesIterator iter;

//  RulesCollectionIterator RuleCollIter;
//  RulesIterator ruleIter;


 STATUS status= OK;

  if (load() == IO_ERROR)
	{
		cout << endl << "Data loading failed. Turn processing stopped" << endl;
		exit(1);
	}

   if (initialize() == IO_ERROR)
	{
		cout << endl << "Data initialization failed. Turn processing stopped" << endl;
		exit(1);
	}

//  for( RuleCollIter = rules_.begin(); RuleCollIter != rules_.end(); ++RuleCollIter)
//	   {
//      for(ruleIter = (*RuleCollIter)->begin(); ruleIter !=(*RuleCollIter)->end(); ++ruleIter )
//        {
//            if(*ruleIter )
//           {
//            if (IO_ERROR == (*ruleIter) ->dataConsistencyCheck())
//			      status = IO_ERROR;
//           }
//        }
//		}

  for( collIter = entities_.begin(); collIter != entities_.end(); collIter++)
	   {
      for(iter = (*collIter)->begin(); iter !=(*collIter)->end(); iter++ )
        {
            if(*iter )
           {
            if (IO_ERROR == (*iter) ->dataConsistencyCheck())
			      status = IO_ERROR;
           }
        }
		}
  if (status == IO_ERROR)
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



void DataManipulator::processCompetitiveRequests(ProcessingMode * )
{
  long int i;
   for ( i=0; i < locations.size();i++)
   {
    if(locations[i] ==0)
          continue;
    locations[i]->processMarket();
    locations[i]->processDailyConflict();
    if(currentDay ==gameConfig.daysInMonth )
          locations[i]->processMonthlyConflict();
  }
}



void DataManipulator::processCombat()
{
  long int i;
   for ( i=0; i < locations.size();i++)
   {
    if(locations[i] ==0)
          continue;
    locations[i]->getCombatManager()->process();
  }
}



void DataManipulator::dailyPreProcessData()
{
  EntitiesCollectionIterator collIter;
  EntitiesIterator iter;
  for( collIter = entities_.begin(); collIter != entities_.end(); collIter++)
	   {
      for(iter = (*collIter)->begin(); iter !=(*collIter)->end(); iter++ )
        {
            if(*iter )
               (*iter) ->dailyPreProcess();
        }
		}

}



void DataManipulator::turnPostProcessing()
{
  EntitiesCollectionIterator collIter;
  EntitiesIterator iter;
  for( collIter = entities_.begin(); collIter != entities_.end(); collIter++)
	   {
      for(iter = (*collIter)->begin(); iter !=(*collIter)->end(); iter++ )
        {
            if(*iter )
               (*iter) ->postProcessData();
        }
		}
/*  for( collIter = entities_.begin(); collIter != entities_.end(); collIter++)
	   {
      for(iter = (*collIter)->begin(); iter !=(*collIter)->end(); iter++ )
        {
            if(*iter )
               (*iter) ->postPostProcessData();
        }
		}*/
// add Reports events from post-processing stage
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
               (*iter) ->cleanPublicReports();
        }
		}
}



void DataManipulator::clear()
{
  RulesCollectionIterator ruleIter;
  EntitiesCollectionIterator iter;
  VarietiesCollectionIterator varietyIter;


for (varietyIter = varieties_.begin(); varietyIter != varieties_.end(); varietyIter++)
   {
      (*varietyIter)->clear();
   }

 for (ruleIter = ruleIndex.getAllRules().begin(); ruleIter != ruleIndex.getAllRules().end(); ruleIter++)
   {
      (*ruleIter) ->clear();
   }

for (iter = entities_.begin(); iter != entities_.end(); iter++)
   {
      (*iter) ->clear();
   }
}



GameData * DataManipulator::findGameData(const string & tag)
{
  EntitiesCollectionIterator iter;
  RulesCollectionIterator ruleIter;
  GameData * data;
  for (iter = entities_.begin(); iter != entities_.end(); iter++)
   {
      data = (*iter) ->findByTag(tag,false);
      if (data)
        return data;

   }
 for (ruleIter = ruleIndex.getAllRules().begin(); ruleIter != ruleIndex.getAllRules().end(); ruleIter++)
   {
      data = (*ruleIter) ->findByTag(tag,false);
      if (data)
        return data;
   }
  return 0;
}


// Looks for placeholder "tag"
// If such placeholder doesn't exist yet - creates it
NewEntityPlaceholder * DataManipulator::findOrAddPlaceholder(const string & tag)
{
  EntitiesCollectionIterator iter;
  NewEntityPlaceholder * data;
  for (iter = entities_.begin(); iter != entities_.end(); iter++)
   {
      data = (*iter) ->findOrAddPlaceholder(tag);
      if (data)
        return data;

   }
  return 0;
}
