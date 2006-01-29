/***************************************************************************
                              CombatReport.h
                             -------------------
    begin                : Dec 6 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef COMBAT_REPORT_H
#define COMBAT_REPORT_H
#include <vector>
#include "ReportPrinter.h"
using namespace std;

/**Report about battle (collection of ReportMessages)
  *@author Alex Dribin
  */
class ReportMessage;

class CombatReport {
public:
   CombatReport();
   ~CombatReport();
		void add(ReportMessage * message);
		void print(ReportPrinter &out);
 protected:
	vector <ReportMessage *> messageCollection_;



};

   inline ostream& operator << (ReportPrinter& out,  CombatReport& combatReport)
                                { combatReport.print(out); return out;}
   inline void operator >> (ReportMessage * message, CombatReport& combatReport)
                                { combatReport.add(message);}
#endif
