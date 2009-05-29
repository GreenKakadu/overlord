/***************************************************************************
                          ReportMessage.h
Usualy reports are standard messages that only have very few different fields
Each Report Message is able to print it's message, inserting into it
appropriate parameters.
ReportMessage is the Generic object for all report patterns.
                             -------------------
    begin                : Thu Dec 26 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef REPORT_PATTERN_H
#define REPORT_PATTERN_H

#include <iostream>
#include <stdlib.h>
#include "ReportPattern.h"
#include "ReportRecord.h"
extern const int BLOCK_SIZE;
using namespace std;
class ReportMessage {
public:
  virtual void printReport(ReportPrinter &) const{}
  virtual void clean() {}
  virtual int getTag(){return 0;} // dummy method which is never used
                            // defined in order to provide  reportElements
                            // with interface similar with other elements

protected:
	ReportMessage(){}
	virtual ~ReportMessage(){}
};


template <class T>  static void * NEW (size_t size)
{
	if(size != sizeof(T) )
		return ::operator new(size);

	T * p =  T::headOfFreeList;
	if(p)
			T::headOfFreeList = p->next;
	else
		{
			void * temp =				(:: operator new ( BLOCK_SIZE * sizeof (T) ) );
			T * newBlock = static_cast<T *> (temp);
			for (int i = 1; i < BLOCK_SIZE - 1 ; ++i)
				{
					newBlock[i].next = &newBlock[i+1];
				}
			newBlock[BLOCK_SIZE - 1 ].next = 0;
			p = newBlock;
			T::headOfFreeList = &newBlock[1];
		}
	return p;
}
template <class T>  static void  DELETE_PATTERN (void * deadObject, size_t size)
{
 if(deadObject == 0) return;
 if(size != sizeof(T) )
		{
			::operator delete(deadObject);
			return;
		}
	T * p = static_cast<T *> (deadObject);
	p->next = T::headOfFreeList;
	T::headOfFreeList = p;
}
  inline ReportPrinter& operator << (ReportPrinter& out,  ReportMessage & data)
                               { data.printReport(out); return out;}


#endif
