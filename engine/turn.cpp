/*************************************************************************
                  Turn.cpp  -  main object of turn generator
                             -------------------
 begin                : Fri Nov  9 19:24:42 IST 2001
 copyright            : (C) 2001 by Alex Dribin
 email                : alexliza@netvision.net.il
**************************************************************************/
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>

#include "Global.h"
#include "GameConfig.h"
#include "DataManipulator.h"
#include "BasicCombatManager.h"
#include "ReportPattern.h" //temp
#include "CombatReport.h"
#include "ReportPrinter.h"
// tests

extern int currentDay;
extern bool testMode;
extern ProcessingMode immediateOrders;
extern ProcessingMode stackOrders;
extern ProcessingMode dayOrders;
#ifndef VERSION
  #define VERSION "0.3.4"
#endif
string GameConfig::version = VERSION;
DataManipulator * dataManipulatorPtr = 0;
BasicCombatManager * combatManager = new BasicCombatManager();
void combatSimulator();


int main( int argc, char * argv[] )
{
  clock_t tick = 0;
  tick = clock();
  time_t start, end;
  double dif1 = 0, dif2 = 0, dif3 = 0;
  time( & start );

  cout << endl << endl
       << " ===================================================" << endl;
  cout << " ===================== Overlord " << VERSION
       << " ====================" << endl;
  cout << " ===================================================" << endl;


  if ( argc >= 2 )
  {
#ifdef TEST_MODE
    if ( !strcmp( argv[1], "test" ) )
    {
      cout << "Test Mode" << endl;
      testMode = true;
      if ( argc >= 3 )
      {
        gameConfig.init( argv[2] );
      }
      else
      {
        gameConfig.init( "overlord.config" );
      }
    }
    else
#endif
    {
      gameConfig.init( argv[1] );
    }
  }
  else
  {
    gameConfig.init( "overlord.config" );
  }

  cout << "Units data taken from " << * ( gameConfig.getUnitsFile() ) << endl;
  cout << "Factions data taken from " << * ( gameConfig.getFactionsFile() ) << endl;
  cout << "Locations data taken from " <<
       * ( gameConfig.getLocationsFile() ) << endl;
  //   cout<< "Orders taken from " <<gameConfig.getOrdersFileName()<<endl;
  DataManipulator dataManipulator;
  dataManipulatorPtr = & dataManipulator;

  dataManipulator.turnPreProcessing();

  if (  gameConfig.runMode ==  COMBAT_SIMULATOR )
	{
		combatSimulator();
		cout << endl<< "    === Combat simulation completed. ==="<<endl;
		return 0;
	}

  if ( ( gameConfig.runMode == MEMORY_TEST )
       || ( gameConfig.runMode == DAILY_MEMORY_TEST ) )
       {
         cout << "After loading orders \n"; getchar();
  }

  if ( gameConfig.runMode == TIME_TEST )
  {
    time( & end );
    dif1 = difftime( end, start );
    time( & start );
    cout << endl << "Reading: " << dif1 << " sec." << endl;
    cout << "......................................................................."
         << endl;
  }
  // Temporary

  ReportPattern::initialize( "report_patterns.txt" );
  //cout << "Number of free positions in ReportElement pool:
  // "<<countList(BasicReportElement::headOfFreeList)<<endl;
  cout << endl << endl << " ================= Turn processing ==============="
       << endl << endl;

  for ( currentDay = 1; currentDay <= gameConfig.daysInMonth; currentDay++ )
  {
    cout << endl << "    #### Processing day " << currentDay
         << " ####" << endl;
    cout << endl << "Daily data pre-Processing  " << endl << endl;
    dataManipulator.dailyPreProcessData();

    cout << endl << "Processing immediate orders " << endl << endl;
    dataManipulator.processOrders( & immediateOrders );

    cout << "Processing stack orders " << endl << endl;
    dataManipulator.processOrders( & stackOrders );

    cout << "Processing combat resoution  for all combats" << endl << endl;
    dataManipulator.processCombat();


    cout << "Processing full day orders " << endl << endl;
    dataManipulator.processOrders( & dayOrders );

    cout << "Processing competitive requests " << endl << endl;
    dataManipulator.processCompetitiveRequests( 0 );

    if ( gameConfig.runMode == DAILY_MEMORY_TEST )
    {
      cout << "After Processing orders on day " << currentDay << "\n";
      getchar();
    }

    dataManipulator.dailyUpdate(); // mana, time-lasting effects, item decay,  reports

    if ( gameConfig.runMode == DAILY_MEMORY_TEST )
    {
      cout << "After Processing reports on day " << currentDay << "\n";
      getchar();
    }
    //cout << "Number of free positions in ReportElement pool:
    // "<<countList(BasicReportElement::headOfFreeList)<<endl;
  }
  cout << endl << endl << " ============== End of Turn processing ============"
       << endl << endl;
  if ( gameConfig.runMode == TIME_TEST )
  {
    time( & end );
    dif2 = difftime( end, start );
    time( & start );
  }
#ifdef TEST_MODE
  //   if(testMode)  dataManipulator.printAllData();
#endif
  currentDay = gameConfig.daysInMonth; // currentDay is daysInMonth + 1 after finishing cycle
  dataManipulator.turnPostProcessing();
  dataManipulator.save();
  gameConfig.save();

  //... More operations
  cout << endl << endl
       << " ===================================================" << endl;
  cout << " ===================== The End ====================" << endl;


  if ( gameConfig.runMode == TIME_TEST )
  {
    time( & end );
    dif3 = difftime( end, start );
    cout << endl << "Reading: " << dif1 << " sec." << "Processing: " << dif2
         << " sec." << " Saving : " << dif3 << " sec." << " Total RunTime: "
         << ( dif1 + dif2 + dif3 ) << " sec." << endl;
  }
  cout << " ===================================================" << endl;



  cout << "The size of integer is " << sizeof( int ) << endl;
  cout << "The size of sampleGameData is " << sizeof( sampleGameData ) << endl;
  cout << "The size of sampleEntity is " << sizeof( sampleEntity ) << endl;
  cout << "The size of sampleTokenEntity is "
       << sizeof( sampleTokenEntity ) << endl;
  cout << "The size of sampleUnit is " << sizeof( sampleUnit ) << endl;
  cout << "The size of sampleFaction is " << sizeof( sampleFaction ) << endl;
  cout << "The size of sampleLocation is " << sizeof( sampleLocation ) << endl;


}


#include "TokenEntity.h"
#include "FileParser.h"
#include "CombatEngine.h"
#include <vector>
void combatSimulator()
{
  FileParser * parser = new FileParser ( "combat.dat" );
	if( parser->status != OK)
		{
		cout << "Can't find combat.dat" << endl;
		exit(1);
		}

	vector <TokenEntity *>  attackers;
	vector <TokenEntity *>  defenders;
	TokenEntity * currentToken;
	do
    {
      parser->getLine();
	 if (parser->matchKeyword ("ATTACKER") )
    		{
					currentToken = units[ parser->getWord()];
					if(currentToken)
      			attackers.push_back(currentToken);
      		continue;
    		}
	 if (parser->matchKeyword ("DEFENDER") )
    		{
					currentToken = units[ parser->getWord()];
					if(currentToken)
      			defenders.push_back(currentToken);
      		continue;
    		}

    } while (! parser ->eof() );

  ReportPrinter combatReportFile;
  combatReportFile.open("combat_simulation_report.txt");
	realCombatEngine.processBattle(attackers,defenders);
  realCombatEngine.getCombatReport()->print(combatReportFile);
  combatReportFile.close();
	delete parser;
}
