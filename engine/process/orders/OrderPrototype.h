/***************************************************************************
                          OrderPrototype.h 
     Generic order object               .
                             -------------------
    begin                : Wen Aug  7 13:28:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef ORDER_PROTOTYPE_H
#define ORDER_PROTOTYPE_H
#include <vector>
#include <string>
#include <iostream>

#include "Parser.h"
#include "SystemTypes.h"
#include "OverlordTypes.h"
#include "ProcessingMode.h"
#include "BasicCollection.h"
#include "NewEntityPlaceholder.h"
using namespace std;

class Entity;
class ReportRecord;
class UnitEntity;
class Order;
class AbstractData;
class Reporter;

class OrderPrototype
{
    public:
        OrderPrototype();
        virtual ~OrderPrototype();

           virtual STATUS loadParameters(Parser * parser, vector <AbstractData *>  &parameters, Entity * entity );
                   STATUS save( ostream &out);
	   virtual ORDER_STATUS process (Entity * entity, vector <AbstractData *>  &parameters, Order * orderId);
// some orders can't be processed immediatelly. Instead of that they are submiting
// requests that are resolved later. completeProcessing  is a second part of
// order processing for such request-submitting orders. It is called from
// conflict resolution, when it is over.
     virtual ORDER_STATUS completeProcessing (Entity * entity, vector <AbstractData *>  &parameters, Order * orderId, int result);
	   string getKeyword() const; 
    bool isFullDayOrder();
    bool mayBeProcessed(ProcessingMode * processingMode, Entity * entity);
    protected:
    bool entityIsUnit(Entity *entity) ;
    bool parseGameDataParameter(Entity *entity, Parser * parser, BasicCollection & collection,
          const string & parameterTypeName, vector <AbstractData *>  &parameters);
    bool parseGameDataParameter(Entity *entity, const string & tag, BasicCollection & collection,
          const string & parameterTypeName, vector <AbstractData *>  &parameters);
    bool parseIntegerParameter(Parser * parser, vector <AbstractData *>  &parameters);
    bool parseOptionalGameDataParameter(Entity *entity, Parser * parser,
          BasicCollection & collection, const string & parameterTypeName,
          vector <AbstractData *>  &parameters);
    bool checkParameterTag(Entity *entity,const string & tag,
              BasicCollection & collection, vector <AbstractData *>  &parameters);
    template <class T>  T * DOWNCAST_ENTITY(AbstractData * entity);
    int getIntegerParameter(vector <AbstractData *>  &parameters, int parIndex);
      string   keyword_;
      string   description;
      ORDER_TYPE orderType_;
    private:
};
enum report_type {
  INVALID_ORDER    = 0,
  NOT_ACCEPTING    = 1,
  CANT_INTERRACT   = 2,
  DONE             = 3
                };

typedef enum report_type REPORT_TYPE;

#define CLEAR_BUFFER(buffer)  buffer.freeze(false); buffer.seekp(0,ios::beg);

template <class T>  T * OrderPrototype::DOWNCAST_ENTITY(AbstractData * entity)
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

#endif



