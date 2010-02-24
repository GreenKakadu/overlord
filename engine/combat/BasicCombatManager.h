/***************************************************************************
                          BasicCombatManager.h
  Similar to BasicConflict responsible for combat processing
                             -------------------
    begin                : Mon May 24 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#if !defined(BASIC_COMBAT_MANAGER_H)
  #define BASIC_COMBAT_MANAGER_H

#include "BasicCombatEngine.h"
class OrderLine;
class TokenEntity;
class FactionEntity;
class BasicCombatEngine;
class GuardingRequest;
class PillageRequest;

class BasicCombatManager  {
public:
    BasicCombatManager();
    virtual ~BasicCombatManager(){}
    virtual void attackAttempt(TokenEntity * attacker,
                    TokenEntity * defender,OrderLine * orderId = 0,
                    void (*funPtr)(TokenEntity * attacker, TokenEntity * defender,const BATTLE_RESULT result) = 0);
     void attackAttempt(FactionEntity * attacker,
                    TokenEntity * defender,OrderLine * orderId = 0,
                    void (*funPtr)(TokenEntity * attacker, TokenEntity * defender,const BATTLE_RESULT result) = 0);
    virtual void process() {}
    virtual void initialize(){}
    virtual void addGuardingRequest(GuardingRequest *){}
    virtual void addPatrolingRequest(GuardingRequest *){}
    virtual void addPillageRequest(PillageRequest *){}
    virtual void removePillageRequest(TokenEntity *, bool){}

protected:
    BasicCombatEngine  * combatEngine_;
private:

};
extern BasicCombatManager * combatManager;
#endif // BASIC_COMBAT_MANAGER_H
