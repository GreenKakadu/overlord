/***************************************************************************
                          DataHandler.cpp  

                             -------------------
    begin                : Wen May 22 13:52:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "DataHandler.h"
#include "PrototypeManager.h" 
// #include "PrototypeManager.h" 
// extern PrototypeManager  prototypeManager;

DataHandler::DataHandler( BasicCollection &newCollection,
			   const char * filename)
{
  collection_ =  &newCollection;
  parser_ = new FileParser ( filename );
  beginning_ = parser_->getPosition();
  do //Find collection keyword  definition
    {
      parser_->getLine();
    } while (! ( parser_->matchKeyword ("KEYWORD"))  || parser_->eof() );

  collectionKeyword_ = parser_->getWord();

    
}

DataHandler::~DataHandler()
{
  delete &collection_;
  delete parser_;
  cout << "Data handler ["<<collectionKeyword_ <<"] Destroyed " << endl;
}



int DataHandler::load()
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
return 0;
}
int DataHandler::save()
{

return 0;
}
int DataHandler::initialize()
{
 GameData * currentObject = 0;
  string currentTag;
  int i=0;

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
return 0;
}

void  DataHandler::print()
{
    cout << "====================== "<< collectionKeyword_  <<" ======================" <<endl;
    collection_ -> print();

}
