/***************************************************************************
                             NameOrder.cpp
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/
#include "NameOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnaryMessage.h"
#include "BinaryMessage.h"
#include "TertiaryMessage.h"
#include "EntitiesCollection.h"
extern ReportPattern *	changeNameReporter;

NameOrder * instantiateNameOrder = new NameOrder();

NameOrder::NameOrder(){
  keyword_ = "Name";
  registerOrder_();
  description = string("NAME \"unit name\" \n") +
  "Immediate, one-shot.  This order executes immediately, and changes the visual\n" +
  "name of the unit to be a new string.  Names are used only in reports.\n";
  orderType_   = IMMEDIATE_ORDER;
  mayInterrupt_ = true;
}

STATUS NameOrder::loadParameters(Parser * parser,
                            ParameterList &parameters, Entity * entity )
{
	if(!parseStringParameter(entity, parser,parameters))
        return IO_ERROR;
    else
  		return OK;
}



ORDER_STATUS NameOrder::process (Entity * entity, ParameterList &parameters)
{

//  StringData * h1 = new StringData("zaraza");
//  StringData * nameParameter =dynamic_cast<StringData *>(parameters[0]);
//  assert(nameParameter);
//  string  newName = nameParameter->print();
//  string  newName = (parameters[0])->print();
  string  g1 = ((parameters[0])->print()).c_str();
//  string  g1 = h1->print();
//  entity->setName(newName);
  entity->setName(g1);
//  entity->setName("zaraza");
//  entity->setName((parameters[0])->print());
//   cout<< "NameOrder::process  "<< g1 <<endl;
//   cout<< "NameOrder::process  "<< newName <<endl;
//  entity->addReport(new UnaryMessage(changeNameReporter,new StringData(newName)));
  entity->addReport(new UnaryMessage(changeNameReporter,new StringData(g1)));
//   cout<< "NameOrder::process  "<< (parameters[0])->print() <<endl;
//   cout<< "NameOrder::process  "<< g1 <<endl;
//   cout<< "NameOrder::process  "<< g1 <<endl;
//  entity->addReport(new UnaryMessage(changeNameReporter,new StringData((parameters[0])->print())));
  return SUCCESS;
}

