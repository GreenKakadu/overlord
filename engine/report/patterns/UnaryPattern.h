/***************************************************************************
                          UnaryPattern.h  -  description
                             -------------------
    begin                : Wed Jan 22 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef UNARY_PATTERN_H
#define UNARY_PATTERN_H

#include "ReportPattern.h"
class AbstractData;
class Reporter;
/**
  *@author Alex Dribin
  */

class UnaryPattern : public ReportPattern  {
public: 
	UnaryPattern(){}
	UnaryPattern(Reporter * reporter, AbstractData * param1){reporter_ = reporter; param1_ = param1;}
  static void * operator new(size_t size);
  static void   operator delete(void * deadObject, size_t size);

  void print(ostream &out) const;
	static  UnaryPattern * headOfFreeList;
	union
	{
			UnaryPattern  * next; //for use in operator new
  		Reporter * reporter_;
	};		
protected:
	~UnaryPattern(){}
		AbstractData * param1_;
};

#endif

