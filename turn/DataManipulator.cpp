/***************************************************************************
                          DataManipulator.cpp  

                             -------------------
    begin                : Wen May 22 13:52:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "DataManipulator.h"

DataManipulator::DataManipulator()
{

}



DataManipulator::~DataManipulator()
{
     cout << "Destructing data handlers... " << endl;
  list<DataHandler *>::const_iterator iter;
 for (iter = rules_.begin(); iter != rules_.end(); iter++)
   {
     delete (*iter);
   } 
 for (iter = entities_.begin(); iter != entities_.end(); iter++)
   {
     delete (*iter);
   } 
}



void DataManipulator::addRules ( BasicCollection &newCollection,
			   const char * filename)
{

 DataHandler * data  = new DataHandler(newCollection, filename);
 rules_.push_back(data);
}



void DataManipulator::addEntities ( BasicCollection &newCollection,
			   const char * filename)
{

 DataHandler * data  = new DataHandler(newCollection, filename);
 entities_.push_back(data);
}



STATUS DataManipulator::load()
{
  list<DataHandler *>::const_iterator iter;
 for (iter = rules_.begin(); iter != rules_.end(); iter++)
   {
      (*iter) ->load();
   } 
 for (iter = entities_.begin(); iter != entities_.end(); iter++)
   {
      (*iter) ->load();
   } 
 return OK;
}
STATUS DataManipulator::save()
{
  list<DataHandler *>::const_iterator iter;
 for (iter = entities_.begin(); iter != entities_.end(); iter++)
   {
      (*iter) ->save();
   } 
 return OK;
}
STATUS DataManipulator::initialize()
{
  list<DataHandler *>::const_iterator iter;
 for (iter = rules_.begin(); iter != rules_.end(); iter++)
   {
      (*iter) ->initialize();
   }

 for (iter = entities_.begin(); iter != entities_.end(); iter++)
   {
      (*iter) ->initialize();
   }
 return OK;
}
void DataManipulator::print()
{
  list<DataHandler *>::const_iterator iter;
 for (iter = rules_.begin(); iter != rules_.end(); iter++)
   {
      (*iter) ->print();
   } 
 for (iter = entities_.begin(); iter != entities_.end(); iter++)
   {
      (*iter) ->print();
   } 
}
