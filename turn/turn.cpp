/***************************************************************************
                          Turn.cpp  -  main object of turn generator
                             -------------------
    begin                : Fri Nov  9 19:24:42 IST 2001
    copyright            : (C) 2001 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#include "GameData.h"
#include "PrototypeManager.h"
#include "RulesCollection.h"
#include "DataManipulator.h"

// #include "Title.h"
// #include "Skill.h"
// #include "Item.h"
// #include "Race.h"
// #include "MovementMode.h"
#include "TerrainRule.h"
// #include "Unit.h"
// #include "Faction.h"
// #include "Location.h"
// #include "Game.h"


// const int NUMBER_OF_DAYS = 30;

PrototypeManager * GameData::prototypeManager= new PrototypeManager;

// Game game("game");
GameData sampleGameData("GameData",0);
TerrainRule sampleTerrain("TERRAIN",&sampleGameData);

RulesCollection <TerrainRule>   terrains;


//... More collections


int main()
{

  DataManipulator dataManipulator;
  
  dataManipulator.addRules(terrains,"terrains" );

  // dataManipulator.addRules  (&titles );
  // dataManipulator.addRules  (&skills );
  // dataManipulator.addRules  (&items );
  // dataManipulator.addRules  (&races );
  // dataManipulator.addRules  (&movementModes );


//... add more collections
  // dataManipulator.addEntities  (&units );
  // dataManipulator.addEntities  (&factions );
  // dataManipulator.addEntities  (&locations );

  dataManipulator.load();
  dataManipulator.initialize();
  dataManipulator.print();
  dataManipulator.save();
  

//... More operations




 cout << endl<< endl << "Tests for RulesCollection operations:" <<  endl<< endl<<endl;
 cout << endl<< endl << "Terains:" <<  terrains.getSize() << endl<<endl;

 cout << "Test for out of range index: " << endl; 

 if(terrains[7])
   cout << "=====>  Tag [7] in range: " << (terrains[7])->getTag()  << endl;
 if(terrains[77])
   cout << "=====>    Tag [77] in range: " << (terrains[77])->getTag()  << endl;

 cout << "Test for finding by tag: " << endl  << "=====>      Name [fore]: " <<terrains["fore"]->getName();
 cout <<  " index " <<terrains.getIndex("fore") << endl;
 if (terrains.isValidTag("fore"))
   cout <<  "=====>  [fore] is a valid tag" <<  endl;
 else 
   cout <<  "=====>  [fore] is not a valid tag" << endl;
 
 if (terrains.isValidTag("wer"))
   cout <<  "=====>  [wer] is a valid tag" <<  endl;
 else 
   cout <<  "=====>  [wer] is not a valid tag" << endl;

}
