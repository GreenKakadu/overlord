/***************************************************************************
                          BasicAttribute.h
                             -------------------
    begin                : Thu Mar 11 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef BASIC_ATTRIBUTE_H
#define BASIC_ATTRIBUTE_H
#include <vector>
#include <iostream>
#include "OverlordTypes.h"
//class AbstractData;
class Parser;

using namespace std;
/**Defines ISRARH interface for Entity Properties
  *  ISRARH = Initialize Save Report Add Remove Has
  *@author Alex Dribin
  */

class BasicAttribute {
public:
	BasicAttribute();
	virtual ~BasicAttribute();
  virtual STATUS     initialize      ( Parser *);
  virtual void save(ostream &out);
  virtual ostream& report(ostream &out);
  virtual void clean() {}
//  virtual void add(AbstractData * data);
//  virtual void remove(AbstractData * data);
//  virtual bool has(AbstractData * data);
};

#endif
