/***************************************************************************
                             DescribeOrder.cpp 
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "DescribeOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "TertiaryPattern.h"
#include "EntitiesCollection.h"

DescribeOrder * instantiateDescribeOrder = new DescribeOrder();

DescribeOrder::DescribeOrder(){
  keyword_ = "Describe";
  registerOrder_();
  description = string("DESCRIBE [\"unit look\"] \n") +
  "Immediate, one-shot.  Executes immediately, and sets your unit textual\n" +
  "description to the specified string.  That description has no effect on the game,\n" +
  "but is intended for the reports.  Specifying no description text erases your\n" +
  "current description.\n";
  mayInterrupt_ = true;

  orderType_   = IMMEDIATE_ORDER;
}

STATUS DescribeOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
	if(!parseStringParameter(entity, parser,parameters))
        return IO_ERROR;
    else    
  		return OK;
}



ORDER_STATUS DescribeOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
  string  newName = (parameters[0])->print();
  entity->setDescription(newName);
  return SUCCESS;
}

