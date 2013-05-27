/***************************************************************************
                          MovementBonusAttribute.h
                             -------------------
    begin                : Tue Jun 8 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef MOVEMENT_BONUS_ATTRIBUTE_H
#define MOVEMENT_BONUS_ATTRIBUTE_H

#include "BasicAttribute.h"
#include "MovementBonusElement.h"

/**
  *@author Alex Dribin
  *
  * used for representing effect on movement speed
  */

class MovementVariety;

class MovementBonusAttribute : public BasicAttribute  {
public: 
	MovementBonusAttribute();
	~MovementBonusAttribute();
  STATUS     initialize      ( Parser *);
	int getMovementBonus(MovementVariety * mode);
  void save(ostream &out);
  void add( MovementBonusElement * data);
  void remove( MovementBonusElement * data);
  void clean();
  bool isEmpty();
      vector <AbstractArray>  aPrintReport();
    protected:
  vector <MovementBonusElement> movementBonuses_;
};

#endif
