/***************************************************************************
                          TertiaryPattern.cpp 
                             -------------------
    begin                : Wed Feb 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#include "TertiaryPattern.h"
#include "Reporter.h"
extern const int BLOCK_SIZE;

void * TertiaryPattern::operator new(size_t size)
{
   return NEW<TertiaryPattern>( size);
}


void  TertiaryPattern::operator delete(void * deadObject, size_t size)
{
	DELETE_PATTERN<TertiaryPattern>(deadObject, size);
}


void TertiaryPattern::print(ostream &out) const
{
  reporter_->print(out, param1_, param2_, param3_ );
}



void TertiaryPattern::clean()
{
  param1_->clean();
  param2_->clean();
  param3_->clean();
}




TertiaryPattern  * TertiaryPattern::headOfFreeList;

