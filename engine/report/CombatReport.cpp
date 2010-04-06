/***************************************************************************
                              CombatReport.cpp
                             -------------------
    begin                : Dec 6 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "CombatReport.h"
#include "ReportMessage.h"
CombatReport::CombatReport()
{

}



CombatReport::~CombatReport()
{
}



void CombatReport::add(ReportMessage * message)
{
	messageCollection_.push_back(message);
}



void CombatReport::print(ReportPrinter &out)
{
 for (vector <ReportMessage *>::iterator iter = messageCollection_.begin();
 				iter != messageCollection_.end(); ++iter)
 {
   
  out << **iter;
//   (*iter)->printReport(out);   
 }
}



