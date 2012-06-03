/***************************************************************************
                          IntegerData.cpp 
                             -------------------
    begin                : Wed Mar 5 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "IntegerData.h"
#include "LineParser.h"
string longtostr(long u);

/** returns name for use in reports and messages
 * (string)
 */
string IntegerData::print() 
{

  return longtostr(value_);
}


// creates instance of integer data with value 0
AbstractData * IntegerData::createAbstractInstance()
{
   return new IntegerData(0);
}



AbstractData *  IntegerData::loadInstance(Parser *parser)
{
   setValue(parser->getInteger());
   return this;
}



/** returns integer in a form of parameter accepted by order */
void      IntegerData::saveAsParameter (ostream &out)
{
  out <<  " " << value_;
}

