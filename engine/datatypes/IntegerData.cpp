/***************************************************************************
                          IntegerData.cpp 
                             -------------------
    begin                : Wed Mar 5 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "IntegerData.h"
extern void longtostr(unsigned long u, char *out);

/** returns name for use in reports and messages
 * (string)
 */
string IntegerData::printName() 
{
  char buffer[12];
  longtostr(value_,buffer);
  return string(buffer);
}



/** returns integer in a form of parameter accepted by order */
void      IntegerData::saveAsParameter (ostream &out)
{
  out <<  " " << value_;
}
