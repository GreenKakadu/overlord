/***************************************************************************
                          EnchantmentsAttribute.h
                             -------------------
    begin                : Thu Mar 11 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef EFFECTS_ATTRIBUTE_H
#define EFFECTS_ATTRIBUTE_H

#include "BasicAttribute.h"
class EnchantmentElement;
class EntityStatistics;

/**
  *@author Alex Dribin
  */

class EnchantmentsAttribute : public BasicAttribute  {
public: 
	EnchantmentsAttribute();
	~EnchantmentsAttribute();
  STATUS     initialize      ( Parser *);
  void save(ostream &out);
  ostream& report(ostream &out);
  void add(EnchantmentElement * data);
  void remove(EnchantmentElement * data);
  void addStats(EntityStatistics * stats, int figuresNumber );
  void processExpiration(int figuresNumber );
  int getCapacity(int modeIndex, int figuresNumber );
   EnchantmentElement * has(EnchantmentElement * data);
  EnchantmentElement * findAndDo(EnchantmentElement * data, EnchantmentElement * (*toDo) ());
    protected:
  vector <EnchantmentElement *> dataCollection_;
};
typedef   vector <EnchantmentElement *>::iterator  EnchantmentAttributesIterator;
#endif
