/***************************************************************************
                          BinaryPattern.h 
                             -------------------
    begin                : Thu Feb 13 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef BINARY_PATTERN_H
#define BINARY_PATTERN_H

#include "ReportPattern.h"
class AbstractData;
class Reporter;

/**
  *@author Alex Dribin
  */

class BinaryPattern : public ReportPattern  {
public: 
	BinaryPattern(){}
	BinaryPattern(Reporter * reporter, AbstractData * param1, AbstractData * param2){reporter_ = reporter; param1_ = param1; param2_ = param2;}
  static void * operator new(size_t size);
  static void   operator delete(void * deadObject, size_t size);

  void printReport(ostream &out) const;
  void clean();
	static  BinaryPattern * headOfFreeList;
	union
	{
			BinaryPattern  * next; //for use in operator new
  		Reporter * reporter_;
	};
protected:
	~BinaryPattern(){}
		AbstractData * param1_;
		AbstractData * param2_;

};

#endif
