/***************************************************************************
                          DirectionVariety.cpp  -  description
                             -------------------
    begin                : Sun Jan 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#include "DirectionVariety.h"

//DirectionVariety::DirectionVariety(const string & keyword, GameData * parent)
//{
//    keyword_   = keyword;
//    parent_   = parent;
//    prototypeManager->addToRegistry(this);
//}
//
//DirectionVariety::DirectionVariety(const DirectionVariety * prototype)
//{
//}
VarietiesCollection <DirectionVariety>     directions(new DataStorageHandler("directions.var"));
DirectionVariety sampleDirection ("DIR",      &sampleGameData);

GameData * DirectionVariety::createInstanceOfSelf()
{
  return CREATE_INSTANCE<DirectionVariety> (this);
}


STATUS
DirectionVariety::initialize        ( Parser *parser )
{

	
  if (parser->matchKeyword ("NAME") )
    {
      setName(parser->getText());
      return OK;
    }

      return OK;

 }
