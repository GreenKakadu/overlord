/***************************************************************************
                          OrderElement.cpp
                    .
                             -------------------
    begin                : Tue Nov  5 11:46:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "OrderElement.h"
extern OrderPrototypesCollection orderPrototypesCollection;
OrderElement::OrderElement(const string & order)
{
  //  cout << "New OrderElement created" << endl;  
  isParsed = false;
  parser_ = new Parser(order);  
}
OrderElement::~OrderElement()
{
 list<OrderModifier *>::const_iterator iterator;
 vector<ParameterHandler *>::const_iterator iterator2;

 delete parser_; 

 for (iterator = controlModifiers_.begin(); iterator != controlModifiers_.end(); iterator++)
   {
     delete *iterator;
   } 
 for (iterator = reportModifiers_.begin(); iterator != reportModifiers_.end(); iterator++)
   {
     delete *iterator;
   }
 if(isParsed)
   {
     for (iterator2 = parameterHandlers_.begin(); iterator2 != parameterHandlers_.end(); iterator2++)
       {
	 delete *iterator2;
       }
   }
}
STATUS
OrderElement::check(ostream &out)
{
  /* check modifiers: error on D without number */

  /* check order keyword:  nonalpha symbols where keyword expected (possible error in modifier */
  /* check order keyword: unknown keyword */

  /* check parameters: wrong parameter type */
  /* check parameters: missing parameter */
  /* check parameters: Rule/Entity doesn't exist <- option */
  /* check parameters: wrong Rule/Entity type */
  return OK;
}


void
OrderElement::load()
{
  MODIFIER_TYPE  type;
  int  value;
  OrderModifier * modifier;
 while (OrderModifier::fetchModifier( parser_ , type, value ))
    {
      modifier = new OrderModifier( type, value);
      if (modifier-> isControlModifier())
	controlModifiers_.push_back(modifier);
      else if (modifier-> isReportModifier())
	reportModifiers_.push_back(modifier);
    }
}

ORDER_STATUS
OrderElement::process(GameData * entity, ostream &out)
{
  if (!isParsed)
    parse();
  return orderPrototype_ -> process(entity, parameterHandlers_,out, reportModifiers_);
}

void
OrderElement::parse()
{
 orderPrototype_ = orderPrototypesCollection.find (parser_ -> getWord()); // add checks
 orderPrototype_ -> load(parser_, parameterHandlers_ );
  isParsed = true;
}
void
OrderElement::save(ostream &out)
{
 list<OrderModifier *>::const_iterator iterator;
 vector<ParameterHandler *>::const_iterator iterator2;
      out << "ORDER ";
 for (iterator = controlModifiers_.begin(); iterator != controlModifiers_.end(); iterator++)
   {
     (*iterator)->save(out);
   } 
 for (iterator = reportModifiers_.begin(); iterator != reportModifiers_.end(); iterator++)
   {
     (*iterator)->save(out);
   }
 out << " ";
  if (isParsed)
    {
      orderPrototype_ -> save(out);
      for (iterator2 = parameterHandlers_.begin(); iterator2 != parameterHandlers_.end(); iterator2++)
	{
	  (*iterator2)->saveParameter(out);
	}
    }
  else
    {
      out << parser_ -> getText();
    }
 out << endl;
}

OrderModifier *
OrderElement::getTopDurationModifier()
{
 list<OrderModifier *>::const_iterator iterator;
 for (iterator = controlModifiers_.begin(); iterator != controlModifiers_.end(); iterator++)
   {
     if ( (*iterator) -> isDurationModifier() )
	return (*iterator);
   }
	return 0;
}

OrderModifier *
OrderElement::getTopConditionModifier()
{
 list<OrderModifier *>::const_iterator iterator;
 for (iterator = controlModifiers_.begin(); iterator != controlModifiers_.end(); iterator++)
   {
     if ( (*iterator)-> isConditionModifier() )
	return (*iterator);
   }
	return 0;
}
void OrderElement::deleteModifier(OrderModifier * orderModifier)
{
 list<OrderModifier *>::iterator iterator;
 for (iterator = controlModifiers_.begin(); iterator != controlModifiers_.end(); iterator++)
   {
     if ( (*iterator) == orderModifier)
     		{
     			controlModifiers_.erase(iterator);
     		}
     	else
     		{
     			cout << "Error on attenpt to delete order modifier in order " << endl;
     			save(cout) ;
     			cout << endl;
     		}
	delete orderModifier;
   }
}
