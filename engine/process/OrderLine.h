/***************************************************************************
                          Order.h
    Order object with modifiers and parameters.                .
                             -------------------
    begin                : Wen Aug  7 16:08:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef ORDER_H
#define ORDER_H
#include <vector>
#include <string>
#include <vector>
#include <iostream>
//#include "AbstractData.h"
#include "Parser.h"
#include "OverlordTypes.h"
#include "OrderPrototype.h"

using namespace std;

class AbstractData;
class Entity;
class OrderPrototype;


class Order
{
    public:
        Order(const string & order, Entity * entity);
        ~Order();

	ORDER_STATUS process( ProcessingMode * processingMode, Entity * entity, ostream &out);
// some orders can't be processed immediatelly. Instead of that they are submiting
// requests that are resolved later. completeProcessing  is a second part of
// order processing for such request-submitting orders. It is called from
// conflict resolution, when it is over.
	ORDER_STATUS completeProcessing(Entity * entity, int result);
	void save(ostream &out); 
	void parseModifiers(Parser * parser );
	bool parse(Parser * parser, Entity * entity );
//	bool isParsed;
	int ifConditionLevel;
	inline int repetitionCounter() const {return repetitionCounter_;}
	UINT reportFlags;
	inline bool isPermanent() const {return isPermanent_;}
	inline bool whileCondition() const {return  whileCondition_;}
  inline void setWhileCondition(bool value) {whileCondition_ = value;}
  inline void decrementRepetitionCounter(){ repetitionCounter_ --;}
  bool isFullDayOrder();
  PROCESSING_STATE getProcessingState() const;
  void setProcessingState(PROCESSING_STATE state);
 inline bool isNormalReportEnabled() const { return !(reportFlags & NO_NORMAL_REPORT_FLAG);}
  inline bool isErrorReportEnabled() const { return !(reportFlags &  NO_ERROR_REPORT_FLAG);}
         void setReportingFlag(UINT flag);
         void clearReportingFlag(UINT flag);
         bool getReportingFlag(UINT flag);
    protected:
static const  UINT NO_NORMAL_REPORT_FLAG;
static const  UINT NO_ERROR_REPORT_FLAG;
	bool isPermanent_;
	PROCESSING_STATE state_;
  bool whileCondition_;
	int dayRestricted_;
  int executedOnDay_ ;
	int repetitionCounter_;
  string comment_ ;
	OrderPrototype * orderPrototype_;
	vector <AbstractData *> parameters_;
//	Parser * parser_;
  UINT translate_(UINT flag);
     private:

};
#endif







