/***************************************************************************
                          TertiaryPattern.h 
                             -------------------
    begin                : Wed Feb 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef TERTIARY_PATTERN_H
#define TERTIARY_PATTERN_H

#include "ReportPattern.h"
class AbstractData;
class Reporter;

/**
  *@author Alex Dribin
  */

class TertiaryPattern : public ReportPattern  {
public: 
	TertiaryPattern(){}
	TertiaryPattern(Reporter * reporter, AbstractData * param1, AbstractData * param2, AbstractData * param3)
        {reporter_ = reporter; param1_ = param1; param2_ = param2; param3_ = param3;}
  static void * operator new(size_t size);
  static void   operator delete(void * deadObject, size_t size);

  void print(ostream &out) const;
	static  TertiaryPattern * headOfFreeList;
	union
	{
			TertiaryPattern  * next; //for use in operator new
  		Reporter * reporter_;
	};
protected:
	~TertiaryPattern(){}
		AbstractData * param1_;
		AbstractData * param2_;
		AbstractData * param3_;
};

#endif
