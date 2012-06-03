/***************************************************************************
                          ConstructionWorksVariety.cpp 
                             -------------------
    begin                : Tue Sep 2 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                                            *
 *   This program is free software; you can redistribute it and/or   *
 *  modify it under the terms of the BSD License.                       *
 *                                                                                            *
 ***************************************************************************/
#include "ConstructionWorksVariety.h"
ConstructionWorksVariety   sampleConstructionWork  ("CONSTRUCTION_WORK", &sampleGameData);
//VarietiesCollection <ConstructionWorksVariety>  construction_works(new DataStorageHandler("construction_works.var"));


ConstructionWorksVariety::ConstructionWorksVariety( const ConstructionWorksVariety * prototype ) : Variety(prototype)
{

}
ConstructionWorksVariety::~ConstructionWorksVariety(){}
GameData * ConstructionWorksVariety::createInstanceOfSelf()
{
  return CREATE_INSTANCE<ConstructionWorksVariety> (this);
}


STATUS
ConstructionWorksVariety::initialize        ( Parser *parser )
{

  if (parser->matchKeyword ("NAME") )
    {
      setName(parser->getText());
      return OK;
    }
  if (parser->matchKeyword("TEXT"))
    {
      setDescription(parser->getText());
      return OK;
    }
  if (parser->matchKeyword("PLURAL"))
    {
      pluralName_ = parser->getText();
      return OK;
    }

  return OK;
}
