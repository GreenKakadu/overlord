/***************************************************************************
                          BasicCombatManager.h
  Similar to BasicConflict responsible for combat processing
                             -------------------
    begin                : Mon May 24 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#if !defined(BASIC_COMBAT_MANAGER_H)
  #define BASIC_COMBAT_MANAGER_H

class OrderLine;
class TokenEntity;
class BasicCombatEngine;

class BasicCombatManager  {
public:
    BasicCombatManager();
    virtual ~BasicCombatManager(){}
    virtual void attackAttempt(TokenEntity * attacker,
                    TokenEntity * defender,OrderLine * orderId = 0);
    virtual void process() {}
    virtual void initialize(){}

protected:
private:
    BasicCombatEngine  * combatEngine_;

};
extern BasicCombatManager * combatManager;
#endif // BASIC_COMBAT_MANAGER_H
