/***************************************************************************
                          MeleeAttackElement.h
                             -------------------
    begin                : 30 Mar 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#ifndef MELEE_ATTACK_ELEMENT_H
#define MELEE_ATTACK_ELEMENT_H
#include <vector>


/**Contains information for melee attack reporting
This element is not based on Element template
  *@author Alex Dribin
  */
class BattleInstance;
using namespace std;

class MeleeAttackElement {
public: 
	MeleeAttackElement(BattleInstance * targ, int numStrikes, int dam, int died);
	MeleeAttackElement();
	virtual ~MeleeAttackElement();
	BattleInstance * target;
	int hits;
	int damage;
	int killed;
 protected:
};
const MeleeAttackElement operator + (const MeleeAttackElement& leftArgument, const MeleeAttackElement& rightArgument);

typedef vector<MeleeAttackElement> MeleeReport;

#endif
