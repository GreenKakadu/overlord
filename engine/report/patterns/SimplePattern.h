/***************************************************************************
                          SimplePattern.h  -  description
                             -------------------
    begin                : Tue Jan 21 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef SIMPLE_PATTERN_H
#define SIMPLE_PATTERN_H
#include "ReportPattern.h"
class Reporter;

class SimplePattern : public ReportPattern  {
public: 
	SimplePattern(){}
	SimplePattern(Reporter * reporter){reporter_ = reporter;}
  static void * operator new(size_t size);
  static void   operator delete(void * deadObject, size_t size);

  void printReport(ostream &out) const;
	static  SimplePattern * headOfFreeList;
	union
	{
			SimplePattern  * next; //for use in operator new
  		Reporter * reporter_;
	};		
protected:
	~SimplePattern(){}
};

#endif
