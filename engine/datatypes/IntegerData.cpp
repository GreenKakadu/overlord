/***************************************************************************
                          IntegerData.cpp 
                             -------------------
    begin                : Wed Mar 5 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "IntegerData.h"
extern string longtostr(long u);

/** returns name for use in reports and messages
 * (string)
 */
string IntegerData::print() 
{

  return longtostr(value_);
}



/** returns integer in a form of parameter accepted by order */
void      IntegerData::saveAsParameter (ostream &out)
{
  out <<  " " << value_;
}
