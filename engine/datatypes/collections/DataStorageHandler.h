/***************************************************************************
                          DataStorageHandler.h  
	DataStorageHandler provides independence of data handling from the data
	type and the way how these data actually stored and accessed.
	(for instance they may be stored in files or in database).
	Each DataStorageHandler handles one Collection.
                             -------------------
    begin                : Wen May 22 13:52:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H
#include "FileParser.h"
#include "BasicCollection.h"
//#include "SystemTypes.h"
#include "OverlordTypes.h"

class ProcessingMode;

class DataStorageHandler {
public:
  DataStorageHandler(const char * filename);
  DataStorageHandler(string * filename);
  ~DataStorageHandler();
  void setCollection(BasicCollection  *collection);
  STATUS open();
  STATUS load();
  STATUS save();
  STATUS initializeData();
  void print();
   STATUS status;
protected:
  FileParser * parser_;
  string * filename_;
  string  filenameString_;
  BasicCollection * collection_;
  string  collectionKeyword_;
   long int collectionSize_;
  long int  beginning_;
private:
};

#endif