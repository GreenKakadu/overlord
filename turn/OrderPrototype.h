/***************************************************************************
                          OrderPrototype.h 
                    .
                             -------------------
    begin                : Wen Aug  7 13:28:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef ORDER_PROTOTYPE_H
#define ORDER_PROTOTYPE_H
#include <list>
#include <string>
#include <vector>
#include <iostream>
#include "GameData.h"
#include "Parser.h"
#include "OrderModifier.h"
#include "ParameterHandler.h"


enum order_status {
  SUCCESS = 0,
  FAILURE = 1,
  INVALID = 2
};
typedef enum order_status ORDER_STATUS;
extern int current_day;
class OrderPrototype
{
    public:
        OrderPrototype();
        virtual ~OrderPrototype();
           virtual STATUS check(Parser * parser, vector <ParameterHandler *>  &parameters, ostream &out);
           virtual STATUS load(Parser * parser, vector <ParameterHandler *>  &parameters );
           /* virtual STATUS load(); */
                   STATUS save( ostream &out);
	   virtual ORDER_STATUS process (GameData * entity, vector <ParameterHandler *>  &parameters,
					 ostream &out, list <OrderModifier *> reportModifiers_);
	   string getKeyword() const; 
    protected:
           string   keyword_;            
           string   description_;        
    private:

};
class GiveOrderPrototype : public OrderPrototype
{
    public:
  GiveOrderPrototype();
            STATUS check(Parser * parser, vector <ParameterHandler *>  &parameters, ostream &out);
            STATUS load(Parser * parser, vector <ParameterHandler *>  &parameters );
	    ORDER_STATUS process (GameData * entity, vector <ParameterHandler *>  &parameters,
				  ostream &out, list <OrderModifier *> reportModifiers_);
    protected:
    private:
  
};
#endif












