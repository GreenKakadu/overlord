/***************************************************************************
                          QuartenaryPattern.h 
                             -------------------
    begin                : Wed Mar 5 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef QUARTENARY_PATTERN_H
#define QUARTENARY_PATTERN_H

#include "ReportPattern.h"
class AbstractData;
class Reporter;

/**
  *@author Alex Dribin
  */

class QuartenaryPattern : public ReportPattern  {
public: 
	QuartenaryPattern(){}
	QuartenaryPattern(Reporter * reporter, AbstractData * param1, AbstractData * param2, AbstractData * param3, AbstractData * param4)
        {reporter_ = reporter; param1_ = param1; param2_ = param2; param3_ = param3; param4_ = param4;}
  static void * operator new(size_t size);
  static void   operator delete(void * deadObject, size_t size);

  void printReport(ostream &out) const;
  void clean();
	static  QuartenaryPattern * headOfFreeList;
	union
	{
			QuartenaryPattern  * next; //for use in operator new
  		Reporter * reporter_;
	};
protected:
	~QuartenaryPattern(){}
		AbstractData * param1_;
		AbstractData * param2_;
		AbstractData * param3_;
		AbstractData * param4_;
};

#endif
