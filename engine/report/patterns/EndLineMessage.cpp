/***************************************************************************
                          EndLineMessage.cpp  -  description
                             -------------------
    begin                : Dec 07 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "EndLineMessage.h"
EndLineMessage * endLineMessage = new EndLineMessage();
void EndLineMessage::printReport(ReportPrinter &out) const
{
	out << endl;
}

