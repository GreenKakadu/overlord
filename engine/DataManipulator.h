/***************************************************************************
                          DataManipulator.h  
   Top-level data manipulator, encapsulating all data details.
                             -------------------
    begin                : Wen May 22 13:52:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef DATA_MANIPULATOR_H
#define DATA_MANIPULATOR_H
#include  <vector>
//#include "../collections/BasicCollection.h"
//#include "../collections/BasicEntitiesCollection.h"
//#include "../collections/BasicRulesCollection.h"
//#include "../collections/EntitiesCollection.h"
//#include "../collections/RulesCollection.h"
//#include "../collections/OrderPrototypesCollection.h"
//#include "../entities/Entity.h"
//#include "../rules/Rule.h"
//class OrderPrototypesCollection;
class BasicEntitiesCollection;
class BasicRulesCollection;
class BasicVarietiesCollection;
class ProcessingMode;

class DataManipulator {
public:
  DataManipulator();
  ~DataManipulator();
  STATUS load();
  STATUS save();
  void print();
  STATUS initialize(); 
  void addEntities( BasicEntitiesCollection * collection);
  void addVarieties( BasicVarietiesCollection * collection);
  void processOrders(ProcessingMode * processingMode);
  void processCompetitiveRequests(ProcessingMode * processingMode);
  STATUS checkOrders();
  //void saveOrders();
  void prepareData();
  void dailyUpdate();
  void dailyPreProcessData();
  void turnPostProcessing();
  void turnPreProcessing();
protected:
private:
  vector < BasicEntitiesCollection  * > entities_;
  vector < BasicVarietiesCollection  *>  varieties_;
};
typedef vector < BasicRulesCollection  *>::iterator RulesCollectionIterator;
typedef vector < BasicEntitiesCollection  *>::iterator EntitiesCollectionIterator;
typedef vector < BasicVarietiesCollection  *>::iterator VarietiesCollectionIterator;
#endif
