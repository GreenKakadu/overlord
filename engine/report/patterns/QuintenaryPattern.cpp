/***************************************************************************
                          QuintenaryPattern.cpp 
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
#include "QuintenaryPattern.h"
#include "Reporter.h"
extern const int BLOCK_SIZE;

void * QuintenaryPattern::operator new(size_t size)
{
   return NEW<QuintenaryPattern>( size);
}


void  QuintenaryPattern::operator delete(void * deadObject, size_t size)
{
	DELETE_PATTERN<QuintenaryPattern>(deadObject, size);
}


void QuintenaryPattern::print(ostream &out) const
{
  reporter_->print(out, param1_, param2_, param3_ , param4_ , param5_ );
}

void QuintenaryPattern::clean()
{
  param1_->clean();
  param2_->clean();
  param3_->clean();
  param4_->clean();
  param5_->clean();
}




QuintenaryPattern  * QuintenaryPattern::headOfFreeList;
