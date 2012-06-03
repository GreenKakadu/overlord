/***************************************************************************
                          StringData.cpp 
                             -------------------
    begin                : Thu May 22 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "StringData.h"
#include "LineParser.h"

// creates instance of string data with value of empty string
AbstractData * StringData::createAbstractInstance()
{
   return new StringData("");
}

AbstractData *  StringData::loadInstance(Parser *parser)
{
   Data_ = parser->getWord();
   return this;
}

