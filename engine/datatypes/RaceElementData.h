/***************************************************************************
                          RaceElementData.h
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

#ifndef RACE_ELEMENT_DATA_H
#define RACE_ELEMENT_DATA_H

#include "AbstractData.h"
class RaceRule;

/**
  *@author Alex Dribin
  */

class RaceElementData : public AbstractData  {
public: 
	  RaceElementData(RaceRule * race, int num) {race_ =  race; num_ = num;}
    string printName();
    void saveAsParameter (ostream &out);
    protected:
  RaceRule * race_;
  int num_;
};

#endif
