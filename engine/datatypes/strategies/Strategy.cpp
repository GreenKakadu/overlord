/***************************************************************************
                          Strategy.cpp
                             -------------------
    begin                : Sun Mar 16 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "Strategy.h"
#include "LineParser.h"

GameData * Strategy::createInstanceOfSelf()
{
  return CREATE_INSTANCE<Strategy> (this);
}

STATUS
Strategy::initialize        ( Parser *parser )
{


  if (parser->matchKeyword ("NAME") )
    {
      setName(parser->getText());
      return OK;
    }

      return OK;

 }
