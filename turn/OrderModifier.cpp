/***************************************************************************
                          OrderModifier.cpp
                    .
                             -------------------
    begin                : Tue Nov  5 11:46:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#include "OrderModifier.h"
#include "Symbols.h"

OrderModifier::OrderModifier( MODIFIER_TYPE  type, int  value)
{
  type_ = type;
  counter_ = value;
}

OrderModifier::~OrderModifier( )
{
}

MODIFIER_TYPE
OrderModifier::getType() const
{
  return type_;
}
int
OrderModifier::getCounter() const
{
  return counter_;
}
bool
OrderModifier::decreaseCounter()
{
  counter_ --;
  if (counter_ )
    return true;
  else
    return false;
}


void
OrderModifier::save(ostream &out)
{
  switch (type_)
    {
    case PERMANENT_ORDER:
      {
	out << PERMANENT_ORDER_SYMBOL;
	break;
      }
    case DAY_SPECIFIC_ORDER:
      {
	out << DAY_SPECIFIC_ORDER_SYMBOL << counter_;
	break;
      }
    case IF_CONDITION:
      {
	out << IF_CONDITION_SYMBOL;
	break;
      }
    case WHILE_CONDITION:
      {
	out << WHILE_CONDITION_SYMBOL;
	break;
      }
    case COUNTER_CONDITION:
      {
	out << counter_;
	break;
      }
    case NO_ERROR_REPORT:
      {
	out << NO_ERROR_REPORT_SYMBOL;
	break;
      }
    case NO_NORMAL_REPORT:
      {
	out << NO_NORMAL_REPORT_SYMBOL;
	break;
      }

    }
}
bool
OrderModifier::isControlModifier() const
{
  switch (type_)
    {
    case PERMANENT_ORDER:
    case DAY_SPECIFIC_ORDER:
    case IF_CONDITION:
    case WHILE_CONDITION:
    case COUNTER_CONDITION:
      return true;
    default:
      return false;
    }
}
bool
OrderModifier::isReportModifier() const
{
  switch (type_)
    {
    case NO_ERROR_REPORT:
    case NO_NORMAL_REPORT:
      return true;
    default:
      return false;
    }
}
 
//============================================================================
//
//   bool   fetchModifier (MODIFIER_TYPE & type, int & value)
//
//  This function is intended for parsing of order line.
//
// It is supposed that input is \n - terminated c-string starting from a set
// of predefined modifiers.  (There may be also some spaces  before modifiers
// or between them).  fetchModifier case-insensitive looks for specified
// modifier at the beginning  of the string.
//
// If any modifier found it shifts input string pointer to the right 
// so that resulting string it points to starts from the next  symbol after
//  modifier. Modifier identified and it's type and value are set. Function in
// this case returns TRUE. 
//
// If no  modifier was  found at the beginning of the string 
// function returns FALSE.
//
//============================================================================
bool
OrderModifier::fetchModifier(Parser * parser, MODIFIER_TYPE & type, int & counter)
{


  counter = 0;
  type = NONE;
  if (parser -> matchChar (PERMANENT_ORDER_SYMBOL) )
    {
      type = PERMANENT_ORDER;
      return true;
    }
  if (parser -> matchChar (DAY_SPECIFIC_ORDER_SYMBOL) )
    {
      if (parser -> matchInteger()  )
	{
	  type = DAY_SPECIFIC_ORDER;
	  counter = parser -> getInteger();
	  return true;
	}
      return false;

    }
  if (parser -> matchChar (IF_CONDITION_SYMBOL) )
    {
      type = IF_CONDITION;
      return true;
    }
  if (parser -> matchChar (WHILE_CONDITION_SYMBOL) )
    {
      type = WHILE_CONDITION;
      return true;
    }
  if (parser -> matchChar (NO_ERROR_REPORT_SYMBOL) )
    {
      type = NO_ERROR_REPORT;
      return true;
    }
  if (parser -> matchChar (NO_NORMAL_REPORT_SYMBOL) )
    {
      type = NO_NORMAL_REPORT;
      return true;
    }
  if (parser -> matchInteger()  )
    {
      type = COUNTER_CONDITION;
      counter = parser -> getInteger();
      return true;
    }
      return false;
}

