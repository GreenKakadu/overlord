/***************************************************************************
                          EvenConflict.h
                             -------------------
    begin                : Tue Apr 22 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef EVEN_CONFLICT_H
#define EVEN_CONFLICT_H

#include "Rational.h"
#include "BasicConflict.h"

/**
  *@author Alex Dribin
  */
class ItemRule;

class EvenConflict : public BasicConflict  {
public: 
	EvenConflict(LocationEntity * location):BasicConflict(location){}
	~EvenConflict(){}
   void resolve(vector  <BasicCompetitiveRequest *> &requests);
  Rational getTotalAvailableValue(ItemRule * item) const;
};

#endif
