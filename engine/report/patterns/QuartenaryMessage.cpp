/***************************************************************************
                          QuartenaryMessage.cpp
                             -------------------
    begin                : Wed Mar 5 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#include "QuartenaryMessage.h"
#include "ReportPattern.h"
extern const int BLOCK_SIZE;

void * QuartenaryMessage::operator new(size_t size)
{
   return NEW<QuartenaryMessage>( size);
}


void  QuartenaryMessage::operator delete(void * deadObject, size_t size)
{
	DELETE_PATTERN<QuartenaryMessage>(deadObject, size);
}


void QuartenaryMessage::printReport(ReportPrinter &out) const
{
  reporter_->printReport(out, param1_, param2_, param3_ , param4_ );
}



void QuartenaryMessage::clean()
{
  param1_->clean();
  param2_->clean();
  param3_->clean();
  param4_->clean();
}



QuartenaryMessage  * QuartenaryMessage::headOfFreeList;


