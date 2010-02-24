/***************************************************************************
                          TertiaryMessage.cpp
                             -------------------
    begin                : Wed Feb 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#include "TertiaryMessage.h"
#include "ReportPattern.h"
extern const int BLOCK_SIZE;

void * TertiaryMessage::operator new(size_t size)
{
   return NEW<TertiaryMessage>( size);
}


void  TertiaryMessage::operator delete(void * deadObject, size_t size)
{
	DELETE_PATTERN<TertiaryMessage>(deadObject, size);
}


void TertiaryMessage::printReport(ReportPrinter &out) const
{
  reporter_->printReport(out, param1_, param2_, param3_ );
}



void TertiaryMessage::clean()
{
  param1_->clean();
  param2_->clean();
  param3_->clean();
}

vector <AbstractData *>  TertiaryMessage::aPrint()
{
  return reporter_->aPrint(param1_, param2_, param3_);
}



TertiaryMessage  * TertiaryMessage::headOfFreeList;

