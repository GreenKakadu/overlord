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

class PrototypeManager;

class DataHandler {
public:
  DataHandler( BasicCollection &newCollection,
			   const char * filename);
  ~DataHandler();
  int load();
  int save();
  int initialize();
  void print();
protected:
private:
  FileParser * parser_;
  BasicCollection * collection_;
  string  collectionKeyword_;
  int beginning_;
};   
#endif
