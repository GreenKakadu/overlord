/***************************************************************************
                          CombatManager.h
  Similar to BasicConflict responsible for combat processing
                             -------------------
    begin                : Mon May 24 2004
    copyright            : (C) 2004 by Alex Dribin
    email                : alexliza@netvision.net.il
 ***************************************************************************/

#if !defined(COMBAT_MANAGER_H)
  #define COMBAT_MANAGER_H

#include  <vector>
#include "BasicCombatManager.h"
class BasicCombatRequest;
using namespace std;

class CombatManager : public BasicCombatManager {
public:
    CombatManager(){}
    virtual ~CombatManager(){}
    virtual void attackAttempt(TokenEntity * attacker,
                    TokenEntity * defender,OrderLine * orderId = 0);
    void process(); // Determine combat participants
    virtual void initialize(){}

protected:
virtual void addCombatRequest_(BasicCombatRequest *request);
  vector  <BasicCombatRequest *> requests_;

private:


};
//extern CombatManager combatManager;
#endif // COMBAT_MANAGER_H
