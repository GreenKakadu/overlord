/***************************************************************************
                          Turn.cpp  -  main object of turn generator
                             -------------------
    begin                : Fri Nov  9 19:24:42 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "Global.h"
#include "GameInfo.h"
#include "DataManipulator.h"
// tests
extern GameInfo game;
extern int currentDay;
extern bool testMode;
extern ProcessingMode   immediateOrders;
extern ProcessingMode   stackOrders;
extern ProcessingMode 	dayOrders;
#ifndef VERSION
#define VERSION 0.3
#endif
int main(int argc, char *argv[])
{
  clock_t tick =0;
  tick = clock();
  time_t start, end;
  double dif1, dif2 = 0, dif3 = 0;
  time(&start);
  srand ( time(NULL) ); // init random

	cout <<endl << endl << " ==================================================="<<endl;
	cout << " ===================== Overlord "<<VERSION<<" ===================="<<endl;
	cout << " ==================================================="<<endl;


  if (argc >=2)
  {
#ifdef TEST_MODE
  	if(!strcmp (argv[1],"test"))
  		{
    		cout << "Test Mode"<< endl;
    		testMode = true;
 			 if (argc >=3)
 			 {
 				game.init(argv[2]);
 			 }
   			else
  				{
  	 			game.init("game");
  				}
 		}
  		else
 #endif
  		{
  	   	game.init(argv[1]);
  	   }
  }
  else
  {
  	 game.init("game");
  }

   cout<< "Units data taken from " <<*(game.getUnitsFile())<<endl;
   cout<< "Factions data taken from " <<*(game.getFactionsFile())<<endl;
   cout<< "Locations data taken from " <<*(game.getLocationsFile())<<endl;
//   cout<< "Orders taken from " <<game.getOrdersFileName()<<endl;
   DataManipulator dataManipulator;

   dataManipulator.turnPreProcessing();

	if ((game.runMode == MEMORY_TEST)||(game.runMode == DAILY_MEMORY_TEST))
	{
		cout << "After loading orders \n"; getchar();
	}

	if (game.runMode == TIME_TEST)
	{
		time(&end);
  	dif1 = difftime(end,start);
		time(&start);
  	cout << endl<<"Reading: "<<dif1 << " sec." <<endl;
		cout <<"......................................................................."<<endl;
	}

	cout <<endl << endl << " ================= Turn processing ==============="<< endl<<endl;

for (currentDay = 1;  currentDay<= game.daysInMonth ; currentDay ++)
	{
		cout <<endl << "    #### Processing day "<<currentDay<<" ####"<<endl;
    cout <<endl<< "Daily data pre-Processing  " <<endl<<endl;
  	dataManipulator.dailyPreProcessData();

    cout <<endl<< "Processing immediate orders " <<endl<<endl;
  	dataManipulator.processOrders(&immediateOrders);

    cout << "Processing stack orders " <<endl<<endl;
 		dataManipulator.processOrders(&stackOrders);
// process combat resoution  for all combats

    cout <<"Processing full day orders " <<endl<<endl;
	 	dataManipulator.processOrders(&dayOrders);

    cout <<"Processing competitive requests " <<endl<<endl;
    dataManipulator.processCompetitiveRequests(0);
    
	if (game.runMode == DAILY_MEMORY_TEST)
	{
		cout << "After Processing orders on day "<<currentDay<<"\n"; getchar();
	}

		dataManipulator.dailyUpdate();  // mana, time-lasting effects, item decay,  reports

	if (game.runMode == DAILY_MEMORY_TEST)
	{
		cout << "After Processing reports on day "<<currentDay<<"\n"; getchar();
	}
}
	cout <<endl << endl << " ============== End of Turn processing ============"<< endl<<endl;

	if (game.runMode == TIME_TEST)
	{
		time(&end);
		dif2 = difftime(end,start);
		time(&start);
	}
#ifdef TEST_MODE
//   if(testMode)  dataManipulator.print();
#endif
    dataManipulator.turnPostProcessing();
  	dataManipulator.save();
   	game.save();

	//... More operations
	cout <<endl << endl << " ==================================================="<<endl;
	cout << " ===================== The End ===================="<<endl;


	if (game.runMode == TIME_TEST)
	{
		time(&end);
  	dif3 = difftime(end,start);
  	cout << endl<<"Reading: "<<dif1 << " sec." <<"Processing: "<<dif2 << " sec." <<" Saving : "<<dif3 <<" sec." <<" Total RunTime: "<< (dif1+dif2+dif3) << " sec." <<endl;
	}
	cout << " ==================================================="<<endl;

//  cout << "Plains walking "<<terrains["moun"]->getTravelTime(movementModes["walk"])<<endl;
//  cout << "Plains riding "<<terrains["moun"]->getTravelTime(movementModes["ride"])<<endl;
//  cout << "Plains flying "<<terrains["moun"]->getTravelTime(movementModes["fly"])<<endl;
//  cout << "Plains swiming "<<terrains["moun"]->getTravelTime(movementModes["swim"])<<endl;
//  cout << "Plains climbing "<<terrains["moun"]->getTravelTime(movementModes["climb"])<<endl;

if(currentDay<0) samplePhysicalEntity.cancelTeachingOffer();  // to provide instantiation of samplePhysicalEntity

cout << "The size of sampleGameData is " << sizeof(sampleGameData) <<endl;
cout << "The size of sampleEntity is " << sizeof(sampleEntity) <<endl;
cout << "The size of samplePhysicalEntity is " << sizeof(samplePhysicalEntity) <<endl;
cout << "The size of sampleUnit is " << sizeof(sampleUnit) <<endl;
cout << "The size of sampleFaction is " << sizeof(sampleFaction) <<endl;
cout << "The size of sampleLocation is " << sizeof(sampleLocation) <<endl;

//	cout << " ===================   Pathfinding tests   ====================="<<endl;
//  cout << "Travel time from L165 to L175 is "<<
//          locations["L165"]->findExit(locations["L175"])->getTravelTime(walkingMode)<<endl;
//  locations["L165"]->findWalkingDistance(locations["L165"],locations["L101"],100);
}

