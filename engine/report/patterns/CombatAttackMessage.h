/***************************************************************************
                          CombatAttackMessage.h
                             -------------------
    begin                : Mar 22 2005
    copyright            : (C) 2005 by Alex Dribin
    email                : Alex.Dribin@gmail.com
 ***************************************************************************/

#ifndef COMBAT_ATTACK_PATTERN_H
#define COMBAT_ATTACK_PATTERN_H

#include "ReportMessage.h"
class BattleInstance;
class TokenEntity;

/**
  *@author Alex Dribin
  */

class CombatAttackMessage : public ReportMessage  {
public:
	CombatAttackMessage(){}
	CombatAttackMessage(int initiative, BattleInstance * attacker, int hits, BattleInstance * target, int damage, int killed, bool killedAll = false);
/*  static void * operator new(size_t size);
  static void   operator delete(void * deadObject, size_t size);*/

  void printReport(ReportPrinter &out) const;
  void clean();
/*	static  CombatAttackMessage * headOfFreeList;
	union
	{
			CombatAttackMessage  * next; //for use in operator new
  		ReportPattern * reporter_;
	};*/
protected:
	~CombatAttackMessage(){}
		int initiative_;
		TokenEntity * attacker_;
		int hits_;
		TokenEntity * target_;
		int damage_;
		int killed_;
		bool killedAll_;


};

#endif
