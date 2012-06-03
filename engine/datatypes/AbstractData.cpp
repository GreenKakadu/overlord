/***************************************************************************
                          AbstractData.cpp 
                             -------------------
    begin                : Tue Mar 11 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "AbstractData.h"
#include "LineParser.h"
#include "PrototypeManager.h"
 vector <AbstractData *> AbstractData::aPrint()
{
  vector <AbstractData *> v; 
  v.push_back(this); 
  return v;
}
 /** Creation of object of type specified by keyword  */
AbstractData * AbstractData::createByKeyword(const string &keyword)
{
 AbstractData * temp =  prototypeManager->abstractFindInRegistry(keyword);
	if(temp == 0)
	{
		cout << "Unknown polymorphic persistent object [" << keyword  <<"]"<< endl;
		return 0;
	}
	else
	{
 		return temp ->createAbstractInstance();
 	}
}


// This is a generic wrapper for createInstanceOfSelf function
// createInstanceOfSelf for each class creates instance of that class
AbstractData * AbstractData::createAbstractInstance()
{
   return new AbstractData();
}

AbstractData * AbstractData::loadByKeyword(const string &keyword, Parser *parser)
{
    // get first word - keyword
    //
    AbstractData * temp =  createByKeyword(keyword);
    return temp->loadInstance(parser);
}

AbstractData * AbstractData::loadInstance(Parser *parser)
{
    return 0;
}