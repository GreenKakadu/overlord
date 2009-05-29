/***************************************************************************
                          BinaryMessage.h
                             -------------------
    begin                : Thu Feb 13 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#ifndef BINARY_PATTERN_H
#define BINARY_PATTERN_H

#include "ReportMessage.h"
class AbstractData;
class ReportPattern;

/**
  *@author Alex Dribin
  */

class BinaryMessage : public ReportMessage  {
public:
	BinaryMessage(){}
	BinaryMessage(ReportPattern * reporter, AbstractData * param1, AbstractData * param2){reporter_ = reporter; param1_ = param1; param2_ = param2;}
  static void * operator new(size_t size);
  static void   operator delete(void * deadObject, size_t size);

  void printReport(ReportPrinter &out) const;
  void clean();
	static  BinaryMessage * headOfFreeList;
	union
	{
			BinaryMessage  * next; //for use in operator new
  		ReportPattern * reporter_;
	};
protected:
	~BinaryMessage(){}
		AbstractData * param1_;
		AbstractData * param2_;

};

#endif
