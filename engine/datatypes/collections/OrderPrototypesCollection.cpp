/***************************************************************************
                          OrderPrototypesCollection.cpp 
                    collection supporting fast search by keyword.
                             -------------------
    begin                : Tue Nov  5 11:46:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "OrderPrototypesCollection.h"
#include "OrderPrototype.h"
extern bool ciStringCompare(const string& s1,const string& s2);
OrderPrototypesCollection *  orderPrototypesCollection;

OrderPrototypesCollection::OrderPrototypesCollection()
{
//  cout << "OrderPrototypesCollection created \n";
}
OrderPrototypesCollection::~OrderPrototypesCollection()
{
//  cout << "OrderPrototypesCollection destroyed \n";
//  vector<OrderPrototype *>::iterator iter;
//  for (iter=prototypes_.begin(); iter != prototypes_.end(); iter++)
//    {
//	delete (*iter);
//    }
}



void
OrderPrototypesCollection::add(OrderPrototype * prototype)
{
  prototypes_.push_back(prototype);
}



OrderPrototype *
OrderPrototypesCollection::find(const string & keyword)
{
  vector<OrderPrototype *>::const_iterator iter;
  // in order to make search faster a vector may be sorted by alphabet
  // and a table of iterators pointing to each  group of orders,
  // starting from the same letter may be done.

//  cout << "Loooking for ["<< keyword <<"] in OrderPrototypesCollection ";
  for (iter=prototypes_.begin(); iter != prototypes_.end(); iter++)
    {
      if (!ciStringCompare((*iter)->getKeyword(), keyword))
      {
//          cout << " ... found!\n";
	      return (*iter);
      }
    }
//          cout << " not found!\n";
  /* check order keyword:  nonalpha symbols where keyword expected (possible error in modifier */
  /* check order keyword: unknown keyword */
  return 0;
}

