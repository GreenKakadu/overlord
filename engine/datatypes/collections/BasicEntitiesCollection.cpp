/***************************************************************************
                          BasicEntitiesCollection.cpp  
              Generic layer of Entities Collection 
                             -------------------
    begin                : Mon Jun 10 12:24:42 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include <time.h>
#include <algorithm>
#include "GameInfo.h"
#include "BasicEntitiesCollection.h"
#include "Entity.h"
#include "DataStorageHandler.h"
extern void longtostr(unsigned long u, char *out);
extern bool ciCharCompare(char c1, char c2);
extern GameInfo game;
//extern Entity * RIPplaceholder;
BasicEntitiesCollection::BasicEntitiesCollection (DataStorageHandler * handler,
                                          long int dimensions = 1000) : BasicCollection(handler)
{
  redimention(dimensions);
  dimensions_ = dimensions;
  emptyPlaces_ = dimensions - 1;
  // prefix should be configured later in game configuration
  // meanwhile it will be equal to the first letter of keyword
  prefix_ = 0;
  status = OK;
}
BasicEntitiesCollection::~BasicEntitiesCollection()
{
  // delete handler_; // <- check
}



GameData* BasicEntitiesCollection::findByTag (const string &tag, bool errorReportEnabled)
{
  long int index = getIndex(tag);
  if (status == OK)
         return data_[index];

  if(errorReportEnabled)
   cerr << "Error: Tag (" << tag  << ") was not found in "<<collectionKeyword_<<"-s collection\n";
 return 0 ;
}
GameData* BasicEntitiesCollection::findByIndex (const long int index, bool errorReportEnabled)
{
	if (index <= size())
         return data_[index];
  if(errorReportEnabled)
  cerr << "Error: Array index (" << index << ") is out of array dimensions!\n";
 return 0 ;
}


void   BasicEntitiesCollection::add  ( GameData * /*const*/ newEntity)
{
// cout << "Adding " << newEntity->printName()<<endl;
 long int index ;
 if (prefix_ == 0)
		{
			prefix_ = (newEntity->getTag())[0];
		}
 	index = getIndex(newEntity->getTag());
	if (status != OK)
		{
			cout << "GetIndex error while adding \n";
			return;
		}
	 if (data_[index]  != 0 )
   	{
     	cout << "Duplicated tag " << newEntity->getTag() <<endl;
		status = IO_ERROR;
		return;
   }
  else
    data_[index] =  dynamic_cast< Entity*> (newEntity);
 //   if (data_[index] == 0) cout << "Cast error\n";
// cout <<index<<"                "<< data_[index]->printName()<<" Added "<<endl;
   emptyPlaces_ --;
   if((emptyPlaces_ < 1) || (dimensions_ / emptyPlaces_ > 10 )) // Collection is almost full
     {
        redimention((long int)(dimensions_ * 1.1) ) ; // reallocate additional 10% of storage
     }
}



/** Note that isValidTag only checks existing entities but not temporary names of new entities*/
bool BasicEntitiesCollection::isValidTag (const string &tag) //const
{
EntitiesIterator iter;
 for (iter = begin(); iter != end(); iter++)
   {
      if(*iter == 0)
          continue;
      if ((*iter) ->getTag() == tag)
       return true;

   }
 return false;
}


void BasicEntitiesCollection::redimention (long int newSize)
{
  data_.resize(newSize);
  emptyPlaces_ = emptyPlaces_ + (newSize - dimensions_);
//  cout << dimensions_<< " - Reallocating memory to " <<newSize<<endl;// for test
  dimensions_ = newSize;

}

/** This function checks that given string may be tag for this type of data */
/** Note that for temporary names of new entities it will  false */
bool BasicEntitiesCollection::checkDataType(const string &tag)
{
	if(prefix_ == 0)
	{
		return true; // check disabled
	}

  if (!isalpha (tag[0]) )
    {
      return false;
    }
 if ( (tag[0] & 0x1F) == (prefix_ & 0x1F) )// after throwing away all
   //non-alpha  symbols this works as case-nonsensitive comparison of symbols
    return true;
  else
    return false;
}


/** Note: this method can also find Entity by it's temporary name */
long  BasicEntitiesCollection::getIndex (const string &tag)
{
 int i;
 int prefixLen=0;
 long int index;

	status = OK;

	 if(prefix_ != 0)
	{
  		if (!isalpha (tag[0]) )
    		{

     			cout << "Tag [" << tag << "] is invalid (non-alphabetical prefix)" <<endl;
				status = IO_ERROR;
				return 0;
    		}
 		if ( (tag[0] & 0x1F) != (prefix_ & 0x1F) )// after throwing away all
   //non-alpha  symbols this works as case-nonsensitive comparison of symbols
		   {
//				cout << "Tag "<< tag << " is invalid (wrong prefix: '" <<prefix_<<"' expected)\n";
				status = IO_ERROR;
				return 0;
		    }
		prefixLen=1;
	}

//  if ( ! ciStringCompareN(tag, game.getNewEntityPrefix(),game.getNewUnitPrefixSize() ) )
//   {
//     // New Unit tag
//
//    }
   
  for (i=0; i< Parser::INTEGER_LENGTH; i++)
   {
     if (!isdigit (tag[i+prefixLen]) )  // Non-digit was found


       break;

     if (!tag[i+prefixLen])  // End of string
       break;
   }

 if ( i == 0 ) // No digits were found.
    {
     cout << "Tag " << tag << " is invalid (no  digits)" <<endl;
 				status = IO_ERROR;
				return 0;
    }
 if ( i >=  Parser::INTEGER_LENGTH  ) // More than maximum of digits were found.
    {
      cout << "Tag " << tag << " is invalid (too many digits)" <<endl;
				status = IO_ERROR;
				return 0;
    }

 index = atoi (tag.c_str() +prefixLen);

 if (index >= dimensions_)
	{
     	cout << "Tag " << tag << " is invalid (too big index)" <<endl;
				status = IO_ERROR;
				return 0;
	}
 else
      return index;
}



void BasicEntitiesCollection:: remove  (const string &tag)
{
 long int index ;
 		index =getIndex(tag);
		if (status != OK)
   			return;

 if ( data_[index] == 0)
   return;

 else
   delete  data_[index];
    data_[index] = 0;//RIPplaceholder; // This slot shouldn't be used once more   
    RIP_.push_back(index);
    sort(RIP_.begin(),RIP_.end());
}



void BasicEntitiesCollection:: setEntityTagPrefix (char prefix)
{
	prefix_ = prefix;
}



string BasicEntitiesCollection::tagFromIndex(long int index)
{
  char buffer[Parser::INTEGER_LENGTH+2];
  
		if(prefix_ == 0)
    {
//		  sprintf(buffer,"%lu",index); // May use hand-made convertor itoa
      longtostr(index,buffer);
    }
		else
    {
        buffer[0]=prefix_;
        longtostr(index,buffer + 1);        
//		  sprintf(buffer,"%c%lu",prefix_,index);
     }
      return string(buffer);
}


void BasicEntitiesCollection::addRIPindex(long int index)
{
  RIP_.push_back(index);
}

long int BasicEntitiesCollection::getRIPsize() const
{
  return  RIP_.size();
}

long int BasicEntitiesCollection::getRIPbyIndex(long int i) const
{
  if(i < RIP_.size())
   return RIP_[i];
 cerr << "Error: RIP Array index (" << i << ") is out of array dimensions!\n";
 return 0 ;
}

NewEntityPlaceholder * BasicEntitiesCollection::findPlaceholder(const string &tag)
{
 vector <NewEntityPlaceholder *>::iterator iter;
 for (iter = newEntities_.begin(); iter != newEntities_.end(); iter++)
   {
      if ((*iter) ->getName() == tag)
       return (*iter);
   }
//   cout << "newPlaceholder with tag " << tag<< " should be created"<<endl;
 NewEntityPlaceholder * newPlaceholder = new NewEntityPlaceholder(tag);
 newEntities_.push_back(newPlaceholder);
 return newPlaceholder;
}
