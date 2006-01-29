/***************************************************************************
                            BasicCombatRequest.cpp
                          ------------------
    begin                : Mon May 24 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#include "TokenEntity.h"
#include "OrderLine.h"
#include "BasicCombatRequest.h"
//BasicCombatEngine * BasicCombatRequest::combatType_ = &combatEngine;
BasicCombatEngine * BasicCombatRequest::combatType_ = &realCombatEngine;




BasicCombatRequest::BasicCombatRequest(TokenEntity * attacker,
	TokenEntity * defender, OrderLine * orderId,
	void (*funPtr)(TokenEntity * attacker, TokenEntity * defender,
	const  BATTLE_RESULT result))
{
	attacker_ = attacker;
	defender_ = defender;
	orderId_ = orderId;
	postCombatProcessing_ = funPtr;
	location_ = defender->getLocation();
	assert(location_);
}



void BasicCombatRequest::answerRequest(const BATTLE_RESULT result)
{
   if(orderId_)
    orderId_->completeProcessing(attacker_,result);
   if(postCombatProcessing_)
    (*postCombatProcessing_)(attacker_,defender_,result);
}



bool BasicCombatRequest::isEqualTo(BasicCombatRequest * request)
{
  return (this->getLocation() ==  request->getLocation());
}



bool BasicCombatRequest::isValid()
{
  if(location_ == 0)
    return false;
  if(attacker_== 0)
    return false;
  if(defender_== 0)
    return false;
  if(!attacker_->isAlive())  // Dead
    return false;
  if(!defender_->isAlive())  // Dead
    return false;
  return true;
}



string BasicCombatRequest::print()
{
  return  attacker_->print() + (" requests attack on  ") + defender_->print() ;
}

