/***************************************************************************
                          OrderPrototypesCollection.h 
                    collection supporting fast search by keyword.
                             -------------------
    begin                : Tue Nov  5 11:46:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef ORDER_PROTOTYPES_COLLECTION_H
#define ORDER_PROTOTYPES_COLLECTION_H
#include <vector>
#include <string>
using namespace std;
class OrderPrototype;


class OrderPrototypesCollection {
    public:
       OrderPrototypesCollection();
       ~OrderPrototypesCollection();
 OrderPrototype * find(const string & keyword);
 void add(OrderPrototype * prototype);
    protected:
 vector <OrderPrototype *> prototypes_;
 
    private:
  
};
#endif
