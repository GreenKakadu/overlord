/***************************************************************************
                          BasicVarietiesCollection.cpp
              Basic Collection of Polymorphic Persistent objects
                             -------------------
    begin                : Mon Jun 10 12:24:42 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include <time.h>
#include "BasicVarietiesCollection.h"
#include "Variety.h"
#include "DataStorageHandler.h"
extern bool ciStringCompare(const string& s1,const string& s2);
BasicVarietiesCollection::~BasicVarietiesCollection()
{
  // delete handler_; // <- check
}


GameData* BasicVarietiesCollection::findByTag (const string &tag, bool errorReportEnabled)
{
VarietiesIterator iter;
 for (iter = begin(); iter != end(); iter++)
   {
      if (!ciStringCompare((*iter) ->getTag() , tag))
       return  (*iter);
   }

  if(errorReportEnabled)
 cerr << "Error: Tag (" << tag  << ") was not found in "<<collectionKeyword_<<"-s collection\n";
 return 0 ;
}
GameData* BasicVarietiesCollection::findByIndex (const  long int index, bool errorReportEnabled)
{
	if ( index <= size())
         return data_[index];
  if(errorReportEnabled)
 cerr << "Error: Array index (" << index << ") is out of array dimensions!\n";
 return 0 ;
}

void   BasicVarietiesCollection::add  ( GameData * /*const*/ newVariety)
{
  data_.push_back ( dynamic_cast< Variety*> (newVariety) );
//  vector <Variety *>::iterator iter = data_.end() - 1;
//  if (*iter) cout <<" inside " << (*iter)-> printName()  <<endl;
}




bool BasicVarietiesCollection::isValidTag (const string &tag) //const
{
VarietiesIterator iter;
 for (iter = begin(); iter != end(); iter++)
   {
      if ((*iter) ->getTag() == tag)
       return true;

   }
 return false;
}



long int  BasicVarietiesCollection::getIndex (const string &tag)
{
VarietiesIterator iter;
 long int i=0;

 for (iter = begin(); iter != end(); iter++)
   {
      if ((*iter) ->getTag() == tag)
       return i;
      i++;
   }
 return i-1;

}


