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
#include "EffectRule.h"
#include "SeasonRule.h"
#include "DataStorageHandler.h"
#include "MovementVariety.h"
#include "BattleField.h"// temporary
#include "CombatTargetVariety.h"
#include "CombatManager.h"
#include "TurnReport.h"
#include "GameFacade.h"

//extern GameFacade * gameFacade;
DataManipulator::DataManipulator()
{

}

/*
 * This function makes initialization of DataManipulator.
 * List sets of data to be managed
 */

void DataManipulator::init()
{

   addVarieties  (&(gameFacade->movementModes));
   addVarieties  (&(gameFacade->directions));
   addVarieties  (&(gameFacade->stances));
   addVarieties  (&(gameFacade->combatStances));
   addVarieties  (&(gameFacade->combatMoves));
   addVarieties  (&(gameFacade->combatRanks));
   addVarieties  (&(gameFacade->combatFiles));
   addVarieties  (&(gameFacade->equipments));
   addVarieties  (&(gameFacade->construction_works));
   addVarieties  (&(gameFacade->combatTargets));

   gameFacade->ruleIndex.addRules (&(gameFacade->terrains));
   gameFacade->ruleIndex.addRules (&(gameFacade->titles));
   gameFacade->ruleIndex.addRules (&(gameFacade->skills));
   gameFacade->ruleIndex.addRules (&(gameFacade->items));
   gameFacade->ruleIndex.addRules (&(gameFacade->races));
   gameFacade->ruleIndex.addRules (&(gameFacade->effectRules));
   gameFacade->ruleIndex.addRules (&(gameFacade->eventRules));
   gameFacade->ruleIndex.addRules (&(gameFacade->fx_actions));
   gameFacade->ruleIndex.addRules (&(gameFacade->enchantments));
   gameFacade->ruleIndex.addRules (&(gameFacade->constructions));
   gameFacade->ruleIndex.addRules (&(gameFacade->weathers));
   gameFacade->ruleIndex.addRules (&(gameFacade->seasons));


//... add more collections Order is important here: factions should be after units (disbanding)
   addEntities  (&(gameFacade->units));
   addEntities  (&(gameFacade->factions));
   addEntities  (&(gameFacade->locations));
   addEntities  (&(gameFacade->buildingsAndShips));
   addEntities  (&(gameFacade->effects));

  if( prototypeManager == 0)
  {
	  cout << "Prototype Manager created on "<<"DataManipulator constructor"<< " call"<<endl;
    prototypeManager = new PrototypeManager;
  }

   prototypeManager->addToRegistry(new IntegerData(0));
   prototypeManager->addToRegistry(new StringData(""));
   prototypeManager->addToRegistry(new RationalNumber());

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
     if((*varietyIter)->getStorageHandler() ==0)
     {
         cerr<<"ERROR: varietyCollection with undefined storage handler"<<endl;
         continue;
     }
      if (IO_ERROR == (*varietyIter)->getStorageHandler()->load())
			status = IO_ERROR;
   }
 for (ruleIter = gameFacade->ruleIndex.getAllRules().begin(); ruleIter != gameFacade->ruleIndex.getAllRules().end(); ruleIter++)
   {
     if((*ruleIter)->getStorageHandler() ==0)
     {
         cerr<<"ERROR: ruleCollection with undefined storage handler"<<endl;
         continue;
     }
      if (IO_ERROR == (*ruleIter)->getStorageHandler()->load())
			status = IO_ERROR;
//      (*ruleIter)->getStorageHandler()->print();
   }
 for (iter = entities_.begin(); iter != entities_.end(); iter++)
   {
     if((*iter)->getStorageHandler() ==0)
     {
         cerr<<"ERROR: entitiesCollection with undefined storage handler"<<endl;
         continue;
     }
      if (IO_ERROR == (*iter)->getStorageHandler()->load())
			status = IO_ERROR;
 //     (*iter)->getStorageHandler()->print();
   }

// Events are not loaded from file


// Define some global variables for special use

  cash = gameFacade->items["coin"];
    assert (cash);
  food = gameFacade->items["food"];
   assert(food);
  walkingMode = gameFacade->movementModes["walk"];
   assert(walkingMode);
  swimingMode = gameFacade->movementModes["swim"];
   assert(swimingMode);
  flyingMode = gameFacade->movementModes["fly"];
   assert(flyingMode);
  privateStance =    gameFacade->stances["private"];
   assert(privateStance);
   alliedStance =    gameFacade->stances["ally"];
    assert(alliedStance);
  friendlyStance =  gameFacade->stances["friend"];
   assert(friendlyStance);
  neutralStance =   gameFacade->stances["neutral"];
   assert(neutralStance);
  unknownStance =   gameFacade->stances["unknown"];
   assert(unknownStance);
  hostileStance =   gameFacade->stances["hostile"];
   assert(hostileStance);
  enemyStance =   gameFacade->stances["enemy"];
   assert(enemyStance);
  avoidStance = gameFacade->combatStances["avoid"];
   assert(avoidStance);
  defenceStance = gameFacade->combatStances["defend"];
   assert(defenceStance);
  attackStance = gameFacade->combatStances["attack"];
   assert(defenceStance);
  defaultCombatStance = avoidStance;
  defaultCombatMove = gameFacade->combatMoves["flee"];
  assert(defaultCombatMove);
  defaultCombatRank = gameFacade->combatRanks["rear"];
  assert(defaultCombatRank);
  defaultCombatFile = gameFacade->combatFiles["center"];
  assert(defaultCombatFile);

  strategySkill = gameFacade->skills["stra"];
  assert(strategySkill);
  tacticSkill = gameFacade->skills["tact"];
  assert(tacticSkill);
  combatSkill = gameFacade->skills["cmbt"];
  assert(combatSkill);
  ambushSkill = gameFacade->skills["ambu"];
  assert(ambushSkill);
 parrySkill= gameFacade->skills["parr"];
  assert(parrySkill);
 meleeSkill= gameFacade->skills["mele"];
  assert(meleeSkill);
 missileSkill= gameFacade->skills["arch"];
  assert(missileSkill);

  weaponSlot = gameFacade->equipments["equip_weapon"];
  assert(weaponSlot);

  frontCombatRank = gameFacade->combatRanks["front"];
  middleCombatRank = gameFacade->combatRanks["middle"];
  rearCombatRank = gameFacade->combatRanks["rear"];
  leftCombatFile = gameFacade->combatFiles["left"];
  centerCombatFile = gameFacade->combatFiles["center"];
  rightCombatFile = gameFacade->combatFiles["right"];


  battleTargetAll = gameFacade->combatTargets["BATTLEFIELD_TARGET_ALL"];
  battleTargetOwnSide = gameFacade->combatTargets["BATTLEFIELD_TARGET_OWN_SIDE"];
  battleTargetOppositeSide = gameFacade->combatTargets["BATTLEFIELD_TARGET_OPPOSITE_SIDE"];
  battleTargetFriend = gameFacade->combatTargets["BATTLEFIELD_TARGET_FRIENDLY"];
  battleTargetSelf = gameFacade->combatTargets["BATTLEFIELD_TARGET_SELF"];
  battleTargetFriendlyLeader = gameFacade->combatTargets["BATTLEFIELD_TARGET_FRIENDLY_LEADER"];
  battleTargetOpposingLeader = gameFacade->combatTargets["BATTLEFIELD_TARGET_OPPOSING_LEADER"];
  battleTargetOpposing = gameFacade->combatTargets["BATTLEFIELD_TARGET_OPPOSING"];




 return status;
}

/*
 *  This is for use in client
 */
void DataManipulator::check()
{
  RulesCollectionIterator ruleIter;
  EntitiesCollectionIterator iter;
 for (ruleIter = gameFacade->ruleIndex.getAllRules().begin(); ruleIter != gameFacade->ruleIndex.getAllRules().end(); ruleIter++)
   {
     if((*ruleIter)->getStorageHandler() ==0)
     {
         cerr<<"ERROR: checking ruleCollection with undefined storage handler"<<endl;
         continue;
     }
     createFileIfNotExist((*ruleIter)->getStorageHandler()->getInputFileName(), (*ruleIter)->getSampleObject());
   }
 for (iter = entities_.begin(); iter != entities_.end(); iter++)
   {
     if((*iter)->getStorageHandler() ==0)
     {
         cerr<<"ERROR: checking entitiesCollection with undefined storage handler"<<endl;
         continue;
     }
     createFileIfNotExist((*iter)->getStorageHandler()->getInputFileName(), (*iter)->getSampleObject());
   }
}



void DataManipulator::createFileIfNotExist(string  * fileName, GameData * sample)
{
     if(!isFileExist(*fileName)) // If not exist - create void file
        {
            string keyword;
            time_t rawtime;
            if (sample)
            {
                keyword = sample->getKeyword();
            }
            else
            {
                keyword = makeRandomName();
            }
            cout<<"New data file "<<*fileName<<" created with keyword "<<keyword<<endl;
            ofstream outfile(fileName->c_str());
            outfile << "# Overlord  data " << endl;
            time(&rawtime);
            outfile << "# Ver " << GameConfig::version << " " << ctime(&rawtime) << endl;
            outfile << "KEYWORD " << keyword << " " << 0 << endl;
            outfile.close();
     }
}
/*
 *
 */
STATUS DataManipulator::save(const char * suffix)
{
  EntitiesCollectionIterator iter;
  reportlist.open("reportlist.new");
 for (iter = entities_.begin(); iter != entities_.end(); iter++)
   {
     if((*iter)->getStorageHandler() ==0)
     {
         cerr<<"ERROR: entitiesCollection with undefined storage handler"<<endl;
         continue;
     }
      (*iter) ->getStorageHandler()->save(suffix);
   }

 for (RulesCollectionIterator ruleIter = gameFacade->ruleIndex.getAllRules().begin();
         ruleIter != gameFacade->ruleIndex.getAllRules().end(); ruleIter++)
   {
     if((*ruleIter)->getStorageHandler() ==0)
     {
         cerr<<"ERROR: ruleCollection with undefined storage handler"<<endl;
         continue;
     }
      (*ruleIter)->getStorageHandler()->save(suffix);

   }
 gameFacade->events.getStorageHandler()->save(suffix);
   reportlist.close();
 return OK;
}

void DataManipulator::makeReports()
{
	   for (int i=0; i < gameFacade->factions.size();i++)
	   {
	    if(gameFacade->factions[i] ==0)
	          continue;

	     (gameFacade->factions[i])->saveReport(); // Text report
	     //(factions[i])->saveCReport(); //
	     (gameFacade->factions[i])->getTurnReport()->save();// Digital Report
             // Testing
             TurnReport * turnReport = new TurnReport();
             turnReport->init(gameFacade->factions[i]);// Read Digital Report phase 1
             turnReport->load();           // Read Digital Report phase 2
             turnReport->initialize();     // Read Digital Report phase 3
             turnReport->setReportName(string("NewTurnReport") +(gameFacade->factions[i])->getTag() );
             turnReport->save();          // Write  Digital Report
	    }

}


/*
 * This function makes data initialisation
 */
STATUS DataManipulator::initialize()
{
  RulesCollectionIterator ruleIter;
  EntitiesCollectionIterator iter;
  VarietiesCollectionIterator varietyIter;

  STATUS status= OK;
for (varietyIter = varieties_.begin(); varietyIter != varieties_.end(); varietyIter++)
   {
     if((*varietyIter)->getStorageHandler() ==0)
     {
         cerr<<"ERROR: varietyCollection with undefined storage handler"<<endl;
         continue;
     }
      if (IO_ERROR == (*varietyIter)->getStorageHandler()->initializeData())
			status = IO_ERROR;
   }

 for (ruleIter = gameFacade->ruleIndex.getAllRules().begin(); ruleIter != gameFacade->ruleIndex.getAllRules().end(); ruleIter++)
   {
      if((*ruleIter)->getStorageHandler() ==0)
     {
         cerr<<"ERROR: ruleCollection with undefined storage handler"<<endl;
         continue;
     }
     if (IO_ERROR == (*ruleIter) ->getStorageHandler()->initializeData())
			status = IO_ERROR;
   }

for (iter = entities_.begin(); iter != entities_.end(); iter++)
   {
     if((*iter)->getStorageHandler() ==0)
     {
         cerr<<"ERROR: entitiesCollection with undefined storage handler"<<endl;
         continue;
     }
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
  for( ruleIter = gameFacade->ruleIndex.getAllRules().begin(); ruleIter != gameFacade->ruleIndex.getAllRules().end(); ruleIter++)
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

   for ( i=0; i < gameFacade->factions.size();i++)
   {
    if(gameFacade->factions[i] ==0)
          continue;

     (gameFacade->factions[i])->process(processingMode);
    }
// process orders for tokenEntities per Location

   for ( i=0; i < gameFacade->locations.size();i++)
   {
    if(gameFacade->locations[i] ==0)
          continue;
    localChanges = true;
    while(localChanges)
    {
      localChanges = false;
      for ( vector <UnitEntity *>::iterator localIter = (gameFacade->locations[i])->unitsPresent().begin();
           localIter != (gameFacade->locations[i])->unitsPresent().end(); localIter++)
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

      for ( vector <ConstructionEntity *>::iterator localIter = (gameFacade->locations[i])->constructionsPresent().begin();
           localIter != (gameFacade->locations[i])->constructionsPresent().end(); localIter++)
        {
          if(*localIter == 0)
              continue;

          if( (*localIter) ->process(processingMode))
            localChanges = true;

		(*localIter)->eraseAllRemovedUnits();
                (*localIter)->addAllAddedUnits();

        }
	// Only now deleted constructions and units may be erased
 	(gameFacade->locations[i])->eraseAllRemovedConstructions();
  	(gameFacade->locations[i])->eraseAllRemovedUnits();
	(gameFacade->locations[i])->addAllAddedUnits();
	(gameFacade->locations[i])->addAllAddedConstructions();
         if ((gameFacade->locations[i])->ordersToBeRepeated() )  // forced repetition regardless to mode and results
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

    for (collIter = entities_.begin(); collIter != entities_.end(); collIter++)
    {
        for (iter = (*collIter)->begin(); iter != (*collIter)->end(); iter++)
        {
            if (*iter)
            {

                (*iter) ->dailyUpdate();
            }
        }

    }
    for (collIter = entities_.begin(); collIter != entities_.end(); collIter++)
    {
        for (iter = (*collIter)->begin(); iter != (*collIter)->end(); iter++)
        {
            if (*iter)
            {
                (*iter) ->finalizeReports();
                (*iter) ->finalizeEvents();
            }
        }
    }
    for (collIter = entities_.begin(); collIter != entities_.end(); collIter++)
    {
        for (iter = (*collIter)->begin(); iter != (*collIter)->end(); iter++)
        {
            if (*iter)
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

  for( auto RuleCollIter = gameFacade->ruleIndex.getAllRules().begin(); RuleCollIter != gameFacade->ruleIndex.getAllRules().end(); ++RuleCollIter)
	   {
      for(auto ruleIter = (*RuleCollIter)->begin(); ruleIter !=(*RuleCollIter)->end(); ++ruleIter )
        {
            if(*ruleIter )
           {
            if (IO_ERROR == (*ruleIter) ->dataConsistencyCheck())
			      status = IO_ERROR;
           }
        }
		}

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
   for ( i=0; i < gameFacade->locations.size();i++)
   {
    if(gameFacade->locations[i] ==0)
          continue;
    gameFacade->locations[i]->processMarket();
    gameFacade->locations[i]->processDailyConflict();
    if(gameFacade->getCurrentDay() ==gameFacade->getGameConfig()->daysInMonth )
          gameFacade->locations[i]->processMonthlyConflict();
  }

}



void DataManipulator::processCombat()
{
  long int i;
   for ( i=0; i < gameFacade->locations.size();i++)
   {
    if(gameFacade->locations[i] ==0)
          continue;
    gameFacade->locations[i]->getCombatManager()->process();
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
  // Post-turn processing
  for( collIter = entities_.begin(); collIter != entities_.end(); collIter++)
	   {
      for(iter = (*collIter)->begin(); iter !=(*collIter)->end(); iter++ )
        {
            if(*iter )
               (*iter) ->postProcessData();
        }
		}
  // Processing of results of Post-turn processing. Currently not used
  for( collIter = entities_.begin(); collIter != entities_.end(); collIter++)
	   {
      for(iter = (*collIter)->begin(); iter !=(*collIter)->end(); iter++ )
        {
            if(*iter )
               (*iter) ->postPostProcessData();
        }
		}
// add Reports and events from post-processing stage
  for( collIter = entities_.begin(); collIter != entities_.end(); collIter++)
	   {
      for(iter = (*collIter)->begin(); iter !=(*collIter)->end(); iter++ )
        {
            if(*iter )
            {
               (*iter) ->finalizeReports();
               (*iter) ->finalizeTurnEvents();
            }
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

 for (ruleIter = gameFacade->ruleIndex.getAllRules().begin(); ruleIter != gameFacade->ruleIndex.getAllRules().end(); ruleIter++)
   {
      (*ruleIter) ->clear();
   }

for (iter = entities_.begin(); iter != entities_.end(); iter++)
   {
      (*iter) ->clear();
   }
}



Rule* DataManipulator::findRule(const string & tag)
{
  RulesCollectionIterator ruleIter;
  GameData * data;
 for (ruleIter = gameFacade->ruleIndex.getAllRules().begin(); ruleIter != gameFacade->ruleIndex.getAllRules().end(); ruleIter++)
   {
      data = (*ruleIter) ->findByTag(tag,false);
      if (data)
        return dynamic_cast<Rule *>(data);
   }
  return 0;
}


Rule* DataManipulator::findRuleByName(const string & name)
{
  RulesCollectionIterator ruleIter;
  GameData * data;
 for (ruleIter = gameFacade->ruleIndex.getAllRules().begin(); ruleIter != gameFacade->ruleIndex.getAllRules().end(); ruleIter++)
   {
      data = (*ruleIter) ->findByName(name,false);
      if (data)
        return dynamic_cast<Rule *>(data);
   }
  return 0;
}



GameData * DataManipulator::findGameData(const string & tag)
{
  EntitiesCollectionIterator iter; 
  GameData * data;
  for (iter = entities_.begin(); iter != entities_.end(); iter++)
   {
      data = (*iter) ->findByTag(tag,false);
      if (data)
        return data;

   }
  return findRule(tag);
}
GameData * DataManipulator::findGameDataByName(const string & name)
{
  EntitiesCollectionIterator iter; 
  GameData * data;
  for (iter = entities_.begin(); iter != entities_.end(); iter++)
   {
      data = (*iter) ->findByName(name,false);
      if (data)
        return data;

   }
  return findRuleByName(name);
}


string DataManipulator::getCollectionKeyword(const string & tag)
{
    string keyword =getEntityCollectionKeyword(tag);
    if(keyword != "")
    {
        return keyword;
    }
    else
    {
        return getRuleCollectionKeyword(tag);
    }
}
string DataManipulator::getEntityCollectionKeyword(const string & tag)
{
  EntitiesCollectionIterator iter;
  GameData * data;
  for (iter = entities_.begin(); iter != entities_.end(); iter++)
   {
      data = (*iter) ->findByTag(tag,false);
      if (data)
        return (*iter)->getCollectionKeyword();
   }
  return "";
}

string DataManipulator::getRuleCollectionKeyword(const string & tag)
{
  RulesCollectionIterator ruleIter;
  GameData * data;

 for (ruleIter = gameFacade->ruleIndex.getAllRules().begin(); ruleIter != gameFacade->ruleIndex.getAllRules().end(); ++ruleIter)
   {
      data = (*ruleIter) ->findByTag(tag,false);
      if (data)
        return (*ruleIter)->getCollectionKeyword();
   }
  return "";
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

BasicRulesCollection  * DataManipulator::findRuleCollection(const string & keyword)
{
    RulesCollectionIterator ruleIter;
 for (ruleIter = gameFacade->ruleIndex.getAllRules().begin(); ruleIter != gameFacade->ruleIndex.getAllRules().end(); ++ruleIter)
   {
    if(!ciStringCompare((*ruleIter)->getCollectionKeyword(),keyword))
    {
        return (*ruleIter);
    }
   }
    return 0;

}


void DataManipulator::prepareInitialReport()
{
    // Generate object images
    // Add initial locations as visited?
    // Faction::dailyImageProcessing()
	   for (int i=0; i < gameFacade->factions.size();i++)
	   {
	    if(gameFacade->factions[i] ==0)
	          continue;

	     (gameFacade->factions[i])->dailyImageProcessing();
           }
    // mark some rules as initially known and report them here
}

Entity* DataManipulator::findEntity(const string & tag)
  {
      GameData * data = findGameData(tag);
      if(data)
          return dynamic_cast<Entity *>(data);
      return 0;
  }
