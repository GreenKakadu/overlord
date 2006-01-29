/***************************************************************************
                          MovementBonusAttribute.cpp  -  description
                             -------------------
    begin                : Feb 16 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "MovementBonusAttribute.h"
#include "MovementBonusElement.h"
#include "MovementVariety.h"

MovementBonusAttribute::MovementBonusAttribute(){
}



MovementBonusAttribute::~MovementBonusAttribute(){
}



STATUS
MovementBonusAttribute::initialize        ( Parser *parser )
{
	if ( parser->matchKeyword ("MOVE_BONUS") )
    {
      MovementBonusElement moveBonus = MovementBonusElement::readElement(parser);
			if(moveBonus.isValidElement())
				movementBonuses_.push_back(moveBonus);
    }
   return OK;
}



int MovementBonusAttribute::getMovementBonus(MovementVariety * mode)
{
	for(MovementBonusIterator iter = movementBonuses_.begin();
	iter != movementBonuses_.end(); ++iter)
	{
		if ( (*iter).getMovementMode() == mode)
		return (*iter).getMovementBonusPoints();
	}
	return 0;
}



void MovementBonusAttribute::add( MovementBonusElement * data)
{
  for (MovementBonusIterator  iter = movementBonuses_.begin();
    iter != movementBonuses_.end();  ++iter)
    {
      if ( (*iter).getMovementMode() == data->getMovementMode() )
        {

          (*iter).setMovementBonusPoints((*iter).getMovementBonusPoints() +
									 data->getMovementBonusPoints()) ;
            return;
        }
    }
    movementBonuses_.push_back(*data);

}



void MovementBonusAttribute::remove(MovementBonusElement * data)
{
  for (MovementBonusIterator  iter = movementBonuses_.begin();
    iter != movementBonuses_.end();  ++iter)
    {
      if ( (*iter).getMovementMode() == data->getMovementMode() )
        {

          (*iter).setMovementBonusPoints((*iter).getMovementBonusPoints() -
									 data->getMovementBonusPoints()) ;

					if((*iter).getMovementBonusPoints() == 0)
          {
            movementBonuses_.erase(iter);
          }
         return;
        }
    }
}
