/***************************************************************************
                          StringData.h
                             -------------------
    begin                : Thu May 22 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef STRING_DATA_H
#define STRING_DATA_H
#include <string>
#include "AbstractData.h"

/**
  *@author Alex Dribin
  */

class StringData : public AbstractData  {
public: 
	StringData(const string &Data){Data_ = Data;}
	~StringData(){}
  inline void saveAsParameter (ostream &out) {out << " "<<Data_;}
  inline string print()  {return Data_;}
  inline void clean(){ delete this;}
  protected:
  string Data_;
};

#endif
