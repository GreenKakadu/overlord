/***************************************************************************
                          UnaryMessage.cpp  -  description
                             -------------------
    begin                : Wed Jan 22 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "UnaryMessage.h"
#include "ReportPattern.h"
extern const int BLOCK_SIZE;

void * UnaryMessage::operator new(size_t size)
{
   return NEW<UnaryMessage>( size);
}



void  UnaryMessage::operator delete(void * deadObject, size_t size)
{
	DELETE_PATTERN<UnaryMessage>(deadObject, size);
}



void UnaryMessage::printReport(ReportPrinter &out) const
{
  reporter_->printReport(out, param1_);
}



void UnaryMessage::clean()
{
  param1_->clean();
}



UnaryMessage  * UnaryMessage::headOfFreeList;

