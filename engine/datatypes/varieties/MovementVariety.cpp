/***************************************************************************
                          MovementVariety.cpp  -  description
                             -------------------
    begin                : Wed Jan 15 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "MovementVariety.h"

MovementVariety  sampleMode  ("MOVEMENT",  &sampleVariety);
//VarietiesCollection <MovementVariety> movementModes(new DataStorageHandler("movements.var"));


GameData * MovementVariety::createInstanceOfSelf()
{
  return CREATE_INSTANCE<MovementVariety> (this);
}


STATUS
MovementVariety::initialize        ( Parser *parser )
{


  if (parser->matchKeyword ("NAME") )
    {
      setName(parser->getText());
      return OK;
    }
      return OK;

 }
ostream &operator << ( ostream &out, MovementVariety * mode)
{
	out << mode->getTag() << " ";
	return out;
}





