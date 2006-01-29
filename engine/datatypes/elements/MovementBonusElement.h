/***************************************************************************
                          MovementBonusElement.h
                             -------------------
    begin                : Feb 15 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef MOVEMENT_BONUS_ELEMENT_H
#define MOVEMENT_BONUS_ELEMENT_H

#include "MovementVariety.h"


using namespace std;


/**
  *@author Alex Dribin
  */


class MovementBonusElement {
public: 
	MovementBonusElement(MovementVariety * mode, int bonusPoints);
 ~MovementBonusElement(){}
  inline MovementVariety *   getMovementMode()   const     {return movementMode_;}
  inline int          getMovementBonusPoints()  const     {return bonusPoints_;}
  inline void   setMovementBonusPoints(int value)     { bonusPoints_ = value;}
  inline bool isValidElement() const {return ( movementMode_ != 0 ) ;}
  void save(ostream & out);
  string print();
  static MovementBonusElement   readElement (Parser * parser);

	protected:
		MovementVariety * movementMode_;
		int bonusPoints_;

};
typedef  vector <MovementBonusElement >::iterator MovementBonusIterator;

#endif
