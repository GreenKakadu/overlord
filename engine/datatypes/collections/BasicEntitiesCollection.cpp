/***************************************************************************
                          BasicEntitiesCollection.cpp
              Generic layer of Entities Collection
                             -------------------
    begin                : Mon Jun 10 12:24:42 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include <time.h>
#include <algorithm>
#include "GameConfig.h"
#include "BasicEntitiesCollection.h"
#include "Entity.h"
#include "DataStorageHandler.h"
extern string longtostr(long u);
extern bool ciCharCompare(char c1, char c2);
const  int BasicEntitiesCollection::incrementSize = 1000;
//extern Entity * RIPplaceholder;
BasicEntitiesCollection::BasicEntitiesCollection (DataStorageHandler * handler, GameData *sample,
                                          long int dimensions) : BasicCollection(handler,sample)
{
  dimensions_ =-1;
  redimention(dimensions);
  dimensions_ = dimensions;
  emptyPlaces_ = dimensions - 1;
  prefix_=0;
  // prefix should be configured later in game configuration
  // meanwhile it will be equal to the first letter of keyword
  if(sample)
  {
    prefix_ = (sample->getKeyword())[0];
  }
  status = OK;
}



BasicEntitiesCollection::~BasicEntitiesCollection()
{
  // delete handler_; // <- check
}



GameData* BasicEntitiesCollection::findByTag (const string &tag, bool errorReportEnabled)
{
  long int index = getIndex(tag,errorReportEnabled);
  if (status == OK)
         return data_[index];

  if(errorReportEnabled)
   cerr << "Error: Tag [" << tag << "] ("<<getStorageName()
           <<":"<<getStorageLocator() <<") was not found in "
           << "["<<getCollectionKeyword()<<"]"<<" collection"<<endl;
 return 0 ;
}

GameData* BasicEntitiesCollection::findByName (const string &name, bool errorReportEnabled)
{
    
 EntitiesIterator iter;
 for (iter = begin(); iter != end(); iter++)
   {
      if(*iter == 0)
          continue;
      if ((*iter) ->getName() == name)
       return *iter;

   }   
  if(errorReportEnabled)
   cerr << "Error: Name [" << name << "] ("<<getStorageName()
           <<":"<<getStorageLocator() <<") was not found in "
           << "["<<getCollectionKeyword()<<"]"<<" collection"<<endl;
 return 0 ;
}





GameData* BasicEntitiesCollection::findByIndex (const long int index, bool errorReportEnabled)
{
	if (index <= size())
         return data_[index];
  if(errorReportEnabled)
  cerr << "Error: Entity array index (" << index << ") is out of array dimensions!\n";
 return 0 ;
}


void   BasicEntitiesCollection::add  ( GameData * /*const*/ newEntity, bool isReportDuplication)
{
// cout << "Adding " << newEntity->print()<<endl;
 long int index ;
 if (prefix_ == 0)
		{
			prefix_ = (newEntity->getTag())[0];
		}
 	index = extractIndex(newEntity->getTag());
	if (status != OK)
		{
			cout << "Index extraction error while adding "<<newEntity->getTag()<< " with prefix ["<<prefix_ <<"]"<<endl;
			return;
		}
         if (index >= dimensions_)
	{
           long int newSize = ((index /BasicEntitiesCollection::incrementSize)+1) * BasicEntitiesCollection::incrementSize;
          redimention(newSize);
         }

         else if (data_[index]  != 0 )
   	{
             if(isReportDuplication)
             {
                cerr << "Duplicated tag " << newEntity->getTag() <<endl;
             }
		status = IO_ERROR;
		return;
   }
  else
    data_[index] =  dynamic_cast< Entity*> (newEntity);
 //   if (data_[index] == 0) cout << "Cast error\n";
// cout <<index<<"                "<< data_[index]->print()<<" Added "<<endl;
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
  if(newSize <= dimensions_ )
      return;
  data_.resize(newSize);
  emptyPlaces_ = emptyPlaces_ + (newSize - dimensions_);
//  cout << dimensions_<< " - Reallocating memory to " <<newSize<<endl;// for test
  dimensions_ = newSize;

}

/** This function checks that given string may be tag for this type of data */

/** Note that for temporary names of new entities it will  false */
bool BasicEntitiesCollection::checkDataType(const string &tag)
{
    if (prefix_ == 0)
    {
        return true; // check disabled
    }

    if (!isalpha(tag[0]))
    {
        return false;
    }
    if ((tag[0] & 0x1F) == (prefix_ & 0x1F))// after throwing away all
        //non-alpha  symbols this works as case-nonsensitive comparison of symbols
        return true;
    else
        return false;
}

/** Note: this method can also find Entity by it's temporary name */
long BasicEntitiesCollection::getIndex(const string &tag, bool errorReportEnabled)
{
    long int index = extractIndex(tag, errorReportEnabled);

    if (index >= dimensions_)
    {
        cerr << "Tag " << tag << " is invalid (too big index)" << endl;
        status = IO_ERROR;
        return 0;
    }
    else
        return index;
}

/* this function doesn't check array dimensions */
long BasicEntitiesCollection::extractIndex(const string &tag, bool errorReportEnabled)
{
    int i;
    int prefixLen = 0;
    long int index;
    errorReportEnabled = true;
    status = OK;

    if (prefix_ != 0)
    {
        if (!isalpha(tag[0]))
        {
            if (errorReportEnabled)
                cerr << "Tag [" << tag << "] is invalid (non-alphabetical prefix) in " << getStorageName() << ":" << getStorageLocator() << "[" << getCollectionKeyword() << "]" << endl;
            status = IO_ERROR;
            return 0;
        }
        if ((tag[0] & 0x1F) != (prefix_ & 0x1F))// after throwing away all
            //non-alpha  symbols this works as case-nonsensitive comparison of symbols
        {
            //				cout << "Tag "<< tag << " is invalid (wrong prefix: '" <<prefix_<<"' expected)\n";
            status = IO_ERROR;
            return 0;
        }
        prefixLen = 1;
    }

    //  if ( ! ciStringCompareN(tag, gameConfig.getNewEntityPrefix(),gameConfig.getNewUnitPrefixSize() ) )
    //   {
    //     // New Unit tag
    //
    //    }

    for (i = 0; i < Parser::INTEGER_LENGTH; i++)
    {
        if (!isdigit(tag[i + prefixLen])) // Non-digit was found
            break;

        if (!tag[i + prefixLen]) // End of string
            break;
    }

    if (i == 0) // No digits were found.
    {
        if (errorReportEnabled)
            cerr << "Tag [" << tag << "] is invalid (no  digits) in " << getStorageName() << ":" << getStorageLocator() << "[" << getCollectionKeyword() << "]" << endl;
        // cerr << "Tag [" << tag << "] in collection " << this->getCollectionKeyword()<< " is invalid (no  digits)" <<endl;
        this->status = IO_ERROR;
        return 0;
    }
    if (i >= Parser::INTEGER_LENGTH) // More than maximum of digits were found.
    {
        cerr << "Tag [" << tag << "] is invalid (too many digits) in " << getStorageName() << ":" << getStorageLocator() << "[" << getCollectionKeyword() << "]" << endl;
        //cerr << "Tag " << tag << " is invalid (too many digits)" <<endl;
        status = IO_ERROR;
        return 0;
    }

    index = atoi(tag.c_str() + prefixLen);

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


char  BasicEntitiesCollection:: getEntityTagPrefix ()
{
	return prefix_;
}



string BasicEntitiesCollection::tagFromIndex(long int index)
{
		if(prefix_ == 0)
    {
      return collectionKeyword_.substr(0,1) + longtostr(index);
    }
		else
    {
      return string(1,prefix_) + longtostr(index);
    }
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
  if(static_cast<unsigned int>(i) < RIP_.size())
   return RIP_[i];
 cerr << "Error: RIP Array index (" << i << ") is out of array dimensions!\n";
 return 0 ;
}



NewEntityPlaceholder * BasicEntitiesCollection::findOrAddPlaceholder(const string &tag)
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



NewEntityPlaceholder * BasicEntitiesCollection::checkPlaceholder(const string &tag)
{
 vector <NewEntityPlaceholder *>::iterator iter;
 for (iter = newEntities_.begin(); iter != newEntities_.end(); iter++)
   {
      if ((*iter) ->getName() == tag)
       return (*iter);
   }
 return 0;
}



void BasicEntitiesCollection::clear()
{
 for (EntitiesIterator iter = begin(); iter != end(); iter++)
   {
      if(*iter == 0)
          continue;
      delete (*iter);
   }
 data_.clear();
 RIP_.clear();
 for (vector <NewEntityPlaceholder *>::iterator iter = newEntities_.begin();
          iter != newEntities_.end(); iter++)
   {
      if(*iter == 0)
          continue;
      delete (*iter);
   }
 newEntities_.clear();
}
