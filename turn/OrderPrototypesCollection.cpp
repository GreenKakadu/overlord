/***************************************************************************
                          OrderPrototypesCollection.cpp 
                    collection supporting fast search by keyword.
                             -------------------
    begin                : Tue Nov  5 11:46:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "OrderPrototypesCollection.h"
OrderPrototypesCollection::OrderPrototypesCollection()
{
}
OrderPrototypesCollection::~OrderPrototypesCollection()
{
}

void
OrderPrototypesCollection::add(OrderPrototype * prototype)
{
  prototypes_.push_back(prototype);
}

OrderPrototype *
OrderPrototypesCollection::find(const string & keyword)
{
  list<OrderPrototype *>::const_iterator iter;
  // in order to make search faster a list may be sorted by alphabet
  // and a table of iterators pointing to each  group of orders,
  // starting from the same letter may be done.
  for (iter=prototypes_.begin(); iter != prototypes_.end(); iter++)
    {
      if ((*iter)->getKeyword() == keyword)
	return (*iter);
    }
  return 0;
}

