/***************************************************************************
                          OrderElement.h 
                    .
                             -------------------
    begin                : Wen Aug  7 16:08:00 IST 2002
    copyright            : (C) 2002 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#ifndef ORDER_ELEMENT_H
#define ORDER_ELEMENT_H     
#include <list>
#include <string>
#include <vector>
#include <iostream>
#include "GameData.h"
#include "Parser.h"
#include "OverlordTypes.h"
class OrderElementNode;
#include "OrderPrototype.h"
#include "OrderModifier.h"
#include "ParameterHandler.h"
#include "OrderPrototypesCollection.h"
class OrderElement
{
    public:
        OrderElement(const string & order);
        ~OrderElement();
	STATUS check(ostream &out); 
	ORDER_STATUS process(GameData * entity, ostream &out);
	void save(ostream &out); 
	void load(); 
	void parse(); 
	bool isParsed;
	OrderModifier * getTopDurationModifier();
	OrderModifier * getTopConditionModifier();
	void deleteModifier(OrderModifier * );

    protected:
	/* OrderParameters parameters_; */
	list <OrderModifier *> controlModifiers_;
	list <OrderModifier *> reportModifiers_;
	OrderPrototype * orderPrototype_;
	vector <ParameterHandler *> parameterHandlers_;
	Parser * parser_;
    private:

};
#endif







