/***************************************************************************
                          IntegerData.h 
                             -------------------
    begin                : Wed Mar 5 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef INTEGER_DATA_H
#define INTEGER_DATA_H
#include <stdio.h>
#include "AbstractData.h"

/**This is a class, enveloping integers into GameData.
   It serves to provide possibility to pass integers to reporting routines,
   demanding GameData as input.

  *@author Alex Dribin
  */

class IntegerData : public AbstractData  {
public: 
	IntegerData(int num) {value_ = num;}
  string printName();
  inline int  getValue() const {return value_;}
  inline void setValue(int num)  {value_ = num;}
  void saveAsParameter (ostream &out);
  protected:
  int value_;
  
};

#endif
