/***************************************************************************
                          DataHandler.h  

                             -------------------
    begin                : Wen May 22 13:52:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H
#include "FileParser.h"
#include "BasicCollection.h"
#include "OverlordTypes.h"

class PrototypeManager;

class DataHandler {
public:
  DataHandler( BasicCollection &newCollection,
			   const char * filename);
  ~DataHandler();
  STATUS load();
  STATUS save();
  STATUS initialize();
  void print();
protected:
private:
  FileParser * parser_;
  string filename_;
  BasicCollection * collection_;
  string  collectionKeyword_;
  unsigned long  collectionSize_;
  unsigned long  beginning_;
};

#endif
