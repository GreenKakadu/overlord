/***************************************************************************
                          GameData.cpp  
                       Basic Polymorphic Persistent object
                             -------------------
    begin                : Mon Jun 10 12:24:42 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "GameData.h"

            
GameData::GameData (const string & keyword, GameData * parent)
{
  keyword_= keyword; 
  parent_ = parent;
  prototypeManager->addToRegistry(this); 
}



GameData::GameData(const GameData * prototype)
{
      parent_ = prototype->getParent();
      keyword_ = prototype->getKeyword();
}



GameData *
GameData::checkObjectType( const string  &keyword)
{
  if ( keyword == getKeyword() )
    return this;
  GameData * currentParent = getParent();

  while (currentParent)
    {
      if ( currentParent  -> getKeyword() == keyword )
	return this;
      else
	currentParent = currentParent -> getParent();
    } 
    return 0;
}

GameData *
GameData::createInstanceOfSelf ( )
{
  return CREATE_INSTANCE<GameData> (this);
}


string 
GameData::getKeyword() const   
{
 return keyword_;
}

GameData *
GameData::getParent()       const
{
  return parent_;
}

 

STATUS 
GameData::initialize        ( Parser *parser )
{
  if (parser->matchKeyword ("NAME") )
    {
      setName(parser->getText());
      return OK;
    }
  if (parser->matchKeyword("DESCRIPTION"))
    {
      setDescription(parser->getText());
      return OK;
    } 
      return OK;
}

void GameData::print()
{
    cout  << getName();
    cout << " [" << getTag()  << "] "<< endl;
      
}

string 
GameData::getTag()  const  
{
 return tag_;
}



string 
GameData::getName() const  
{
  return name_;
}



string 
GameData::getDescription() const  
{
  return description_;
}



void 
GameData::setTag         ( const string &tag)    
{
  tag_ = tag; 
}



void 
GameData::setName        ( const string &name)  
{
 name_ = name;
}



void 
GameData::setDescription        ( const string &description)  
{
 description_ = description;
}


