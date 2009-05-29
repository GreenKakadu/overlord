/***************************************************************************
                          SimpleMessage.cpp  -  description
                             -------------------
    begin                : Tue Jan 21 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#include "SimpleMessage.h"
#include "ReportPattern.h"
extern const int BLOCK_SIZE;

void * SimpleMessage::operator new(size_t size)
{
   return NEW<SimpleMessage>( size);
}


void  SimpleMessage::operator delete(void * deadObject, size_t size)
{
	DELETE_PATTERN<SimpleMessage>(deadObject, size);
}


void SimpleMessage::printReport(ReportPrinter &out) const
{
  reporter_->printReport(out);
}

SimpleMessage  * SimpleMessage::headOfFreeList;
