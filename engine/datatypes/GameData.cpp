/***************************************************************************
                          GameData.cpp  
                       Basic Polymorphic Persistent object
                             -------------------
    begin                : Mon Jun 10 12:24:42 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "GameData.h"
#include "ProcessingMode.h"
#include "PrototypeManager.h"
 /** Used only for creation of sample objects for use in PrototypeManager  */
GameData::GameData (const string & keyword, GameData * parent)
{
  keyword_= keyword; 
  parent_ = parent;
//	cout << keyword<< " created"<<endl;
  prototypeManager->addToRegistry(this); 
}


/** Used in the cloning of new objects  */
GameData::GameData(const GameData * prototype)
{
	if( prototype == 0)
		cout << "Error in  GameData constructor: prototype is 0\n" ;
      parent_ = prototype->getParent();
      keyword_ = prototype->getKeyword();
}

/** Used for checking in downcasts*/
/** Returns pointer to current object if it is descedant from the object */
/** identified by keyword and 0 otherwise*/
//GameData *
//GameData::checkObjectType( const string  &keyword)
//{
////  cout << "checking object type of "<< keyword<<endl;
//  if ( keyword == getKeyword() )
//    return this;
//  GameData * currentParent = getParent();
//
//  while (currentParent)
//    {
//      if ( currentParent  -> getKeyword() == keyword )
//	return this;
//      else
//	currentParent = currentParent -> getParent();
//    }
//    return 0;
//}
/** Produces a clone of sample object */
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



void
GameData::save(ostream &out)
{
  out << keyword_ << " " <<tag_ << endl;
  if(!name_.empty()) out << "NAME " <<name_ << endl;
  if(!description_.empty()) out << "DESCRIPTION " <<description_  << endl;
  out << endl;
}



bool GameData::operator ==  (GameData data2)
{
  return this->tag_ == data2.getTag();
}



GameData * GameData::createByKeyword(const string &keyword)
{
 GameData * temp =  prototypeManager->findInRegistry(keyword);
	if(temp == 0)
	{
		cout << "Unknown polymorphic persistent object " << keyword  << endl;
		return 0;
	}
	else
	{
 		return temp ->createInstanceOfSelf ();
 	}
}
 /** Checks data consistency */
STATUS GameData::dataConsistencyCheck(){
return OK;
}



