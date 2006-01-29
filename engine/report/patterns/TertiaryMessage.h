/***************************************************************************
                          TertiaryMessage.h
                             -------------------
    begin                : Wed Feb 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef TERTIARY_PATTERN_H
#define TERTIARY_PATTERN_H

#include "ReportMessage.h"
class AbstractData;
class ReportPattern;

/**
  *@author Alex Dribin
  */

class TertiaryMessage : public ReportMessage  {
public:
	TertiaryMessage(){}
	TertiaryMessage(ReportPattern * reporter, AbstractData * param1, AbstractData * param2, AbstractData * param3)
        {reporter_ = reporter; param1_ = param1; param2_ = param2; param3_ = param3;}
  static void * operator new(size_t size);
  static void   operator delete(void * deadObject, size_t size);

  void printReport(ReportPrinter &out) const;
  void clean();
	static  TertiaryMessage * headOfFreeList;
	union
	{
			TertiaryMessage  * next; //for use in operator new
  		ReportPattern * reporter_;
	};
protected:
	~TertiaryMessage(){}
		AbstractData * param1_;
		AbstractData * param2_;
		AbstractData * param3_;
};

#endif
