/***************************************************************************
                          Entity.h  
                             -------------------
    begin                : Wen jul  15:21:12 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef ENTITY_H
#define ENTITY_H
#include <iostream>
#include "GameData.h"
#include "OrderLine.h"
#include "OverlordTypes.h"
#include "assert.h"
//#include "SkillElement.h"
//#include "ReportElement.h"
using namespace std;

class Order;
class ReportRecord;
class ReportElement;
class ReportPattern;
class BasicCondition;
class SkillRule;
class TeachingOffer;
class FactionEntity;
class Rule;

//class InventoryElement;
//class ItemRule;
extern int currentDay;

class Entity : public GameData
{
    public:
      Entity (const string & keyword, GameData * parent ) : GameData(keyword, parent){}
      Entity ( const Entity * prototype ): GameData(prototype){fullDayOrder_ = 0;}
   virtual    ~Entity ();
   virtual     STATUS  initialize      ( Parser *parser ); // temporary: may be virtual?
   virtual     void      save (ostream &out);
   virtual     void      loadOrders ();
           GameData *    createInstanceOfSelf();
   virtual     void      print();
   virtual     bool      process(ProcessingMode * processingMode);
   virtual     bool      processOrderResults(ORDER_STATUS result,
                                    vector<Order *>::iterator & currentIterator);
   virtual     bool      updateOrderResults(ORDER_STATUS result, Order * OrderId);
   virtual STATUS        prepareData();
   virtual     void      preprocessData();
   virtual     void      dailyPreProcess();
   virtual     void      dailyUpdate();
   virtual     void 	   postProcess(ORDER_STATUS result,
		                                            vector<Order *>::iterator  iter);
  /** prints  report for Entity (stats, posessions, private events) */
  virtual      void      report(FactionEntity * faction, ostream &out);
  virtual      bool      defaultAction();
  /** No descriptions */
  virtual void addOrder(Order * newOrder);
  /** No descriptions */
  virtual void clearOrders();
//  virtual void prepareOrders();
  /** No descriptions */
  virtual void addReport(ReportRecord * report);
  virtual void addReport(ReportPattern * report,Order *  orderId = 0, BasicCondition * observationCriteria = 0 );
  /** No descriptions */
  /** No descriptions */
  virtual void extractReport(UnitEntity * unit, vector < ReportElement * > & reports);
  /** No descriptions */
  virtual Entity * getReportDestination();
  /** prints list of events related to this Entity */
  virtual void reportEvents(ostream &out);
  inline virtual Order * getLastOrder() const {return lastOrder_;}
  inline virtual Order * setLastOrder() const {return lastOrder_;}

  /** Deletes all unused event messages. */
  virtual void finalizeReports();
  virtual void cleanReports();
  /** No descriptions */
friend  ostream &operator << ( ostream &out, Entity * entity);
  /** No descriptions */
  virtual inline void setFullDayOrderFlag() {fullDayOrder_ = currentDay;}
  virtual inline void clearFullDayOrderFlag() {fullDayOrder_ = 0;}
  virtual inline bool isFullDayOrderFlagSet() const {return (fullDayOrder_ == currentDay);}
  virtual inline bool isUnaccessible()  const {return false;}
  inline bool isSilent() const {return silent_;}
  inline void setSilent (bool value) { silent_ = value;}
  virtual bool mayInterract(UnitEntity * unit);
  virtual void addTeachingOffer(TeachingOffer * offer);
  virtual void clearTeachingOffers();
  virtual int  getSkillLevel(SkillRule  * const skill);
   virtual TeachingOffer * findTeachingOffer(SkillRule  * skill, int level);
   virtual bool checkTeachingConfirmation();
  /** How many seats occupies this entity in the class. Number of entities that can be tought by one teacher determined by this value. */
  virtual int getLearningCapacity();
  virtual  bool teacherRequired(SkillRule * skill);
//===============================  Knowledge ============
  virtual bool addKnowledge(Rule * info);
  virtual bool addSkillKnowledge(SkillRule * knowledge, int level);
    protected:
//      bool isItem_(ItemRule * item);
      Order * lastOrder_; // Last Full-day or movement order
    int fullDayOrder_;
    vector <Order *> orders_;
    vector <ReportRecord *> publicReports_;
    vector <ReportElement *> collectedReports_;
    vector <TeachingOffer  *> teachingOffers_;
    bool silent_;
    private:
};

#endif





