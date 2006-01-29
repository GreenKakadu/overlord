/***************************************************************************
                          QuintenaryMessage.cpp
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
#include "QuintenaryMessage.h"
#include "ReportPattern.h"
extern const int BLOCK_SIZE;

void * QuintenaryMessage::operator new(size_t size)
{
   return NEW<QuintenaryMessage>( size);
}


void  QuintenaryMessage::operator delete(void * deadObject, size_t size)
{
	DELETE_PATTERN<QuintenaryMessage>(deadObject, size);
}


void QuintenaryMessage::printReport(ReportPrinter &out) const
{
  reporter_->printReport(out, param1_, param2_, param3_ , param4_ , param5_ );
}

void QuintenaryMessage::clean()
{
  param1_->clean();
  param2_->clean();
  param3_->clean();
  param4_->clean();
  param5_->clean();
}




QuintenaryMessage  * QuintenaryMessage::headOfFreeList;
