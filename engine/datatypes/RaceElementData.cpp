/***************************************************************************
                          RaceElementData.cpp 
                             -------------------
    begin                : Tue Jun 10 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
 
#include "RaceElementData.h"
#include "RaceRule.h"
extern void longtostr(unsigned long u, char *out);
string RaceElementData::printName()
{
  if (race_ == 0) return "";
  char buffer[12];
      longtostr(num_,buffer);
//		  sprintf(buffer,"%d",num_);// May use hand-made convertor itoa
  if( num_ > 1)
      return string(buffer) + " " + race_->getPluralName() + race_->printTag();
  else
      return string(buffer) + " " + race_->printName();
}



void   RaceElementData::saveAsParameter (ostream &out)
{
  out <<  " " << num_ <<  " " <<race_->getTag();
}


