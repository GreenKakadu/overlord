/***************************************************************************
                          DataManipulator.h  

                             -------------------
    begin                : Wen May 22 13:52:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef DATA_MANIPULATOR_H
#define DATA_MANIPULATOR_H
#include "BasicCollection.h"
#include "DataHandler.h"

class DataManipulator {
public:
  DataManipulator();
  ~DataManipulator();
  int load();
  int save();
 void print();
  int initialize(); 
  void addRules( BasicCollection &newCollection, const char * filename);
protected:
private:
  list < DataHandler * > rules_;
};


#endif
