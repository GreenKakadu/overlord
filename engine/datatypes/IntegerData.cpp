/***************************************************************************
                          IntegerData.cpp 
                             -------------------
    begin                : Wed Mar 5 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "IntegerData.h"
extern void longtostr(unsigned long u, char *out);

string IntegerData::printName() 
{
  char buffer[12];
  longtostr(value_,buffer);
//		  sprintf(buffer,"%d",value_); // May use hand-made convertor itoa
  return string(buffer);
}
void      IntegerData::saveAsParameter (ostream &out)
{
  out <<  " " << value_;
}
