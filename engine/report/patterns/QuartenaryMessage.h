/***************************************************************************
                          QuartenaryMessage.h
                             -------------------
    begin                : Wed Mar 5 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#ifndef QUARTENARY_PATTERN_H
#define QUARTENARY_PATTERN_H

#include "ReportMessage.h"
class AbstractData;
class ReportPattern;

/**
  *@author Alex Dribin
  */

class QuartenaryMessage : public ReportMessage  {
public:
	QuartenaryMessage(){}
	QuartenaryMessage(ReportPattern * reporter, AbstractData * param1, AbstractData * param2, AbstractData * param3, AbstractData * param4)
        {reporter_ = reporter; param1_ = param1; param2_ = param2; param3_ = param3; param4_ = param4;}
  static void * operator new(size_t size);
  static void   operator delete(void * deadObject, size_t size);

  void printReport(ReportPrinter &out) const;
  void clean();
	static  QuartenaryMessage * headOfFreeList;
	union
	{
			QuartenaryMessage  * next; //for use in operator new
  		ReportPattern * reporter_;
	};
    inline ReportPattern * getPattern() {return reporter_;}
      vector <AbstractData *>  aPrint();
  protected:
	~QuartenaryMessage(){}
		AbstractData * param1_;
		AbstractData * param2_;
		AbstractData * param3_;
		AbstractData * param4_;
};

#endif
