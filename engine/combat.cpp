/*************************************************************************
                  Combat.cpp
				  main object of combat simulator
                             -------------------
 begin                : Dec  20  2004
 copyright            : (C) 2004 by Alex Dribin
 email                : Alex.Dribin@gmail.com
**************************************************************************/
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "Global.h"
#include "GameConfig.h"
#include "DataManipulator.h"
#include "BasicCombatManager.h"
#include "ReportPattern.h" //temp
// tests

extern int currentDay;


#ifndef VERSION
  #define VERSION "0.3.4"
#endif
string GameConfig::version = VERSION;
DataManipulator * dataManipulatorPtr = 0;
BasicCombatManager * combatManager = new BasicCombatManager();



int main( int argc, char * argv[] )
{
  clock_t tick = 0;
  tick = clock();
  time_t start, end;
  double dif1, dif2 = 0, dif3 = 0;
  time( & start );

  cout << endl << endl
       << " ===================================================" << endl;
  cout << " ============== Overlord Combat Simulator" << VERSION
       << " ====================" << endl;
  cout << " ===================================================" << endl;


  if ( argc >= 2 )
  {
    {
			// Read input from "combat.dat"
			gameConfig.init( argv[1] );
    }
  }
  else
  {
    gameConfig.init( "combat.config" );
  }

  cout << "Units data taken from " << * ( gameConfig.getUnitsFile() ) << endl;
  cout << "Factions data taken from " << * ( gameConfig.getFactionsFile() ) << endl;
  cout << "Locations data taken from " <<
       * ( gameConfig.getLocationsFile() ) << endl;

	DataManipulator dataManipulator;
  dataManipulatorPtr = & dataManipulator;

  dataManipulator.turnPreProcessing();

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



  ReportPattern::initialize( "report_patterns.txt" );

  cout << endl << endl << " ================= Combat processing ==============="
       << endl << endl;



  cout << endl << endl << " ================= End of combat processing ==============="
       << endl << endl;
	if ( gameConfig.runMode == TIME_TEST )
  {
    time( & end );
    dif2 = difftime( end, start );
    time( & start );
  }


  if ( gameConfig.runMode == TIME_TEST )
  {
    time( & end );
    dif3 = difftime( end, start );
    cout << endl << "Reading: " << dif1 << " sec." << "Processing: " << dif2
         << " sec." << " Saving : " << dif3 << " sec." << " Total RunTime: "
         << ( dif1 + dif2 + dif3 ) << " sec." << endl;
  }
  cout << " ===================================================" << endl;


//  if ( currentDay < 0 ) sampleTokenEntity.cancelTeachingOffer(); // to provide
  // instantiation of sampleTokenEntity



}
