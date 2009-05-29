/***************************************************************************
                          EndLineMessage.h  -  description
                             -------------------
    begin                : Dec 07 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef END_LINE_PATTERN_H
#define END_LINE_PATTERN_H
#include "ReportMessage.h"
class ReportPattern;

class EndLineMessage : public ReportMessage  {
public:
	EndLineMessage(){}
  void printReport(ReportPrinter &out) const;
protected:
	~EndLineMessage(){}
};
extern EndLineMessage * endLineMessage;
#endif
