/***************************************************************************
                          OwnershipPolicy.h  -  description
                             -------------------
    begin                : Tue Jun 8 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/


#ifndef OWNERSHIP_POLICY_H
#define OWNERSHIP_POLICY_H
#include <iostream>
//#include "PermissionElement.h"
//#include "FactionPermissionElement.h"
//class LocationEntity;
using namespace std;
class StanceVariety;
class ItemRule;
class RaceRule;
class Parser; 
/**
  *@author Alex Dribin
  * Ownership policy allows location owner to regulate suhc activities of other
  * factions as resource harvesting, selling / buying on market, recruiting and movement.
  * building - TBD
  * Current implementation provides only default policy (as in Overlord Rules)
  */

class OwnershipPolicy {
public: 
	OwnershipPolicy(/*LocationEntity * location*/);
	~OwnershipPolicy();
  StanceVariety * getHarvestPermission(ItemRule * item);
  StanceVariety * getBuyPermission(ItemRule * item);
  StanceVariety * getSellPermission(ItemRule * item);
  StanceVariety * getRecruitPermission(RaceRule * item);
  StanceVariety * getMovePermission();
  void setMovePermission(StanceVariety * stance);
  void initialize(Parser * parser);
  void save(ostream &out);
  void report(ostream &out);
protected:
//  LocationEntity * location_;
//  vector <PermissionElement> harvestPermissions_;
//  vector <PermissionElement> buyPermissions_;
//  vector <PermissionElement> sellPermissions_;
//  vector <PermissionElement> recruitPermissions_;
    StanceVariety * movePermission_;
//  vector <FactionPermissionElement> harvestFactionPermissions_;
//  vector <FactionPermissionElement> buyFactionPermissions_;
//  vector <FactionPermissionElement> sellFactionPermissions_;
//  vector <FactionPermissionElement> recruitFactionPermissions_;
//  vector <FactionPermissionElement> moveFactionPermission_;
};

#endif
