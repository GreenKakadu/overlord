/***************************************************************************
                          BasicAttribute.cpp
                             -------------------
    begin                : Thu Mar 11 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "BasicAttribute.h"
#include "LineParser.h"
#include "Entity.h"

BasicAttribute::BasicAttribute()
{
// attributeOwner->registerAttribute(this);
}

BasicAttribute::~BasicAttribute()
{
}

STATUS
BasicAttribute::initialize        ( Parser *parser )
{


  if (parser->matchKeyword ("NAME") )
    {
//      setName(parser->getText());
      return OK;
    }

      return OK;

 }



void BasicAttribute::save(ostream &out)
{

}



ostream&  BasicAttribute::report(ostream &out)
{
  return out;
}



//void BasicAttribute::add(AbstractData * data)
//{
//
//}
//
//
//
//void BasicAttribute::remove(AbstractData * data)
//{
//
//}
//
//
//
//bool BasicAttribute::has(AbstractData * data)
//{
//  return false;
//}


