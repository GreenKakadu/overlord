/** ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
*************************************************************************
 OrderLine.cpp . -------------------
 begin                : Tue Nov  5 11:46:00 IST 2002
 copyright            : (C) 2002 by Alex Dribin
 email                : Alex.Dribin@gmail.com
fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff */
#include "OrderLine.h"
#include "Entity.h"
#include "OrderPrototype.h"
#include "OrderPrototypesCollection.h"
#include "UnitEntity.h" //For Debugging only
extern int currentDay;
extern bool testMode;



OrderLine::OrderLine( const string & order, Entity * entity )
{
  //    cout << entity->print()<<" New Order created: " << order<<endl;
  //  isParsed = false;
  Parser * parser = new Parser( order );
  executedOnDay_ = 0;
  isCompleted_ = false;
  dayRestricted_ = 0;
  state_ = NORMAL_STATE;
  whileCondition_ = false;
  ifConditionLevel = 0;
  reportFlags = 0;
  isPermanent_ = false;
  repetitionCounter_ = 0;
  parseModifiers( parser );
  parse( parser, entity );
  comment_ = parser->getText();
	ifStatement_ = false;
	elseStatement_ = false;
	endifStatement_ = false;
  ifStatementLevel = 0;
  elseStatementLevel = 0;
  delete parser;
}

const UINT OrderLine::NO_NORMAL_REPORT_FLAG = 0x01;
const UINT OrderLine::NO_ERROR_REPORT_FLAG = 0x02;



bool OrderLine::getCompletionFlag() const
{
  return isCompleted_;
}



OrderLine::~OrderLine()
{
  //#ifdef TEST_MODE
 //    /*if(testMode) */ cout << "Order deleted" << endl;
  //#endif

//   for ( vector < AbstractData * >::iterator iterator = parameters_.begin();
//        iterator != parameters_.end(); ++iterator )
//        {
//          ( * iterator )->clean();
//   }

}



void OrderLine::parseModifiers( Parser * parser )
{
  bool isParsing = true;

  while ( isParsing )
  {
    if ( parser->matchChar( PERMANENT_ORDER_SYMBOL ) )
    {
      isPermanent_ = true;
      isParsing = true;
      continue;
    }
    if ( parser->matchChar( DAY_SPECIFIC_ORDER_SYMBOL ) )
    {
      if ( parser->matchInteger() )
      {
        dayRestricted_ = parser->getInteger();
        isParsing = true;
        continue;
      }
      isParsing = false;
      /* D without number */
      parser->rewind( -1 );
      continue;
    }
    if ( parser->matchChar( IF_CONDITION_SYMBOL ) )
    {
      ifConditionLevel++;
      isParsing = true;
      continue;
    }
    if ( parser->matchChar( WHILE_CONDITION_SYMBOL ) )
    {
      whileCondition_ = true;
      isParsing = true;
      continue;
    }
    if ( parser->matchChar( NO_ERROR_REPORT_SYMBOL ) )
    {
      reportFlags |= NO_ERROR_REPORT_FLAG;
      isParsing = true;
      continue;
    }
    if ( parser->matchChar( NO_NORMAL_REPORT_SYMBOL ) )
    {
      reportFlags |= NO_NORMAL_REPORT_FLAG;
      isParsing = true;
      continue;
    }
    if ( parser->matchInteger() )
    {
      repetitionCounter_ = parser->getInteger();
      if ( repetitionCounter_ > 1 )
      {
        isParsing = true;
        continue;
      }
    }
    if ( parser->matchKeyword( "if" ) )
    {
      isParsing = true;
			ifStatement_ = true;
      continue;
    }
    if ( parser->matchKeyword( "else" ) )
    {
      isParsing = true;
			elseStatement_ = true;
      continue;
    }
    if ( parser->matchKeyword( "endif" ) )
    {
      isParsing = true;
			endifStatement_ = true;
      continue;
    }
    isParsing = false;
  }
}



void OrderLine::stripModifiers(Parser * parser )
{
  bool isParsing = true;
  int repetitionCounter;

  while ( isParsing )
  {
    if ( parser->matchChar( PERMANENT_ORDER_SYMBOL ) )
    {
      isParsing = true;
      continue;
    }
    if ( parser->matchChar( DAY_SPECIFIC_ORDER_SYMBOL ) )
    {
      if ( parser->matchInteger() )
      {
        isParsing = true;
        continue;
      }
      isParsing = false;
      /* D without number */
      parser->rewind( -1 );
      continue;
    }
    if ( parser->matchChar( IF_CONDITION_SYMBOL ) )
    {
      isParsing = true;
      continue;
    }
    if ( parser->matchChar( WHILE_CONDITION_SYMBOL ) )
    {
      isParsing = true;
      continue;
    }
    if ( parser->matchChar( NO_ERROR_REPORT_SYMBOL ) )
    {
      isParsing = true;
      continue;
    }
    if ( parser->matchChar( NO_NORMAL_REPORT_SYMBOL ) )
    {
      isParsing = true;
      continue;
    }
    if ( parser->matchInteger() )
    {
      repetitionCounter = parser->getInteger();
      if ( repetitionCounter > 1 )
      {
        isParsing = true;
        continue;
      }
    }
    isParsing = false;
  }
}



bool OrderLine::parse( Parser * parser, Entity * entity )
{
  string tempKeyword = parser->getWord();
// If keyword is "combat" we would like to insert  order
  orderPrototype_ = orderPrototypesCollection->find( tempKeyword );
//cout << " Order ->"<<orderPrototype_->getKeyword()<<endl;
  if ( orderPrototype_ == 0 )
  {
		if(elseStatement_ || endifStatement_)
         return true;
		else
		cerr << "=xx= Parsing failed for order " << tempKeyword << " "
         << parser->getText() << endl;
    return false;
  }
  
  else if ( orderPrototype_->
       loadParameters( parser, parameters_, entity ) == OK )
         return true;
  else
  {
    orderPrototype_ = 0;
    return false;
  }

}



ORDER_STATUS OrderLine::process( ProcessingMode * processingMode,
     Entity * entity )
     {
       ORDER_STATUS result;

//        if(entity->isTraced())                                  //For Debugging only
//          {                                                      //For Debugging only
//            cout <<"==== Trying to process "; printOrderLine(cout); //For Debugging only
//          }                                                     //For Debugging only


#ifdef TEST_MODE
       if ( testMode )
       {
         cout << "==== Trying to process ";
         save( cout );
       }
     #endif

       if ( orderPrototype_ == 0 )
		{
		if(elseStatement_ || endifStatement_)
         		return FAILURE;
					else
         		return INVALID;
		}
       if ( executedOnDay_ == currentDay )
       {
     #ifdef TEST_MODE
         if ( testMode )
           cout << "==== Was already executed on this day" << endl;
     #endif

/*           if(entity->isTraced())                                  //For Debugging only
           {                                                      //For Debugging only
             cout <<"==== Was already executed on this day" << endl;  //For Debugging only
           }  */                                                   //For Debugging only

         return FAILURE;
       }

       if ( entity->isFullDayOrderFlagSet() )
       {
     #ifdef TEST_MODE
         if ( testMode )
           cout << "==== Full day order Was already executed on this day"
                << endl;
     #endif
         return FAILURE;
       }
       if ( !orderPrototype_->mayBeProcessed( processingMode, entity ) )
       {
     #ifdef TEST_MODE
         if ( testMode )
           cout << "==== Order can't be processed duiring this mode" << endl;
     #endif

/*           if(entity->isTraced())                                  //For Debugging only
           {                                                      //For Debugging only
             cout <<"==== Order can't be processed duiring this mode "<< endl;  //For Debugging only
           }                                                     //For Debugging only
    
 */        return FAILURE;
       }

       if ( ( dayRestricted_ == 0 ) || ( dayRestricted_ == currentDay ) )
       {
         entity->setCurrentOrder( this );
         result = orderPrototype_->process( entity, parameters_ );
         entity->setCurrentOrder( 0 );
       }
       else
         return FAILURE;
       if ( ( result == SUCCESS ) || ( result == IN_PROGRESS ) )
         executedOnDay_ = currentDay;

       if ( ( result == FAILURE ) && isFullDayOrder() ) // This order was already checked
              executedOnDay_ = currentDay;

       return result; 
}



ORDER_STATUS OrderLine::completeProcessing( Entity * entity, int result )
{
  entity->setCurrentOrder( this );
  ORDER_STATUS status =
       orderPrototype_->completeOrderProcessing( entity, parameters_, result );
  entity->setCurrentOrder( 0 );
  return status;
}



void OrderLine::save( ostream & out )
{
  out << "ORDER ";
  printOrderLine( out );
}



void OrderLine::printOrderLine( ostream & out )
{
  vector < AbstractData * >::const_iterator iterator2;
  int i;
  if ( dayRestricted_ != 0 )
    out << DAY_SPECIFIC_ORDER_SYMBOL << dayRestricted_ << " ";
  if ( isPermanent_ ) out << PERMANENT_ORDER_SYMBOL;
  for ( i = 0; i < ifConditionLevel; i++ )
  {
    out << IF_CONDITION_SYMBOL;
  }
  if ( whileCondition_ ) out << WHILE_CONDITION_SYMBOL;

  if ( repetitionCounter_ > 1 ) out << repetitionCounter_;
  if ( reportFlags & NO_NORMAL_REPORT_FLAG ) out << NO_NORMAL_REPORT_SYMBOL;
  if ( reportFlags & NO_ERROR_REPORT_FLAG ) out << NO_ERROR_REPORT_SYMBOL;

  if(ifStatement_) out << "IF ";
  if(elseStatement_) out << "ELSE ";
  if(endifStatement_) out << "ENDIF ";

  if ( orderPrototype_ != 0 )
  {
    orderPrototype_->save( out );
    for ( iterator2 = parameters_.begin(); iterator2 != parameters_.end();
         iterator2++ )
         {
//		out<<" ";
           ( * iterator2 )->saveAsParameter( out );
    }
  }

  out << comment_ << endl;
}



bool OrderLine::isFullDayOrder()
{
  return orderPrototype_->isFullDayOrder();

}



void OrderLine::setReportingFlag( UINT flag )
{
  reportFlags |= translate_( flag );
}



void OrderLine::clearReportingFlag( UINT flag )
{
  reportFlags &= translate_( ~flag );

}



bool OrderLine::getReportingFlag( UINT flag )
{
  return ( reportFlags & translate_( flag ) );
}



// reportFlags  - is a bitmap where first two bits are
// NO_NORMAL_REPORT_FLAG and NO_ERROR_REPORT_FLAG
// all other bits may be used be used by order objects




UINT OrderLine::translate_( UINT flag )
{
  return flag << 2; // 2 internal flags are already defined
}



PROCESSING_STATE OrderLine::getProcessingState() const
{
  return state_;
}



void OrderLine::setProcessingState( PROCESSING_STATE state )
{
  state_ = state;
}
