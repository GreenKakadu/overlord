/***************************************************************************
                          UnaryPattern.cpp  -  description
                             -------------------
    begin                : Wed Jan 22 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "UnaryPattern.h"
#include "Reporter.h"
extern const int BLOCK_SIZE;

void * UnaryPattern::operator new(size_t size)
{
   return NEW<UnaryPattern>( size);
}


void  UnaryPattern::operator delete(void * deadObject, size_t size)
{
	DELETE_PATTERN<UnaryPattern>(deadObject, size);
}


void UnaryPattern::print(ostream &out) const
{
  reporter_->print(out, param1_);
}

UnaryPattern  * UnaryPattern::headOfFreeList;

