/***************************************************************************
                          DataManipulator.h  
   Top-level data manipulator, encapsulating all data details.
                             -------------------
    begin                : Wen May 22 13:52:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef DATA_MANIPULATOR_H
#define DATA_MANIPULATOR_H
#include  <vector>
class BasicEntitiesCollection;
class BasicRulesCollection;
class BasicVarietiesCollection;
class ProcessingMode;
class NewEntityPlaceholder;
class GameData;

class DataManipulator {
public:
  DataManipulator();
  ~DataManipulator();
  STATUS load();
  STATUS save();
  void clear();
  void printAllData();
  STATUS initialize(); 
  void addEntities( BasicEntitiesCollection * collection);
  void addVarieties( BasicVarietiesCollection * collection);
  void processOrders(ProcessingMode * processingMode);
  void processCombat();
  void processCompetitiveRequests(ProcessingMode * processingMode);
  STATUS checkOrders();
  //void saveOrders();
  void prepareData();
  void dailyUpdate();
  void dailyPreProcessData();
  void turnPostProcessing();
  void turnPreProcessing();
  GameData * findGameData(const string & tag);
  NewEntityPlaceholder * findOrAddPlaceholder(const string & tag);
protected:
private:
  vector < BasicEntitiesCollection  * > entities_;
  vector < BasicVarietiesCollection  *>  varieties_;
};
typedef vector < BasicRulesCollection  *>::iterator RulesCollectionIterator;
typedef vector < BasicEntitiesCollection  *>::iterator EntitiesCollectionIterator;
typedef vector < BasicVarietiesCollection  *>::iterator VarietiesCollectionIterator;
#endif
