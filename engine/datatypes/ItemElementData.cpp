/***************************************************************************
                          ItemElementData.cpp 
                             -------------------
    begin                : Wed Mar 5 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "ItemElementData.h"
#include "ItemRule.h"
extern void longtostr(unsigned long u, char *out);
string ItemElementData::printName() 
{
  if (item_ == 0) return "";
  char buffer[12];
      longtostr(num_,buffer);
//		  sprintf(buffer,"%d",num_);// May use hand-made convertor itoa
  if( num_ > 1)
      return string(buffer) + " " + item_->getPluralName() + item_->printTag();
  else
      return string(buffer) + " " + item_->printName();   
}



void   ItemElementData::saveAsParameter (ostream &out)
{
  out <<  " " << num_ <<  " " <<item_->getTag();
}
