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
#include "NewEntityPlaceholder.h"
#include "assert.h"
#include "EnchantmentAttribute.h"
#include "ReportPrinter.h"
#include "ReportRecord.h"
#include "GameConfig.h"
//#include "InternalPropertiesCollection.h"
using namespace std;

class OrderLine;
class ReportElement;
class ReportMessage;
class BasicCondition;
class SkillRule;
class TeachingOffer;
class FactionEntity;
class Rule;
class TokenEntity;
class EnchantmentElement;

extern int currentDay;

class Entity : public GameData
{
    public:
      Entity (const string & keyword, GameData * parent ) : GameData(keyword, parent){}
      Entity ( const Entity * prototype );
   virtual    ~Entity ();
   virtual     STATUS  initialize      ( Parser *parser );
   virtual     void      save (ostream &out);
   virtual     void      loadOrders ();
           GameData *    createInstanceOfSelf();
//               void registerAttribute(BasicAttribute * attribute);
   virtual     bool      process(ProcessingMode * processingMode);
   virtual     bool      updateOrderResults(ORDER_STATUS result);
//   virtual     bool      processOrderResults(ORDER_STATUS result,
//                                    OrderIterator & currentIterator);
//   virtual  void postProcessOrder(ORDER_STATUS result, OrderIterator  iter);
   virtual STATUS        prepareData();
   virtual     void      postProcessData();
   virtual     void      postPostProcessData();
   virtual     void      preprocessData();
   virtual     void      dailyPreProcess();
   virtual     void      dailyUpdate();
   virtual     void      payUpkeep(){}

  /** prints  report for Entity (stats, posessions, private events) */
  virtual      void      produceFactionReport(FactionEntity * faction, ReportPrinter &out);
  virtual      bool      defaultAction();
  virtual void addOrder(string newOrder);
  virtual void clearOrders();
  virtual inline vector < OrderLine*> & getOrderList(){return orders_;}

//  virtual void prepareOrders();
  virtual void addReport(ReportRecord report);
  virtual void addReport(ReportMessage * report,OrderLine *  orderId = 0, BasicCondition * observationCriteria = 0 );
  virtual void extractReport(UnitEntity * unit, vector < ReportElement * > & reports);
  virtual Entity * getReportDestination();
  /** prints list of events related to this Entity */
  virtual void reportEvents(ReportPrinter &out);
  inline virtual OrderLine * getLastOrder() const {return lastOrder_;}
  inline virtual  void   setLastOrder(OrderLine * order) {lastOrder_ = order;}
  inline virtual OrderLine * getCurrentOrder() const {return currentOrder_;}
  inline virtual  void   setCurrentOrder(OrderLine * order) {currentOrder_ = order;}

  /** Deletes all unused event messages. */
  virtual void finalizeReports();
  virtual void cleanCollectedReports();
  virtual void cleanPublicReports();
  virtual inline void setFullDayOrderFlag() {fullDayOrder_ = currentDay;}
  virtual inline void clearFullDayOrderFlag() {fullDayOrder_ = 0;}
  virtual inline bool isFullDayOrderFlagSet() const {return (fullDayOrder_ == currentDay);}
  virtual inline bool isUnaccessible()  const {return false;}
  virtual inline bool isBusy()  const {return false;}
  inline bool isSilent() const {return silent_;}
  inline void setSilent (bool value) { silent_ = value;}
  virtual bool mayInterract(UnitEntity * unit);
  virtual bool mayInterractTokenEntity(TokenEntity * tokenEntity);
  virtual void addTeachingOffer(TeachingOffer * offer);
  virtual void clearTeachingOffers();
  virtual int  getSkillLevel(SkillRule  * const skill);
   virtual TeachingOffer * findTeachingOffer(SkillRule  * skill, int level);
   virtual bool checkTeachingConfirmation();
  /** How many seats occupies this entity in the class. Number of entities that can be tought by one teacher determined by this value. */
  virtual int getLearningCapacity();
  virtual  bool teacherRequired(SkillRule * skill);
   void addEnchantment(EnchantmentElement *enchantment) {enchantments_.add(enchantment); }
   void removeEnchantment(EnchantmentElement *enchantment) {enchantments_.remove(enchantment); }
    EnchantmentAttribute getAllEnchantments() const {return enchantments_; }

  virtual bool isHidden() {return false;}// hidden as staff inside construction
  virtual bool isDisobeying() {return false;}// refuses to follow orders
  virtual void setDisobeying(bool value) {disobeying_ = value;}// refuses to follow orders
//===============================  Knowledge ============
  virtual bool addKnowledge(Rule * info);
  virtual bool addSkillKnowledge(SkillRule * knowledge, int level);
friend  ostream &operator << ( ostream &out, Entity * entity);
    protected:
//     static vector <BasicAttribute *> attributes;
//      bool isItem_(ItemRule * item);
    OrderLine * lastOrder_; // Last Full-day or movement order
    OrderLine * currentOrder_; //  order that is processed now
    int fullDayOrder_;
    vector <OrderLine *> orders_;
    vector <ReportRecord> publicReports_;
    vector <ReportElement *> collectedReports_;
    vector <TeachingOffer  *> teachingOffers_;
//    InternalPropertiesCollection<EffectElement *> effects_;
    EnchantmentAttribute  enchantments_;
    bool silent_;
		bool disobeying_;
    private:
};
// This operation is used for processing order parameters that may be Entity
// or aliases of new entity.
// If parameter is entity it returns ponter to apropriate Entity
// If parameter is aliases of new entity that already exists it returns ponter to this Entity
// If parameter is aliases of new entity that not exists yet it returns 0
template <class T>  T * DOWNCAST_ENTITY(AbstractData * entity)
{
  T *  result = dynamic_cast<T *>(entity);
  if(result)
    return result;
  else
  {
    NewEntityPlaceholder * placeholder = dynamic_cast<NewEntityPlaceholder *>(entity);
    if (placeholder == 0)
      return 0;
   Entity * realEntity = placeholder->getRealEntity();
   if(realEntity == 0)
      return 0;
   return dynamic_cast<T *>(realEntity);
  }
}
extern Entity         sampleEntity;

#endif





