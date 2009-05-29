/***************************************************************************
                          OwnershipPolicy.cpp  -  description
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
#include "OwnershipPolicy.h"
#include "StanceVariety.h"

OwnershipPolicy::OwnershipPolicy(/*LocationEntity * location*/)
{
// location_ = location; 
 movePermission_ = 0;
}



void OwnershipPolicy::initialize(Parser * parser)
{
  if (parser->matchKeyword("OWNERSHIP_POLICY"))
	{
   movePermission_ = stances[parser->getWord()];	 
	}
  if(movePermission_ == 0) // default
    movePermission_ = neutralStance;
}



void OwnershipPolicy::save(ostream &out)
{
  out<<"OWNERSHIP_POLICY "<< movePermission_->getTag()<< endl;
}



void OwnershipPolicy::report(ostream &out)
{
  if(movePermission_ == 0)
    { cout << "OwnershipPolicy:: no movePermission" << endl; return;}
  out<<" Only "<< movePermission_->getName()<<" factions allowed to move.";
}



StanceVariety * OwnershipPolicy::getHarvestPermission(ItemRule * item)
{
  return friendlyStance;
}



StanceVariety * OwnershipPolicy::getBuyPermission(ItemRule * item)
{
  return neutralStance;
}



StanceVariety * OwnershipPolicy::getSellPermission(ItemRule * item)
{
  return neutralStance;

}



StanceVariety * OwnershipPolicy::getRecruitPermission(RaceRule * item)
{
  return neutralStance;

}




void OwnershipPolicy::setMovePermission(StanceVariety * stance)
{  
  movePermission_  = stance;
}



OwnershipPolicy::~OwnershipPolicy()
{
}



StanceVariety * OwnershipPolicy::getMovePermission()
{
  return movePermission_;

}
