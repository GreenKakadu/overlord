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
#include "DataManipulator.h"
#include "misc/LineParser.h"
#include "GameFacade.h"
GameData       sampleGameData("GameData",0);
const string GameData::l_bracket("[");
const string GameData::r_bracket("]");
string GameData::objectKeyword = "GameData";
GameData *  GameData::objectParent = 0;
//extern DataManipulator * dataManipulatorPtr;

GameData * sampleGameDataO = GameData::registerObject();

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
    if (prototype == 0)
    {
        cout << "Error in  GameData constructor: prototype is 0\n";
    }
    else
    {
        parent_ = prototype->getParent();
        keyword_ = prototype->getKeyword();
    }
}

GameData::GameData()
{
}



GameData * GameData::createByKeyword(const string &keyword)
{
 AbstractData * temp =  AbstractData::createByKeyword(keyword);
 return dynamic_cast<GameData *>(temp);
}


AbstractData * GameData::createAbstractInstance()
{

   return createInstanceOfSelf();
}


/** Produces a clone of sample object */
GameData *
GameData::createInstanceOfSelf ( )
{
  return CREATE_INSTANCE<GameData> (this);
}

AbstractData *  GameData::loadInstance(Parser *parser)
{
    return (gameFacade->getDataManipulator()->findGameData(parser->getWord()));
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
void GameData::save(ostream &out)
{
  out << " " <<tag_ << endl;
  if(!name_.empty()) out << "NAME " <<name_ << endl;
  if(!description_.empty()) out << "DESCRIPTION " <<description_  << endl;
  out << endl;
}


void GameData::save(ostream &out, string prefix)
{
  if(!name_.empty()) out <<prefix<< " "<< "NAME " <<name_ << endl;
  if(!description_.empty()) out <<prefix<< " "<< "DESCRIPTION " <<description_  << endl;
}



/** Equivalence: objects are equal if their tags are equal */
bool GameData::operator ==  (GameData data2)
{
  return this->tag_ == data2.getTag();
}




string   GameData::getTag()          const { return tag_;}



/** returns tag for use in reports and messages
 *  (enbraced in square brackets)
 */
string GameData::printTag() const{
 return ( l_bracket + tag_ + r_bracket);
}



/** returns name for use in reports and messages
 * (followed by tag enbraced in square brackets)
 */
string GameData::print() /*const*/
{
 return (name_ + l_bracket + tag_ + r_bracket);
}



/** returns data identificator in a form of parameter accepted by order */
void      GameData::saveAsParameter (ostream &out)
{
  out <<  " " <<tag_;
}



string   GameData::getDescription()  const
{
  if(description_.empty())
    return string("No description");
  else
    return description_;
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

GameData * GameData::registerObject()
{
  GameData * t = new GameData();

  if( prototypeManager == 0)
  {
	  cout << "Prototype Manager created on "<< GameData::objectKeyword<< " call"<<endl;
    prototypeManager = new PrototypeManager;
  }
  prototypeManager->addToRegistry(t);
  return t;
}

