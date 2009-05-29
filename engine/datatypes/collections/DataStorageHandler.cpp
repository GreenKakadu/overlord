/***************************************************************************
                          DataStorageHandler.cpp

                             -------------------
    begin                : Wen May 22 13:52:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include <time.h>
#include <iostream>
#include <fstream>
#include "DataStorageHandler.h"
#include "ProcessingMode.h"
#include "GameData.h"
#include "PrototypeManager.h"
#include "GameConfig.h"

extern bool testMode;
ofstream reportlist; 



DataStorageHandler::DataStorageHandler (string * filename)
{
	filename_ = filename;
  status = UNDEFINED;
//cout << "Collection(s) "<< filename->c_str() << " created.\n";
}



DataStorageHandler::DataStorageHandler (const char * filename)
{
  filenameString_ = string(filename);
	filename_ = &filenameString_;
	status = UNDEFINED;
//cout << "Collection "<< string(filename) << " created.\n";
}


void DataStorageHandler::setCollection(BasicCollection  *collection)
{
  	collection_ =  collection;
	if(status == OK)
    {
      collection->setCollectionKeyword(collectionKeyword_);
      collection->redimention(collectionSize_);
    }
}



DataStorageHandler::~DataStorageHandler()
{
  if(parser_) delete parser_;
//   collection_->clear();
  if(!collectionKeyword_.empty())cout << "Data handler ["<<collectionKeyword_ <<"] Destroyed " << endl;
}



STATUS DataStorageHandler::open()
{
 	parser_ = new FileParser ( filename_->c_str());
	if (parser_->status != OK)
			{
				status = IO_ERROR;
				return status;
			}
	status = OK;
  	beginning_ = parser_->getPosition();
  	do //Find collection keyword  definition
    	{
      		parser_->getLine();
    	} while (! ( parser_->matchKeyword ("KEYWORD"))  || parser_->eof() );

	if(parser_->eof() )
		{
			cout <<"Can't find KEYWORD definition "<<endl;
		}
  	collectionKeyword_ = parser_->getWord();
    if(collection_) collection_->setCollectionKeyword(collectionKeyword_);
  	if( parser_ -> matchInteger() )
    		{
      			collectionSize_ = parser_->getInteger();
      			collection_->redimention(collectionSize_);
    		}
//cout << "Collection "<< collectionKeyword_ << " created.\n";
				return status;
}



STATUS DataStorageHandler::load()
{
string tag;
string keyword;
string optionalDerivativeKeyword;
   if(status == UNDEFINED ) // delayed initialization
      open();
   if(status == IO_ERROR)
    return status;
   parser_->setPosition ( beginning_ );
   parser_->setLineNumber ( 0 );
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
       	GameData * temp =  /*GameData::*/prototypeManager->findInRegistry(keyword) ;
			if(temp == 0)
				{
					cout << "Can't  find object of type " <<  keyword << " in registry " << endl;
				}
			else
				{
  					GameData * newObject = temp ->createInstanceOfSelf ();
       if(newObject == 0)
        {
					cout << "Can't create object from tag " <<  tag  << endl;
          return IO_ERROR;
        }

	  				newObject -> setTag(tag);
//	  				if(newObject->checkObjectType(collectionKeyword_))
						{
	    					collection_ -> add (newObject) ;
//                cout << " After adding "<< newObject->print();

						}
				 }
		}

      if (parser_->matchKeyword ("RIP") )
       	{
          if(parser_->matchInteger())
            {
              collection_->addRIPindex(parser_->getInteger());
            }
          }
    } while (!  parser_->eof() );
if(keyword.empty())
	{
		cout <<"Data in the file " <<filenameString_<<"  are not matching keyword " <<  collectionKeyword_<<". May be it is empty."<<endl ;
	}
return OK;
}



STATUS DataStorageHandler::save()
{
  time_t rawtime;
  long int i;

  cout << "Saving data for "<< collectionKeyword_  <<endl;
  ofstream outfile ((*filename_ + ".new").c_str());
 //  cout << "TEST DataStorageHandler::save filename_: "<< *filename_<<endl;
  outfile << "# Overlord units data " <<endl;
  time ( &rawtime );
  outfile << "# Ver " <<  GameConfig::version <<" " <<ctime(&rawtime) <<endl;
  outfile << "KEYWORD " << collectionKeyword_ << " " <<collection_ -> size() <<endl;


  for (i=0; i < collection_ -> size()  ; i++)
    {
      if (collection_->findByIndex(i) != 0)
	      {
	        //cout << "Saving entity "<< ((*collection_)[i])->getTag()  <<endl;
	        (collection_->findByIndex(i)) -> save(outfile);
	      }
    }
  outfile <<endl;

  for (i=0; i < collection_->getRIPsize()  ; i++)
    {
      outfile << "RIP "<<collection_->getRIPbyIndex(i)<<endl;
    }

  outfile.close();
return OK;
}



STATUS DataStorageHandler::initializeData()
{
 GameData * currentObject = 0;
  string currentTag;
  long int i=0;

   parser_->setPosition ( beginning_ );
   parser_->setLineNumber ( 0 );
  while (!  parser_->eof() )
    {
      parser_->getLine();
     if ( parser_->matchKeyword (collectionKeyword_.c_str()))
	  {
	    currentTag = parser_->getWord();
	    currentObject = collection_->findByTag(currentTag);
	    if (currentObject != 0)
	      {
		i++;
	      }
	    else
	      {
		cerr << "Error while "<<  collectionKeyword_
		     << " data initialization.  Can't find  object for tag "
		     <<	currentTag  <<   " ( " << i << " ) " << endl;
		return IO_ERROR;
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


void  DataStorageHandler::printCollection()
{
    cout << "====================== "<< collectionKeyword_  << " (" << collection_ -> size() << ") ======================" <<endl;
 //   collection_ -> print();
int i;
for (i=0; i < collection_ -> size() ; i++)
    {
      if (collection_->findByIndex(i) != 0)
	      {
 				(collection_->findByIndex(i)) -> print();
		 }
	 }
}

// Provides possibility to add definition of specific objects in collection
void DataStorageHandler::define()
{
}


// Returns input line number for input error messages
int DataStorageHandler::getInputLineNumber()
{
	return parser_->getLineNumber();
}


// Returns input file name for input error messages
string * DataStorageHandler::getInputFileName()
{
	return filename_;
}
