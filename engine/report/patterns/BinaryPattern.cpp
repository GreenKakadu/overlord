/***************************************************************************
                          BinaryPattern.cpp 
                             -------------------
    begin                : Thu Feb 13 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "BinaryPattern.h"
#include "Reporter.h"
extern const int BLOCK_SIZE;

void * BinaryPattern::operator new(size_t size)
{
   return NEW<BinaryPattern>( size);
}


void  BinaryPattern::operator delete(void * deadObject, size_t size)
{
//  cout<< "Destroyed Binary pattern \n"; 
  DELETE_PATTERN<BinaryPattern>(deadObject, size);
}


void BinaryPattern::print(ostream &out) const
{
  reporter_->print(out, param1_, param2_);
}

void BinaryPattern::clean()
{
  param1_->clean();
  param2_->clean();
}



BinaryPattern  * BinaryPattern::headOfFreeList;

