/***************************************************************************
                             EmailOrder.cpp 
                             -------------------
    begin                : Thu Nov 19 2003
    copyright            : (C) 2003 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "EmailOrder.h"
#include "StringData.h"
#include "Entity.h"
#include "TokenEntity.h"
#include "FactionEntity.h"
#include "UnaryPattern.h"
#include "BinaryPattern.h"
#include "TertiaryPattern.h"
#include "EntitiesCollection.h"

extern Reporter * changeEmailReporter;	
EmailOrder * instantiateEmailOrder = new EmailOrder();

EmailOrder::EmailOrder(){
  keyword_ = "Email";
  registerOrder_();
  description = string("EMAIL new-email-address \n") +
  "This order changes your email address for reports and syntax checks..\n";
  orderType_   = IMMEDIATE_ORDER;
  mayInterrupt_ = true;
}

STATUS EmailOrder::loadParameters(Parser * parser,
                            vector <AbstractData *>  &parameters, Entity * entity )
{
	if(!parseStringParameter(entity, parser,parameters))
        return IO_ERROR;
   if(entityIsTokenEntity(entity,NO_PARSING_REPORT))
            return OK;
   if(entityIsFaction(entity,NO_PARSING_REPORT))
            return OK;
    else    
  		return IO_ERROR;
}



ORDER_STATUS EmailOrder::process (Entity * entity, vector <AbstractData *>  &parameters)
{
  string  newEmail = (parameters[0])->print();
  FactionEntity * faction = dynamic_cast<FactionEntity *>(entity);
  if(faction)
  {
    faction->setEMail(newEmail);
//    faction->setEMail("zaraza");
    entity->addReport(new UnaryPattern(changeEmailReporter,new StringData(newEmail)));
    return SUCCESS;
  }
  TokenEntity * unit = dynamic_cast<TokenEntity *>(entity);
  if(unit)
    unit->getFaction()->setEMail(newEmail);
     
  entity->addReport(new UnaryPattern(changeEmailReporter,new StringData(newEmail))); 		
  return SUCCESS;
}

