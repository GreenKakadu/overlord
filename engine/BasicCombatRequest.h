#ifndef BASIC_COMBAT_REQUEST_H
#define BASIC_COMBAT_REQUEST_H
#include "BasicCombatEngine.h"
class TokenEntity;
class BasicCombatRequest {
public:
BasicCombatRequest(TokenEntity * attacker,
                TokenEntity * defender){attacker_ = attacker; defender_ = defender;}
  ~BasicCombatRequest(){}

protected:
  TokenEntity * attacker_;
  TokenEntity * defender_;
  static BasicCombatEngine * combatType_;

private:

};
#endif
