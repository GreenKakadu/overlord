/***************************************************************************
                          DataHandler.cpp  

                             -------------------
    begin                : Wen May 22 13:52:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#include <time.h>
#include <iostream>
#include <fstream>
#include "DataHandler.h"
#include "PrototypeManager.h" 
// #include "PrototypeManager.h" 
// extern PrototypeManager  prototypeManager;

DataHandler::DataHandler( BasicCollection &newCollection,
			   const char * filename)
{
  collection_ =  &newCollection;
  //  filename_ = string(filename);
  filename_ = filename;
  parser_ = new FileParser ( filename );
  beginning_ = parser_->getPosition();
  do //Find collection keyword  definition
    {
      parser_->getLine();
    } while (! ( parser_->matchKeyword ("KEYWORD"))  || parser_->eof() );

  collectionKeyword_ = parser_->getWord();
  if( parser_ -> matchInteger() )
    {
      collectionSize_ = parser_->getInteger();
      newCollection.resize(collectionSize_);
    }

    
}

DataHandler::~DataHandler()
{
  delete &collection_;
  delete parser_;
  cout << "Data handler ["<<collectionKeyword_ <<"] Destroyed " << endl;
}



STATUS DataHandler::load()
{
string tag;
string keyword;
string optionalDerivativeKeyword;
GameData * newObject;

   parser_->setPosition ( beginning_ );
  do //Find class keyword  definition
    {
      parser_->getLine();
     if (parser_->matchKeyword (collectionKeyword_.c_str()) )
       {
	  tag = parser_->getWord();
	  optionalDerivativeKeyword = parser_->getWord();
	  
	  if(optionalDerivativeKeyword.length() > 0)
	    {
	      keyword = optionalDerivativeKeyword;
	    }
	  else
	    {
	      keyword = collectionKeyword_;
	    }

	  GameData * newObject = GameData::prototypeManager->findInRegistry(keyword)
	    ->createInstanceOfSelf ();

	  newObject -> setTag(tag);

	  if(newObject->checkObjectType(collectionKeyword_))
	    collection_ -> add (newObject) ;
	 }
    } while (!  parser_->eof() );
return OK;
}
STATUS DataHandler::save()
{
  time_t rawtime;
  unsigned long i;

  cout << "Saving data for "<< collectionKeyword_  <<endl;
  ofstream outfile ((filename_ + ".new").c_str());
  outfile << "# Overlord units data " <<endl;
  time ( &rawtime );
  outfile << "# Ver 0.1 "<<ctime(&rawtime) <<endl;
  outfile << "KEYWORD " << collectionKeyword_ << " " <<collection_ -> getSize() <<endl;


  for (i=0; i < collectionSize_ ; i++)
    {
      if ((*collection_)[i] != 0)
	{
	  //cout << "Saving entity "<< ((*collection_)[i])->getTag()  <<endl;
	   ((*collection_)[i]) -> save(outfile);
	}
    }


  outfile.close();

return OK;
}
STATUS DataHandler::initialize()
{
 GameData * currentObject = 0;
  string currentTag;
  unsigned long i=0;

   parser_->setPosition ( beginning_ );
  while (!  parser_->eof() )
    {
      parser_->getLine(); 
     if ( parser_->matchKeyword (collectionKeyword_.c_str()))
	  {
	    currentTag = parser_->getWord();
	    currentObject = (*collection_)[currentTag]; 
	    if (currentObject != 0)
	      {
		i++;
	      }
	    else
	      {
		currentObject = 0;
		cerr << "Error while "<<  collectionKeyword_
		     << " data initialisation.  Can't find  object for tag "
		     <<	currentTag  <<   " ( " << i << " ) " << endl;
		return ERROR;
	      }
	  }
     else
       {
	 if(currentObject != 0)
	   {
	     currentObject  ->initialize ( parser_ ); 
	   }
       }
    }

  // // if  ( i != collection_.size() )
  // {
  // cerr << "Error while loading.  Expected " << collection_.size() << " "
  // <<  collectionKeyword_ << " Found " << i << endl;
// }
return OK;
}

void  DataHandler::print()
{
    cout << "====================== "<< collectionKeyword_  <<" ======================" <<endl;
    collection_ -> print();

}
