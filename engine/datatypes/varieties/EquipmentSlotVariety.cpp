/***************************************************************************
                          EquipmentSlotVariety.cpp 
                             -------------------
    begin                : Mon Jan 6 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "EquipmentSlotVariety.h"

GameData * EquipmentSlotVariety::createInstanceOfSelf()
{
  return CREATE_INSTANCE<EquipmentSlotVariety> (this);
}


STATUS
EquipmentSlotVariety::initialize        ( Parser *parser )
{

	
  if (parser->matchKeyword ("NAME") )
    {
      setName(parser->getText());
      return OK;
    }
  if (parser->matchKeyword ("PLURAL") )
    {
//      setPluralName(parser->getText());
      return OK;
    }
  if (parser->matchKeyword("DESCRIPTION"))
    {
      setDescription(parser->getText());
      return OK;
    }

      return OK;

 }

