/***************************************************************************
                          Entity.cpp  -  description
                             -------------------
    begin                : Sun Nov 17 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "Entity.h"
#include <algorithm>

Entity::Entity()
{
}

Entity::~Entity()
{
  list<OrderElementNode *>::const_iterator iter;
  for ( iter = orders_.begin(); iter != orders_.end(); iter++)
    {
      // delete (*iter);
    }
}


Entity::Entity(const string & keyword, GameData * parent)
{
    keyword_   = keyword;
    parent_   = parent;
    prototypeManager->addToRegistry(this);
}
Entity::Entity(const Entity * prototype)
{
      parent_ = prototype->getParent();
      keyword_ = prototype->getKeyword();
}


STATUS  Entity::process(int phase, ostream & out)

{
// for Entity
// pass through NodeList and try to execute every order with appropriate phasing
// in the case of success update conditional orders.
  ORDER_STATUS result;
  OrderElementNode * currentOrderNode;
  OrderElementNode * lastValidOrderNode;
  OrderElementNode * nextValidOrderNode;
  OrderElement * currentOrder;
  OrderModifier * currentModifier;

  list<OrderElementNode *>::iterator currentIterator =orders_.begin();

   for (  currentOrderNode = orders_.front();  currentOrderNode !=  orders_.back(); currentOrderNode = currentOrderNode -> next())
    {
    	currentIterator = find(currentIterator,orders_.end(), currentOrderNode);
    	lastValidOrderNode = currentOrderNode;
 	nextValidOrderNode = currentOrderNode -> next();
       result = (currentOrderNode -> getOrderElement())->process(this,cout);
       switch (result)
        {
           case SUCCESS:
           	{
  			currentOrder = currentOrderNode -> getOrderElement();
  			currentModifier = currentOrder -> getTopDurationModifier();
  			
  			if(currentModifier ->getType() ==  COUNTER_CONDITION)
  				 {
  				 	if(!currentModifier -> decreaseCounter()) // counter is reduced to 0 and should be deleted
  				 	{
  				 	  currentOrder -> deleteModifier(currentModifier);
  				 	}
  				 }
  			 else if( currentModifier ->getType()  != PERMANENT_ORDER)
  				{
  				 	currentIterator = orders_.erase(currentIterator);
  				 	lastValidOrderNode = currentOrderNode -> last();
  				 	currentOrderNode -> ~OrderElementNode();
  				 }
  				
  			for ( ; currentIterator != orders_.end(); currentIterator++)
  				{		
  					currentOrder = (*currentIterator) ->   getOrderElement();
  					currentModifier = currentOrder -> getTopConditionModifier();

  					
  					if (currentModifier == 0)
  					{
  						break;
  					}
  					
  					if (currentModifier ->getType()  == IF_CONDITION)
  					{
  						currentOrder -> deleteModifier(currentModifier);
  						currentModifier = currentOrder -> getTopConditionModifier();
  						// Checking next conditional modifier
  						// if it is "+" Order will be deleted in the next "if" (combination "-+" is illegal (?))
  						// otherwise put "else"
  						if (currentModifier == 0)
  						{
  						//if no conditionals left this order should be included into execution list
  							(*currentIterator)  -> setLast(lastValidOrderNode);
  							(*currentIterator)  -> setNext(nextValidOrderNode);
  							
  							if(nextValidOrderNode) nextValidOrderNode -> setLast(*currentIterator);
   							if(lastValidOrderNode)  lastValidOrderNode -> setNext(*currentIterator);
   							
   							lastValidOrderNode  = *currentIterator;							
  						}
  					}
  					
   					if (currentModifier  ->getType() == WHILE_CONDITION) // Delete order (and Node)
  					{
  					 	currentIterator = orders_.erase(currentIterator);
  					 	(*currentIterator)  ->  ~OrderElementNode();
  					}
  						
 				 }
  					
        		break;
             	}
           	case FAILURE:
           		{
           		break;
           		}
           	case INVALID:
           		{
  				currentOrder = currentOrderNode -> getOrderElement();

   				 currentIterator = orders_.erase(currentIterator);
  				lastValidOrderNode = currentOrderNode -> last();
  			 	currentOrderNode -> ~OrderElementNode();

  				
  			for ( ; currentIterator != orders_.end(); currentIterator++)
  				{		
  					currentOrder = (*currentIterator) ->   getOrderElement();
  					currentModifier = currentOrder -> getTopConditionModifier();

  					
  					if (currentModifier == 0)
  					{
  						break;
  					}
  					
  					if (currentModifier  ->getType() == WHILE_CONDITION)
  					{
  						currentOrder -> deleteModifier(currentModifier);
  						currentModifier = currentOrder -> getTopConditionModifier();
  						// Checking next conditional modifier
  						// if any exists delete order
  						if (currentModifier == 0)
  						{
  						//if no conditionals left this order should be included into execution list
  							(*currentIterator)  -> setLast(lastValidOrderNode);
  							(*currentIterator)  -> setNext(nextValidOrderNode);
  							
  							if(nextValidOrderNode) nextValidOrderNode -> setLast(*currentIterator);
   							if(lastValidOrderNode)  lastValidOrderNode -> setNext(*currentIterator);
   							
   							lastValidOrderNode  = *currentIterator;							
  						}
  						else  // Delete order (and Node)
  						{
 					 	currentIterator = orders_.erase(currentIterator);
  					 	(*currentIterator)  ->  ~OrderElementNode();
  						}
  					}
  					
   					if (currentModifier  ->getType() == IF_CONDITION ) // Delete order (and Node)
  					{
  					 	currentIterator = orders_.erase(currentIterator);
  					 	(*currentIterator)  ->  ~OrderElementNode();
  					}
  						
 				 }
  					
        		break;
           		}
           		//default:
           	}
    }

return OK;
}
GameData * Entity::createInstanceOfSelf()
{
   return CREATE_INSTANCE<Entity> (this);
}

STATUS
Entity::initialize        ( Parser *parser )
{

	
      if (parser->matchKeyword ("NAME") )
        {
	  setName(parser->getText());
	  return OK;
	}
      if (parser->matchKeyword("DESCRIPTION"))
	{
	  setDescription(parser->getText());
	  return OK;
	}
       if (parser->matchKeyword("ORDER"))
 	{
	  //          cout << "Reading order..." << endl;
	  orders_.push_back(new OrderElementNode(new OrderElement(parser->getText())));
 	}

	  return OK;

}
void
Entity::save(ostream &out)
{
  out << keyword_ << " " <<tag_ << endl;
  if(name_.size()) out << "NAME " <<name_ << endl;
  if(description_.size()) out << "DESCRIPTION " <<description_  << endl;
  out << endl;
  list<OrderElementNode *>::const_iterator iter;
  for ( iter = orders_.begin(); iter != orders_.end(); iter++)
    {
           ((*iter)->getOrderElement())->save(out);
    }
}


void  Entity::load(iostream & in)

{
// for Entity
// pass through NodeList and try to execute every order with appropriate phasing
// in the case of success update conditional orders.

  list<OrderElementNode *>::const_iterator iter;
  for ( iter = orders_.begin(); iter !=  orders_.end(); iter++)
    {
           ((*iter)->getOrderElement())->process(this,cout);
    }


}

void Entity::print() // For debugging
{
    cout  << getName();
    cout << " [" << getTag()  << "] "<< endl;

  list<OrderElementNode *>::const_iterator iter;
  for ( iter = orders_.begin(); iter != orders_.end(); iter++)
    {
           ((*iter)->getOrderElement())->save(cout);
    }

}




