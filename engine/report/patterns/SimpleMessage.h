/***************************************************************************
                          SimpleMessage.h  -  description
                             -------------------
    begin                : Tue Jan 21 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef SIMPLE_PATTERN_H
#define SIMPLE_PATTERN_H
#include "ReportMessage.h"
class ReportPattern;

class SimpleMessage : public ReportMessage  {
public:
	SimpleMessage(){}
	SimpleMessage(ReportPattern * reporter){reporter_ = reporter;}
  static void * operator new(size_t size);
  static void   operator delete(void * deadObject, size_t size);

  void printReport(ostream &out) const;
	static  SimpleMessage * headOfFreeList;
	union
	{
			SimpleMessage  * next; //for use in operator new
  		ReportPattern * reporter_;
	};
protected:
	~SimpleMessage(){}
};

#endif
