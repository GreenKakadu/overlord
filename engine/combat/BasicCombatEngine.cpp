/***************************************************************************
                      BasicCombatEngine.cpp
            Provides generic interface to all combat engines
                          ------------------
    begin                : Mon May 24 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include <iostream>
#include <functional>
#include "BasicCombatEngine.h"
#include "TokenEntity.h"
#include "FactionEntity.h"
#include "LocationEntity.h"
#include "CombatReport.h"
#include "reporting.h"

BasicCombatEngine combatEngine;
int BasicCombatEngine::battleId = 1;
extern int currentDay;
extern ReportPattern * combatStartReporter;
extern ReportPattern * combatStart2Reporter;
extern ReportPattern * combatEndReporter;
extern ReportPattern * combatAttackersReporter;
extern ReportPattern * combatDefendersReporter;
extern ReportPattern * listReporter;



BasicCombatEngine::BasicCombatEngine()
{
	report_ = new CombatReport();
//cout << "===> BasicCombatEngine created "<<report_ <<endl;
}

BATTLE_RESULT BasicCombatEngine::processBattle(vector<TokenEntity *> &attackers,
                    vector<TokenEntity *> &defenders)
{
cout << "=== NB ===> New combat created using BasicCombatEngine created" <<endl;
 new UnaryMessage(combatStartReporter,new IntegerData(BasicCombatEngine::battleId)) >>*report_ ;
 new BinaryMessage(combatStart2Reporter,new IntegerData(currentDay),(*(attackers.begin()))->getLocation()) >>*report_ ;

 new SimpleMessage(combatAttackersReporter) >>*report_ ;
  vector<TokenEntity *>::iterator iter;
  for(iter = attackers.begin(); iter != attackers.end(); ++iter )
  {
    new UnaryMessage(listReporter, (*iter)) >>*report_ ;
		(*iter)->setFullDayOrderFlag();// If combat happened
		                               // no more full day orders possible
		(*iter)->getFaction()->addCombatReport(report_);
		}
 endLineMessage >>*report_ ;

 new SimpleMessage(combatDefendersReporter) >>*report_ ;
  for(iter = defenders.begin(); iter != defenders.end(); ++iter )
  {
    new UnaryMessage(listReporter, (*iter)) >>*report_ ;
		(*iter)->setFullDayOrderFlag();// If combat happened
		                               // no more full day orders possible
		(*iter)->getFaction()->addCombatReport(report_);
  }

 new SimpleMessage(combatEndReporter) >>*report_ ;
 endLineMessage >>*report_ ;
	BasicCombatEngine::battleId++;
  return BATTLE_UNDEFINED;
}
