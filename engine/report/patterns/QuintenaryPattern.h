/***************************************************************************
                          QuintenaryPattern.h
                             -------------------
    begin                : Mon Apr 28 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef QUINTENARY_PATTERN_H
#define QUINTENARY_PATTERN_H

#include "ReportPattern.h"
class AbstractData;
class Reporter;

/**
  *@author Alex Dribin
  */

class QuintenaryPattern : public ReportPattern  {
public: 
	QuintenaryPattern(){}
	QuintenaryPattern(Reporter * reporter, AbstractData * param1,
                    AbstractData * param2, AbstractData * param3,
                    AbstractData * param4, AbstractData * param5)
                    {reporter_ = reporter; param1_ = param1; param2_ = param2;
                     param3_ = param3; param4_ = param4; param5_ = param5;}
  static void * operator new(size_t size);
  static void   operator delete(void * deadObject, size_t size);

  void printReport(ostream &out) const;
  void clean();
	static  QuintenaryPattern * headOfFreeList;
	union
	{
			QuintenaryPattern  * next; //for use in operator new
  		Reporter * reporter_;
	};
protected:
	~QuintenaryPattern(){}
		AbstractData * param1_;
		AbstractData * param2_;
		AbstractData * param3_;
		AbstractData * param4_;
		AbstractData * param5_;
};

#endif
