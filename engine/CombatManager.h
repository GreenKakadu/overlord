// Similar to BasicConflict
#if !defined(COMBAT_MANAGER_H)
  #define COMBAT_MANAGER_H

#include "BasicCombatEngine.h"
class BasicCombatRequest;
class CombatManager  {
public:
    CombatManager(){}
    virtual ~CombatManager(){}
    virtual void attackAttempt(TokenEntity * attacker,
                    TokenEntity * defender);
    virtual void attackAttempt(BasicCombatRequest * request);
    void process(); // Determine combat participants
    virtual void initialize(){}

protected:
virtual void insertCombatRequest_(BasicCombatRequest *request);
virtual void addCombatRequest_(BasicCombatRequest *request);
virtual BATTLE_RESULT resolve_();
  vector  <BasicCombatRequest *> requests_;

private:


};
extern CombatManager combatManager;
#endif // COMBAT_MANAGER_H
