/***************************************************************************
                            BasicCombatRequest.h
                          ------------------
    begin                : Mon May 24 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/
#ifndef BASIC_COMBAT_REQUEST_H
#define BASIC_COMBAT_REQUEST_H
#include "BasicCombatEngine.h"
class TokenEntity;
class OrderLine;

class BasicCombatRequest {
public:
BasicCombatRequest(TokenEntity * attacker, TokenEntity * defender,OrderLine * orderId)
                 {attacker_ = attacker; defender_ = defender;orderId_ = orderId;}
  virtual ~BasicCombatRequest(){}
  virtual void answerRequest(const BATTLE_RESULT result);

protected:
  TokenEntity * attacker_;
  TokenEntity * defender_;
  OrderLine * orderId_;
  static BasicCombatEngine * combatType_;

private:

};
#endif
