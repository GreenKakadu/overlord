/***************************************************************************
                      BasicCombatEngine.h
            Provides generic interface to all combat engines
                          ------------------
    begin                : Mon May 24 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include <iostream>
#include <functional>
#include "BasicCombatEngine.h"
#include "TokenEntity.h"
BasicCombatEngine combatEngine;
BATTLE_RESULT BasicCombatEngine::processBattle(vector<TokenEntity *> &attackers,
                    vector<TokenEntity *> &defenders)
{
  cout << "Processing Battle \n";
  cout << "Attacker side: \n";
  vector<TokenEntity *>::iterator iter; 
  for(iter = attackers.begin(); iter != attackers.end(); ++iter )
  {
    cout << (*iter)->print();
  }
  cout<<endl;
  cout << "Defender side: \n";
  for(iter = defenders.begin(); iter != defenders.end(); ++iter )
  {
    cout << (*iter)->print();
  }
  cout<<endl;
  return BATTLE_UNDEFINED;
}
