/***************************************************************************
                          TargetOrder.h
                             -------------------
    begin                : Tue Sep 23 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/

#ifndef TARGET_ORDER_H
#define TARGET_ORDER_H

#include "OrderPrototype.h"

/**
  *@author Alex Dribin
  */

class TargetOrder : public OrderPrototype  {
public: 
	TargetOrder();
	~TargetOrder(){}
  STATUS loadParameters(Parser * parser, vector <AbstractData *>  &parameters, Entity * entity );
  ORDER_STATUS process (Entity * entity, vector <AbstractData *>  &parameters);
  static bool isUnit(const string & target);
  static bool isBuildingOrShip(const string & target);
  static bool isConstruction(const string & target);
  static bool isLocation(const string & target);
  static bool isValidTarget(const string & target);
  static AbstractData * findTarget(const string & tag);
};

#endif
