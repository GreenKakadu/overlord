/***************************************************************************
                          QuartenaryPattern.cpp 
                             -------------------
    begin                : Wed Mar 5 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#include "QuartenaryPattern.h"
#include "Reporter.h"
extern const int BLOCK_SIZE;

void * QuartenaryPattern::operator new(size_t size)
{
   return NEW<QuartenaryPattern>( size);
}


void  QuartenaryPattern::operator delete(void * deadObject, size_t size)
{
	DELETE_PATTERN<QuartenaryPattern>(deadObject, size);
}


void QuartenaryPattern::printReport(ostream &out) const
{
  reporter_->printReport(out, param1_, param2_, param3_ , param4_ );
}



void QuartenaryPattern::clean()
{
  param1_->clean();
  param2_->clean();
  param3_->clean();
  param4_->clean();
}



QuartenaryPattern  * QuartenaryPattern::headOfFreeList;


