/***************************************************************************
                          SimplePattern.cpp  -  description
                             -------------------
    begin                : Tue Jan 21 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#include "SimplePattern.h"
#include "Reporter.h"
extern const int BLOCK_SIZE;

void * SimplePattern::operator new(size_t size)
{
   return NEW<SimplePattern>( size);
}


void  SimplePattern::operator delete(void * deadObject, size_t size)
{
	DELETE_PATTERN<SimplePattern>(deadObject, size);
}


void SimplePattern::printReport(ostream &out) const
{
  reporter_->printReport(out);
}

SimplePattern  * SimplePattern::headOfFreeList;
