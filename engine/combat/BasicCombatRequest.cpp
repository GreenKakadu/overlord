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
BasicCombatEngine * BasicCombatRequest::combatType_ = &combatEngine;



void BasicCombatRequest::answerRequest(const BATTLE_RESULT result)
{
     orderId_->completeProcessing(attacker_,result); 
}


