/***************************************************************************
                          OrderPrototype.h
     Generic order object               .
                             -------------------
    begin                : Wen Aug  7 13:28:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#ifndef ORDER_PROTOTYPE_H
#define ORDER_PROTOTYPE_H
#include <vector>
#include <string>
#include <iostream>

#include "LineParser.h"
#include "SystemTypes.h"
#include "OverlordTypes.h"
#include "ProcessingMode.h"
#include "BasicCollection.h"
#include "NewEntityPlaceholder.h"
using namespace std;
enum  parsing_mode {
  NORMAL_PARSING      = 0,
  NO_PARSING_REPORT   = 1,
  PARSING_MODE_END    = 10
                };
typedef enum parsing_mode PARSING_MODE;
typedef vector <AbstractData *> ParameterList;

class Entity;
class UnitEntity;
//class Order;
class AbstractData;
class ReportPattern;

class OrderPrototype
{
    public:
        OrderPrototype();
        virtual ~OrderPrototype(){}

           virtual STATUS loadParameters(Parser * parser,
		ParameterList &parameters, Entity * entity );
                   STATUS save( ostream &out);
	   virtual ORDER_STATUS process (Entity * entity, ParameterList &parameters);
		ORDER_TYPE getOrderType() const {return orderType_;}
// some orders can't be processed immediatelly. Instead of that they are submiting
// requests that are resolved later. completeOrderProcessing  is a second part of
// order processing for such request-submitting orders. It is called from
// conflict resolution, when it is over.
     virtual ORDER_STATUS completeOrderProcessing (Entity * entity,
		 		ParameterList &parameters, int result);
	inline string getKeyword() const {return keyword_;}
    bool isFullDayOrder();
    bool mayInterrupt();
    bool mayBeProcessed(ProcessingMode * processingMode, Entity * entity);
		virtual inline int getInitiative()const {return initiative_;}//
	  inline bool isSequentive(){return isSequentive_;}//
		virtual bool evaluate(Entity * , ParameterList  &){return true;}//

    protected:
    bool entityIsUnit(Entity *entity, PARSING_MODE mode = NORMAL_PARSING);
    bool entityIsFaction(Entity *entity, PARSING_MODE mode = NORMAL_PARSING);
    bool entityIsTokenEntity(Entity *entity,PARSING_MODE mode = NORMAL_PARSING);
    bool parseGameDataParameter(Entity *entity, Parser * parser,
		    BasicCollection & collection, const string & parameterTypeName,
		    ParameterList &parameters);
    bool parseGameDataParameter(Entity *entity, const string & tag,
					 BasicCollection & collection, const string & parameterTypeName,
					 ParameterList &parameters);
    bool parseIntegerParameter(Parser * parser, ParameterList &parameters);
    bool parseStringParameter(Entity *entity, Parser * parser, ParameterList &parameters);
    bool parseOptionalStringParameter(Entity *entity, Parser * parser,
						ParameterList &parameters, const char * stringParameter);
    bool parseOptionalGameDataParameter(Entity *entity, Parser * parser,
          BasicCollection & collection, ParameterList &parameters);
    bool checkParameterTag(Entity *entity,const string & tag,
              		BasicCollection & collection,
			ParameterList &parameters);
    bool checkRuleParameterTag(Entity *entity, const string & tag,
                                               BasicCollection & collection, ParameterList &parameters);
    int getIntegerParameter(ParameterList &parameters, unsigned int parIndex);
      string   keyword_;
      string   description;
      ORDER_TYPE orderType_;
      bool mayInterrupt_;
      bool fullDayOrder_;

    protected:
       bool registerOrder_();
	int initiative_; //
	bool isSequentive_;//
    private:
};
enum report_type {
  INVALID_ORDER    = 0,
  NOT_ACCEPTING    = 1,
  CANT_INTERRACT   = 2,
  DONE             = 3,
  REPORT_TYPE_END  = 4
                };

typedef enum report_type REPORT_TYPE;

#define CLEAR_BUFFER(buffer)  buffer.freeze(false); buffer.seekp(0,ios::beg);


#endif



