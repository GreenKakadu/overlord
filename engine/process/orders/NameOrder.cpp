/***************************************************************************
                             NameOrder.cpp 
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "NameOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "TertiaryPattern.h"
#include "EntitiesCollection.h"
extern Reporter *	changeNameReporter;

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
                            vector <AbstractData *>  &parameters, Entity * entity )
{
	if(!parseStringParameter(entity, parser,parameters))
        return IO_ERROR;
    else    
  		return OK;
}



ORDER_STATUS NameOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{

//  StringData * h1 = new StringData("zaraza");
//  StringData * nameParameter =dynamic_cast<StringData *>(parameters[0]);
//  assert(nameParameter);
//  string  newName = nameParameter->printName();
//  string  newName = (parameters[0])->printName();
  string  g1 = ((parameters[0])->printName()).c_str();
//  string  g1 = h1->printName();
//  entity->setName(newName);
  entity->setName(g1);
//  entity->setName("zaraza");
//  entity->setName((parameters[0])->printName());
//   cout<< "NameOrder::process  "<< g1 <<endl;
//   cout<< "NameOrder::process  "<< newName <<endl;
//  entity->addReport(new UnaryPattern(changeNameReporter,new StringData(newName)));
  entity->addReport(new UnaryPattern(changeNameReporter,new StringData(g1)));
//   cout<< "NameOrder::process  "<< (parameters[0])->printName() <<endl;
//   cout<< "NameOrder::process  "<< g1 <<endl;
//   cout<< "NameOrder::process  "<< g1 <<endl;
//  entity->addReport(new UnaryPattern(changeNameReporter,new StringData((parameters[0])->printName())));
  return SUCCESS;
}

