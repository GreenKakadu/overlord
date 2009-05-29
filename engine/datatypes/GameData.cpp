/***************************************************************************
                          GameData.cpp
                       Basic Polymorphic Persistent object
                             -------------------
    begin                : Mon Jun 10 12:24:42 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include <algorithm>
#include "GameData.h"
#include "ProcessingMode.h"
#include "PrototypeManager.h"
GameData       sampleGameData("GameData",0);



/** Used only for creation of sample objects for use in PrototypeManager  */
GameData::GameData (const string & keyword, GameData * parent)
{
  keyword_= keyword;
  parent_ = parent;
  name_ = keyword;
  transform (name_.begin() + 1, name_.end(),
             name_.begin() + 1, (int(*)(int))tolower);
//	cout << keyword<< " created"<<endl; // Debugging print
  if( prototypeManager == 0)
  {
	  cout << "Prototype Manager created on "<< keyword<< " call"<<endl;
    prototypeManager = new PrototypeManager;
  }
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




/** Produces a clone of sample object */
GameData *
GameData::createInstanceOfSelf ( )
{
  return CREATE_INSTANCE<GameData> (this);
}



/** initialization  with saved object data */
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



/**  saving object data */
void
GameData::save(ostream &out)
{
  out << keyword_ << " " <<tag_ << endl;
  if(!name_.empty()) out << "NAME " <<name_ << endl;
  if(!description_.empty()) out << "DESCRIPTION " <<description_  << endl;
  out << endl;
}



/** Equivalence: objects are equal if their tags are equal */
bool GameData::operator ==  (GameData data2)
{
  return this->tag_ == data2.getTag();
}



/** Creation of object of type specified by keyword  */
GameData * GameData::createByKeyword(const string &keyword)
{
 GameData * temp =  prototypeManager->findInRegistry(keyword);
	if(temp == 0)
	{
		cout << "Unknown polymorphic persistent object [" << keyword  <<"]"<< endl;
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


/** Used for determining place of current object in object hierarchy*/
/** Checks current object if it is descedant from the object */


     bool GameData::isDescendantFrom  (GameData * object)
{
  string keyword = object->getKeyword();
//  cout << "checking object type of "<< keyword<<endl;
  if ( keyword == getKeyword() )
    return true;
  GameData * currentParent = getParent();

  while (currentParent)
    {
      if ( currentParent  -> getKeyword() == keyword )
	return true;
      else
	currentParent = currentParent -> getParent();
    }
    return false;
}

