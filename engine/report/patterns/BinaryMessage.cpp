/***************************************************************************
                          BinaryMessage.cpp
                             -------------------
    begin                : Thu Feb 13 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "BinaryMessage.h"
#include "ReportPattern.h"
extern const int BLOCK_SIZE;

void * BinaryMessage::operator new(size_t size)
{
   return NEW<BinaryMessage>( size);
}


void  BinaryMessage::operator delete(void * deadObject, size_t size)
{
//  cout<< "Destroyed Binary pattern \n";
  DELETE_PATTERN<BinaryMessage>(deadObject, size);
}


void BinaryMessage::printReport(ostream &out) const
{
  reporter_->printReport(out, param1_, param2_);
}

void BinaryMessage::clean()
{
  param1_->clean();
  param2_->clean();
}



BinaryMessage  * BinaryMessage::headOfFreeList;

