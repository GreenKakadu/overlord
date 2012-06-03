/*************************************************************************
                  Turn.cpp  -  main object of turn generator
                             -------------------
 begin                : Fri Nov  9 19:24:42 IST 2001
 copyright            : (C) 2001 by Alex Dribin
 email                : Alex.Dribin@gmail.com
**************************************************************************/
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include "Global.h"
#include "GameConfig.h"
#include "DataManipulator.h"
#include "BasicCombatManager.h"
#include "ReportPattern.h" 
#include "CombatReport.h"
#include "GameFacade.h"


extern bool testMode;

#ifndef VERSION
  #define VERSION "1.1"
#endif
string GameConfig::version = VERSION;
GameFacade * gameFacade = 0;
BasicCombatManager * combatManager = new BasicCombatManager();
void combatSimulator();


int main( int argc, char * argv[] )
{
  string configFileName =  "overlord.config"; // Default name
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
          configFileName = argv[2];

      }
    }
    else
#endif
    {
          configFileName = argv[1];

    }
  }


  gameFacade = new GameFacade();
  if(!gameFacade->loadConfiguration(configFileName))
  {
      notifyAndExit(configFileName);
  }

  gameFacade->prepareGame();


  if (  gameFacade->getGameConfig()->runMode ==  COMBAT_SIMULATOR )
	{
          cout << endl<< "    === Running combat simulation mode. ==="<<endl<<endl;
          combatSimulator();
		cout << endl<< "    === Combat simulation completed. ==="<<endl;
		return 0;
	}

 if (  gameFacade->getGameConfig()->runMode ==   LOCATION_INITIALIZATION)
	{
          cout << endl<< "    === Running Data initialization mode. ==="<<endl<<endl;
          gameFacade->getDataManipulator()->save();
			cout << endl<< "    === Data initialization completed. ==="<<endl;
		return 0;
	}
  if ( ( gameFacade->getGameConfig()->runMode == MEMORY_TEST )
       || ( gameFacade->getGameConfig()->runMode == DAILY_MEMORY_TEST ) )
       {
         cout << endl<< "    === Running memory test mode. ==="<<endl<<endl;
         cout << "After loading orders \n"; getchar();
  }

  if ( gameFacade->getGameConfig()->runMode == TIME_TEST )
  {
    cout << endl<< "    === Running time test mode. ==="<<endl<<endl;
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

  if ( gameFacade->getGameConfig()->runMode != STARTING_TURN )
  {
	for (gameFacade->setCurrentDay(1);
                gameFacade->getCurrentDay() <= gameFacade->getGameConfig()->daysInMonth;
                gameFacade->setCurrentDay(gameFacade->getCurrentDay() +1))
	{
	cout << endl << "    #### Processing day " << gameFacade->getCurrentDay()
		<< " ####" << endl;
	cout << endl << "Daily data pre-Processing  "  << endl;
	gameFacade->getDataManipulator()->dailyPreProcessData();

	cout  << "Processing immediate orders "  << endl;
	gameFacade->getDataManipulator()->processOrders( & immediateOrders );

        cout  << "Processing request orders "  << endl;
	gameFacade->getDataManipulator()->processOrders( & requestOrders );

	cout << "Processing stack orders "  << endl;
	gameFacade->getDataManipulator()->processOrders( & stackOrders );

	cout << "Processing combat resoution  for all combats"  << endl;
	gameFacade->getDataManipulator()->processCombat();


	cout << "Processing full day orders "  << endl;
	gameFacade->getDataManipulator()->processOrders( & dayOrders );

	cout << "Processing competitive requests "  << endl;
	gameFacade->getDataManipulator()->processCompetitiveRequests( 0 );

	if ( gameFacade->getGameConfig()->runMode == DAILY_MEMORY_TEST )
	{
	cout << "After Processing orders on day " << gameFacade->getCurrentDay() << "\n";
	getchar();
	}

	gameFacade->getDataManipulator()->dailyUpdate(); // mana, time-lasting effects, item decay,  reports

	if ( gameFacade->getGameConfig()->runMode == DAILY_MEMORY_TEST )
	{
	cout << "After Processing reports on day " << gameFacade->getCurrentDay() << "\n";
	getchar();
	}
	//cout << "Number of free positions in ReportElement pool:
	// "<<countList(BasicReportElement::headOfFreeList)<<endl;
	}
	cout << endl << endl << " ============== End of Turn processing ============"
	<< endl << endl;
  }
  else
  {
      // generate missing data for first turn report
      gameFacade->getDataManipulator()->prepareInitialReport();
  }

  if ( gameFacade->getGameConfig()->runMode == TIME_TEST )
  {
    time( & end );
    dif2 = difftime( end, start );
    time( & start );
  }
#ifdef TEST_MODE
  //   if(testMode)  dataManipulator.printAllData();
#endif



  gameFacade->setCurrentDay(gameFacade->getGameConfig()->daysInMonth); // currentDay is daysInMonth + 1 after finishing cycle
  gameFacade->getDataManipulator()->turnPostProcessing();
  gameFacade->getDataManipulator()->save();
  gameFacade->getDataManipulator()->makeReports();
  gameFacade->getGameConfig()->save();

  //... More operations
  cout << endl << endl
       << " ===================================================" << endl;
  cout << " ===================== The End ====================" << endl;


  if ( gameFacade->getGameConfig()->runMode == TIME_TEST )
  {
    time( & end );
    dif3 = difftime( end, start );
    cout << endl << "Reading: " << dif1 << " sec." << "Processing: " << dif2
         << " sec." << " Saving : " << dif3 << " sec." << " Total RunTime: "
         << ( dif1 + dif2 + dif3 ) << " sec." << endl;
  }
  cout << " ===================================================" << endl;



  #ifdef TEST_MODE
  cout << "The size of integer is " << sizeof( int ) << endl;
  cout << "The size of sampleGameData is " << sizeof( sampleGameData ) << endl;
  cout << "The size of sampleEntity is " << sizeof( sampleEntity ) << endl;
  cout << "The size of sampleTokenEntity is "
       << sizeof( sampleTokenEntity ) << endl;
  cout << "The size of sampleUnit is " << sizeof( sampleUnit ) << endl;
  cout << "The size of sampleFaction is " << sizeof( sampleFaction ) << endl;
  cout << "The size of sampleLocation is " << sizeof( sampleLocation ) << endl;
#endif

}


#include "TokenEntity.h"
#include "FileParser.h"
#include "CombatEngine.h"
#include <vector>
void combatSimulator()
{
  ReportPattern::initialize( "report_patterns.txt" );
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
					currentToken = gameFacade->units[ parser->getWord()];
					if(currentToken)
      			attackers.push_back(currentToken);
      		continue;
    		}
	 if (parser->matchKeyword ("DEFENDER") )
    		{
					currentToken = gameFacade->units[ parser->getWord()];
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
