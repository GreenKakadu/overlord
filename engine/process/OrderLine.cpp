/***************************************************************************
                          Order.cpp
                    .
                             -------------------
    begin                : Tue Nov  5 11:46:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "OrderLine.h"
#include "Entity.h"
#include "OrderPrototype.h"
#include "OrderPrototypesCollection.h"
extern OrderPrototypesCollection orderPrototypesCollection;
extern int currentDay;
extern bool testMode;

Order::Order(const string & order, Entity * entity)
{
//    cout << entity->printName()<<" New Order created: " << order<<endl;
//  isParsed = false;
  Parser * parser = new Parser(order);
  executedOnDay_ = 0;
  dayRestricted_ = 0;
  state_ = NORMAL_STATE;
	whileCondition_ = false;
	ifConditionLevel = 0;
	reportFlags = 0;
	isPermanent_ = false;
  repetitionCounter_ = 1;
  parseModifiers(parser);
	parse(parser, entity);
	comment_ = parser->getText();
 delete parser;
}
const UINT Order::NO_NORMAL_REPORT_FLAG = 0x01;
const UINT Order::NO_ERROR_REPORT_FLAG = 0x02;
Order::~Order()
{

 vector<AbstractData *>::const_iterator iterator;

// delete parser_;
#ifdef TEST_MODE
//   if(testMode)  cout << "Order deleted" << endl;
#endif
// if(isParsed)
//  {
     for (iterator = parameters_.begin(); iterator != parameters_.end(); iterator++)
       {
	 //delete *iterator;  Can't delete GameData objects!
       }
//   }
}


void
Order::parseModifiers(Parser * parser )
{
bool isParsing =true;

 while (isParsing)
    {
  		if (parser -> matchChar (PERMANENT_ORDER_SYMBOL) )
    		{
					isPermanent_ = true;
					isParsing = true;
			 		continue;
    		}
  		if (parser -> matchChar (DAY_SPECIFIC_ORDER_SYMBOL) )
    		{
      		if (parser -> matchInteger()  )
						{
	  					dayRestricted_ = parser -> getInteger();
							isParsing = true;
			 				continue;
						}
       	isParsing = false;   /*   D without number */
        parser -> rewind(-1);
 			 	continue;
   			}
  		if (parser -> matchChar (IF_CONDITION_SYMBOL) )
    		{
							ifConditionLevel++;
							isParsing = true;
			 				continue;
    		}
  		if (parser -> matchChar (WHILE_CONDITION_SYMBOL) )
    		{
							whileCondition_ = true;
							isParsing = true;
			 				continue;
    		}
  		if (parser -> matchChar (NO_ERROR_REPORT_SYMBOL) )
    		{
							reportFlags |= NO_ERROR_REPORT_FLAG;
							isParsing = true;
			 				continue;
    		}
  		if (parser -> matchChar (NO_NORMAL_REPORT_SYMBOL) )
    		{
							reportFlags |= NO_NORMAL_REPORT_FLAG;
							isParsing = true;
			 				continue;
    		}
  		if (parser -> matchInteger()  )
    		{
      		repetitionCounter_ = parser -> getInteger();
      		if(repetitionCounter_ > 1)
      		{
							isParsing = true;
			 				continue;
      		}
    		}
		isParsing = false;
	}
}
bool
Order::parse(Parser * parser, Entity * entity )
{
  string tempKeyword = parser -> getWord();
 	orderPrototype_ = orderPrototypesCollection.find (tempKeyword);
	if( orderPrototype_ == 0)
			{
     			cout  << "=xx= Parsing failed for order "<< tempKeyword << parser -> getText()<<endl;
  				return false;
			}
	else
 	 if (orderPrototype_ -> loadParameters(parser, parameters_ ,  entity) == OK)
	 	 return true;
		else
			{
				orderPrototype_ = 0;
  			return false;
			}

 }

ORDER_STATUS
Order::process(ProcessingMode * processingMode, Entity * entity, ostream &out)
{
ORDER_STATUS result;
#ifdef TEST_MODE
   if(testMode)
	{
		cout << "==== Trying to process ";
		save(cout);
	}
#endif

if( orderPrototype_ == 0)
		return INVALID;
	if (executedOnDay_ == currentDay)
	{
#ifdef TEST_MODE
   if(testMode) cout << "==== Was already executed on this day" << endl;
#endif
		return FAILURE;
	}

	if (entity->isFullDayOrderFlagSet())
	{
#ifdef TEST_MODE
   if(testMode) cout << "==== Full day order Was already executed on this day" << endl;
#endif
		return FAILURE;
	}
    if(!orderPrototype_ ->mayBeProcessed(processingMode,entity))
    {
#ifdef TEST_MODE
   if(testMode) cout << "==== Order can't be processed duiring this mode" << endl;
#endif
		return FAILURE;
    }
  
	if((dayRestricted_ == 0) || (dayRestricted_== currentDay))
	result	=orderPrototype_ -> process(entity, parameters_,this);
	 else
		return FAILURE;
	if((result == SUCCESS) ||(result == IN_PROGRESS))
		executedOnDay_ = currentDay;

   if((result == FAILURE) && isFullDayOrder()) // This order was already checked 
		executedOnDay_ = currentDay;

    return result;
}

ORDER_STATUS 
Order::completeProcessing(Entity * entity, int result)
{
  return orderPrototype_ -> completeProcessing(entity, parameters_,this,result);
}
void
Order::save(ostream &out)
{
 vector< AbstractData *>::const_iterator iterator2;
	int i;
      out << "ORDER ";
      if(dayRestricted_ != 0)    out << DAY_SPECIFIC_ORDER_SYMBOL << dayRestricted_<<" ";
      if(isPermanent_)    out << PERMANENT_ORDER_SYMBOL;
      for(i = 0; i< ifConditionLevel; i++)
							{
								out << IF_CONDITION_SYMBOL;
							}
      if(whileCondition_)    out << WHILE_CONDITION_SYMBOL;

      if(repetitionCounter_ > 1)    out << repetitionCounter_;
      if(reportFlags & NO_NORMAL_REPORT_FLAG)    out << NO_NORMAL_REPORT_SYMBOL;
      if(reportFlags & NO_ERROR_REPORT_FLAG)    out << NO_ERROR_REPORT_SYMBOL;


	if( orderPrototype_ != 0)
			{
      	orderPrototype_ -> save(out);
      	for (iterator2 = parameters_.begin(); iterator2 != parameters_.end(); iterator2++)
					{
	  				(*iterator2)->saveAsParameter(out);
					}
			}

      out << comment_ << endl;
}


bool Order::isFullDayOrder()
{
  return orderPrototype_->isFullDayOrder();
  
}

void Order::setReportingFlag(UINT flag)
{
  reportFlags |= translate_(flag);
}
void Order::clearReportingFlag(UINT flag)
{
  reportFlags &= translate_(~flag);
  
}
bool Order::getReportingFlag(UINT flag)
{
  return ( reportFlags &  translate_(flag));
}


// reportFlags  - is a bitmap where first two bits are
// NO_NORMAL_REPORT_FLAG and NO_ERROR_REPORT_FLAG
// all other bits may be used be used by order objects

UINT Order::translate_(UINT flag)
{
  return flag << 2; // 2 internal flags are already defined
}
PROCESSING_STATE Order::getProcessingState() const
{
   return state_;
  }
void Order::setProcessingState(PROCESSING_STATE state)
{
   state_ =  state;
  }
