/***************************************************************************
                          MovementBonusAttribute.cpp  -  description
                             -------------------
    begin                : Feb 16 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "MovementBonusAttribute.h"
#include "MovementBonusElement.h"
#include "MovementVariety.h"
#include "StringData.h"

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

void MovementBonusAttribute::save(ostream &out)
{
    for(vector <MovementBonusElement>::iterator iter =  movementBonuses_.begin();
    iter != movementBonuses_.end(); ++iter)
    {
        out<<"MOVE_BONUS ";
        (*iter).save(out);
    }
}
bool MovementBonusAttribute::isEmpty()
{
    return movementBonuses_.empty();
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
vector <AbstractArray> MovementBonusAttribute::aPrintReport()
{
    vector <AbstractArray> out;
    if(!movementBonuses_.empty())
    {
        vector <AbstractData *> v;
        v.push_back(new StringData("Movement bonuses: "));
         out.push_back(v);
         for (MovementBonusIterator  iter = movementBonuses_.begin();
           iter != movementBonuses_.end();  ++iter)
           {
             out.push_back((*iter).aPrintBonus());
            }
    }


    return out;
}
